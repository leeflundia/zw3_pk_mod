#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdarg.h>
#include <stdio.h>

#define NEXPORT 51
void *g_tab[NEXPORT];

static HMODULE g_real = NULL;
static char g_log[MAX_PATH];

static const char *g_envDesc = "unset";
static const char *g_realDesc = "unset";
static const char *g_pinDesc = "n/a";

static const char *CAND[] = {
    "zy_overlay14.dll", "zy_overlay13.dll", "zy_overlay12.dll", "zy_overlay11.dll",
    "zy_overlay10.dll", "zy_overlay9.dll", "zy_overlay8.dll", "zy_overlay7.dll",
    "zy_overlay6.dll", "zy_overlay5.dll", "zy_overlay4.dll", "zy_overlay3.dll",
    "zy_overlay2.dll", "zy_overlay.dll", NULL
};

static void logline(const char *fmt, ...)
{
    FILE *f = fopen(g_log, "a");
    if (!f) return;
    va_list ap; va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
    fputc('\n', f); fclose(f);
}

static BOOL env_is_wine(void)
{
    HMODULE nt = GetModuleHandleA("ntdll.dll");
    if (nt && GetProcAddress(nt, "wine_get_version") != NULL) return TRUE;

    {
        HKEY k;
        if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Wine", 0, KEY_READ, &k) == ERROR_SUCCESS) {
            RegCloseKey(k);
            return TRUE;
        }
    }
    return FALSE;
}

static DWORD WINAPI boot(LPVOID p)
{
    (void)p;
    logline("[proxy] d3d11 代理已随游戏加载 v6-adaptive (pid=%lu)", (unsigned long)GetCurrentProcessId());
    logline("[proxy] real-select env=%s real=%s handle=%p", g_envDesc, g_realDesc, (void *)g_real);
    logline("[proxy] dxgi-pin=%s", g_pinDesc);

    char dir[MAX_PATH], dll[MAX_PATH];
    GetModuleFileNameA(NULL, dir, MAX_PATH);
    char *s = strrchr(dir, '\\');
    if (s) s[1] = 0;

    int i;
    for (i = 0; CAND[i]; i++) {
        lstrcpyA(dll, dir);
        lstrcatA(dll, CAND[i]);
        if (GetFileAttributesA(dll) == INVALID_FILE_ATTRIBUTES) {
            logline("[proxy]   候选 %s 不存在，跳过", CAND[i]);
            continue;
        }
        HMODULE m = LoadLibraryA(dll);
        logline("[proxy]   加载 %s -> %p", CAND[i], (void *)m);
        if (m) return 0;
    }
    logline("[proxy] !! 没有任何面板模块可加载");
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hi, DWORD reason, LPVOID rv)
{
    (void)rv;
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hi);
        char sys[MAX_PATH];
        GetModuleFileNameA(hi, g_log, MAX_PATH);
        char *p = strrchr(g_log, '\\');
        if (p) p[1] = 0;
        lstrcatA(g_log, "zy_proxy.log");

        {
            int wine = env_is_wine();
            char local[MAX_PATH], lreal[MAX_PATH], sysdll[MAX_PATH];

            GetModuleFileNameA(hi, local, MAX_PATH);
            { char *q = strrchr(local, '\\'); if (q) q[1] = 0; }
            lstrcpyA(lreal, local);
            lstrcatA(lreal, "d3d11_real.dll");

            GetSystemDirectoryA(sysdll, MAX_PATH);
            lstrcatA(sysdll, "\\d3d11.dll");

            g_envDesc = wine ? "wine" : "win";
            if (wine) {
                g_real = LoadLibraryA(lreal);
                if (g_real) g_realDesc = "d3d11_real.dll(DXVK)";
                if (!g_real) {
                    g_real = LoadLibraryA(sysdll);
                    if (g_real) g_realDesc = "system32(fallback)";
                }
            } else {
                {
                    char sysdxgi[MAX_PATH];
                    GetSystemDirectoryA(sysdxgi, MAX_PATH);
                    lstrcatA(sysdxgi, "\\dxgi.dll");
                    g_pinDesc = LoadLibraryA(sysdxgi) ? "ok" : "fail";
                }
                g_real = LoadLibraryA(sysdll);
                if (g_real) g_realDesc = "system32";
                if (!g_real) {
                    g_real = LoadLibraryA(lreal);
                    if (g_real) g_realDesc = "d3d11_real.dll(fallback)";
                }
            }
        }

        if (g_real) {
            static const char *names[NEXPORT] = {
                "CreateDirect3D11DeviceFromDXGIDevice", "CreateDirect3D11SurfaceFromDXGISurface",
                "D3D11CoreCreateDevice", "D3D11CoreCreateLayeredDevice", "D3D11CoreGetLayeredDeviceSize",
                "D3D11CoreRegisterLayers", "D3D11CreateDevice", "D3D11CreateDeviceAndSwapChain",
                "D3D11CreateDeviceForD3D12", "D3D11On12CreateDevice", "D3DKMTCloseAdapter",
                "D3DKMTCreateAllocation", "D3DKMTCreateContext", "D3DKMTCreateDevice",
                "D3DKMTCreateSynchronizationObject", "D3DKMTDestroyAllocation", "D3DKMTDestroyContext",
                "D3DKMTDestroyDevice", "D3DKMTDestroySynchronizationObject", "D3DKMTEscape",
                "D3DKMTGetContextSchedulingPriority", "D3DKMTGetDeviceState", "D3DKMTGetDisplayModeList",
                "D3DKMTGetMultisampleMethodList", "D3DKMTGetRuntimeData", "D3DKMTGetSharedPrimaryHandle",
                "D3DKMTLock", "D3DKMTOpenAdapterFromHdc", "D3DKMTOpenResource", "D3DKMTPresent",
                "D3DKMTQueryAdapterInfo", "D3DKMTQueryAllocationResidency", "D3DKMTQueryResourceInfo",
                "D3DKMTRender", "D3DKMTSetAllocationPriority", "D3DKMTSetContextSchedulingPriority",
                "D3DKMTSetDisplayMode", "D3DKMTSetDisplayPrivateDriverFormat", "D3DKMTSetGammaRamp",
                "D3DKMTSetVidPnSourceOwner", "D3DKMTSignalSynchronizationObject", "D3DKMTUnlock",
                "D3DKMTWaitForSynchronizationObject", "D3DKMTWaitForVerticalBlankEvent",
                "D3DPerformance_BeginEvent", "D3DPerformance_EndEvent", "D3DPerformance_GetStatus",
                "D3DPerformance_SetMarker", "EnableFeatureLevelUpgrade", "OpenAdapter10", "OpenAdapter10_2"
            };
            int i;
            for (i = 0; i < NEXPORT; i++)
                g_tab[i] = (void *)GetProcAddress(g_real, names[i]);
        }
        CreateThread(NULL, 0, boot, NULL, 0, NULL);
    }
    return TRUE;
}
