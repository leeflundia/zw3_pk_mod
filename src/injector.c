#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <wchar.h>

static DWORD find_pid(const wchar_t *needle)
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return 0;
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);
    DWORD pid = 0;
    if (Process32FirstW(snap, &pe)) {
        do {
            if (wcsstr(pe.szExeFile, needle) != NULL) { pid = pe.th32ProcessID; break; }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
    return pid;
}

int wmain(int argc, wchar_t **argv)
{
    if (argc < 3) {
        wprintf(L"usage: inject.exe <process-name-substring> <dll-path>\n");
        return 1;
    }
    const wchar_t *needle = argv[1];
    const wchar_t *dll    = argv[2];

    DWORD pid = find_pid(needle);
    if (!pid) { wprintf(L"[x] process not found: %ls\n", needle); return 2; }

    HANDLE h = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
                           PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
                           FALSE, pid);
    if (!h) {
        wprintf(L"[x] OpenProcess failed pid=%lu err=%lu\n", pid, GetLastError());
        return 3;
    }

    SIZE_T sz = (wcslen(dll) + 1) * sizeof(wchar_t);
    LPVOID mem = VirtualAllocEx(h, NULL, sz, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!mem) { wprintf(L"[x] VirtualAllocEx failed err=%lu\n", GetLastError()); return 4; }

    if (!WriteProcessMemory(h, mem, dll, sz, NULL)) {
        wprintf(L"[x] WriteProcessMemory failed err=%lu\n", GetLastError());
        return 5;
    }

    HMODULE k32 = GetModuleHandleW(L"kernel32.dll");
    FARPROC load = GetProcAddress(k32, "LoadLibraryW");
    if (!load) { wprintf(L"[x] LoadLibraryW not found\n"); return 6; }

    HANDLE th = CreateRemoteThread(h, NULL, 0, (LPTHREAD_START_ROUTINE)load, mem, 0, NULL);
    if (!th) { wprintf(L"[x] CreateRemoteThread failed err=%lu\n", GetLastError()); return 7; }

    DWORD wr = WaitForSingleObject(th, 15000);
    DWORD ec = 0;
    GetExitCodeThread(th, &ec);
    wprintf(L"[ok] pid=%lu injected. thread=%s exitCode=0x%08lX\n",
            pid, (wr == WAIT_OBJECT_0) ? L"done" : L"timeout", ec);

    if (wr == WAIT_OBJECT_0 && ec == 0)
        wprintf(L"[!] LoadLibraryW returned NULL — DLL 加载失败（位数不对/路径不存在/依赖缺失）\n");

    CloseHandle(th);
    VirtualFreeEx(h, mem, 0, MEM_RELEASE);
    CloseHandle(h);
    return 0;
}
