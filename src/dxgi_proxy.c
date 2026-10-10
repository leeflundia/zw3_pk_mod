#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdarg.h>
#include <stdio.h>

#define NEXPORT 20
void *g_tab[NEXPORT];

static HMODULE g_real = NULL;
static char g_log[MAX_PATH];

static const char *g_envDesc = "unset";
static const char *g_realDesc = "unset";

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
    logline("[dxgip] dxgi 垫片已随进程加载 v1 (pid=%lu)", (unsigned long)GetCurrentProcessId());
    logline("[dxgip] real-select env=%s real=%s handle=%p", g_envDesc, g_realDesc, (void *)g_real);
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
        lstrcatA(g_log, "dxgi_proxy.log");

        {
            int wine = env_is_wine();
            char local[MAX_PATH], lreal[MAX_PATH], sysdll[MAX_PATH];

            GetModuleFileNameA(hi, local, MAX_PATH);
            { char *q = strrchr(local, '\\'); if (q) q[1] = 0; }
            lstrcpyA(lreal, local);
            lstrcatA(lreal, "dxgi_real.dll");

            GetSystemDirectoryA(sysdll, MAX_PATH);
            lstrcatA(sysdll, "\\dxgi.dll");

            g_envDesc = wine ? "wine" : "win";
            if (wine) {
                g_real = LoadLibraryA(lreal);
                if (g_real) g_realDesc = "dxgi_real.dll(DXVK)";
                if (!g_real) {
                    g_real = LoadLibraryA(sysdll);
                    if (g_real) g_realDesc = "system32(fallback)";
                }
            } else {
                g_real = LoadLibraryA(sysdll);
                if (g_real) g_realDesc = "system32";
                if (!g_real) {
                    g_real = LoadLibraryA(lreal);
                    if (g_real) g_realDesc = "dxgi_real.dll(fallback)";
                }
            }
        }

        if (g_real) {
            static const char *names[NEXPORT] = {
                "CreateDXGIFactory", "CreateDXGIFactory1", "CreateDXGIFactory2",
                "DXGIDeclareAdapterRemovalSupport", "DXGIGetDebugInterface1",
                "ApplyCompatResolutionQuirking", "CompatString", "CompatValue",
                "DXGID3D10CreateDevice", "DXGID3D10CreateLayeredDevice",
                "DXGID3D10GetLayeredDeviceSize", "DXGID3D10RegisterLayers",
                "DXGIDisableVBlankVirtualization", "DXGIDumpJournal",
                "DXGIReportAdapterConfiguration", "PIXBeginCapture", "PIXEndCapture",
                "PIXGetCaptureState", "SetAppCompatStringPointer", "UpdateHMDEmulationStatus"
            };
            int i;
            for (i = 0; i < NEXPORT; i++)
                g_tab[i] = (void *)GetProcAddress(g_real, names[i]);
        }
        CreateThread(NULL, 0, boot, NULL, 0, NULL);
    }
    return TRUE;
}
