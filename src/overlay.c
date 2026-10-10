#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <d3d11.h>
#include <dxgi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define RUNTIME_BASE 0x005DE580u
#define IDX_ADDR     0x005DD43Cu
#define CITY_MIRROR  0x005E4F58u

#define CITY_N 6
static unsigned int CITY_ARR[CITY_N] = { 0x005DDAA8u, 0x005F7FA4u, 0x005DDCF8u,
                                         0x005FB490u, 0x005FC400u , 0x005F6360u};
#define CITY_FARM2   0x005FC8CCu

#define UI_W0   195
#define HDR_H   34
#define ROW_H   24
#define PAD_H   80
#define UI_H0   (HDR_H + 7 * ROW_H + PAD_H)
#define MARGIN  6
#define UI_COL_BG       RGB(18, 20, 26)
#define UI_COL_ROWHL    RGB(44, 50, 60)
#define UI_COL_BAR      RGB(81, 110, 142)
#define UI_COL_TXT      RGB(240, 241, 242)
#define UI_COL_TXTDIM   RGB(168, 176, 186)
#define UI_COL_HINT     RGB(112, 120, 132)
#define UI_COL_MSG      RGB(126, 158, 196)
#define UI_COL_OFFDIM   RGB(96, 102, 112)
#define UI_COL_EDGE_CITY    RGB(150, 128, 92)
#define UI_COL_EDGE_FAC     RGB(110, 96, 140)
#define UI_COL_EDGE_MAP     RGB(81, 110, 142)
#define UI_COL_TITLE_CITY   RGB(214, 190, 150)
#define UI_COL_TITLE_FAC    RGB(180, 160, 210)
#define UI_COL_TITLE_MAP    RGB(150, 180, 214)
#define UI_COL_STEPUP     RGB(120, 168, 128)
#define UI_COL_STEPDOWN   RGB(188, 120, 118)
#define UI_COL_OVER       RGB(104, 176, 130)
#define UI_COL_Z_RED      RGB(198, 96, 90)
#define UI_COL_Z_YEL      RGB(214, 178, 92)
#define UI_COL_Z_TEA      RGB(98, 168, 172)
#define UI_COL_Z_WHT      RGB(236, 238, 240)
#define UI_COL_VALCUR     RGB(255, 238, 150)
#define UI_TITLE_Y    7
#define UI_HINT_GAP   8
#define UI_HINT_LH    14
#define UI_TEXT_X     12
#define UI_HINT_X     10

typedef HRESULT (WINAPI *PresentFn)(IDXGISwapChain *, UINT, UINT);
typedef HRESULT (WINAPI *Present1Fn)(void *, UINT, UINT, const void *);

#define CHAR_N     8
static const wchar_t *FNAME[CHAR_N] = { L"武力", L"统率", L"智谋", L"政治",
                                   L"忠诚", L"段",   L"经验", L"训练度" };
static const unsigned int FOFF[CHAR_N] = { 0x000000u, 0x01D5ECu, 0x02119Cu, 0x0008F8u,
                                      0x00B5BCu, 0x01E7E4u, 0x02047Cu, 0x00BC0Cu };

static const wchar_t *CNAME[CITY_N] = { L"农业", L"商业", L"治安", L"城墙", L"士兵", L"兵粮" };
static const unsigned int CSTEP[CITY_N] = { 100u, 10u, 5u, 5u, 500u, 5u };

#define PAGE_ID      0x005F8444u
#define MODE_NONE    0
#define MODE_CHAR    1
#define MODE_CITY    2
#define MODE_FACTION 3

#define PANEL_OFF     0
#define PANEL_SOLDIER 1
#define PANEL_MAIN    2
#define PANEL_FUNC    3
#define PANEL_GENERAL 4
#define PANEL_MAP    5
#define PANEL_TRAIN   6
#define PANEL_MECH    7

#define SCENE_NONE         0
#define SCENE_TITLE        2
#define SCENE_CITY_CMD     3
#define SCENE_CITY_DETAIL  4
#define SCENE_CHAR         5
#define SCENE_FACTION      6
#define SCENE_OTHER        7

#define FACTION_N 2
static const unsigned int  FARR[FACTION_N]   = { 0x005F6938u, 0x005EAA60u };
static const wchar_t      *FFNAME[FACTION_N] = { L"总资金", L"命令点" };
static const unsigned int  FSTEP[FACTION_N]  = { 1000u, 10u };

static const unsigned int CAP_CITY[CITY_N]   = { 9999u, 600u, 99u, 250u, 40000u, 250u };
static const unsigned int CAP_FAC[FACTION_N] = { 99999u, 99u };
static const unsigned int CAP_CHAR[CHAR_N]   = { 30u, 30u, 30u, 30u, 30u, 15u, 99u, 99u };

#define PANEL_A 170u

#define OVW_MAX 1920
#define OVH_MAX 1080
#define OV_DIR  "ovimg"
#define OV_FILE "SCREEN.BMP"

#define ACT_NONE      0
#define ACT_UP        1
#define ACT_DOWN      2
#define ACT_INC       3
#define ACT_DEC       4
#define ACT_TOGGLE    6
#define ACT_ZOOM_OUT  7
#define ACT_ZOOM_IN   8
#define ACT_CONFIRM  10
#define ACT_MAXFIELD 30
#define ACT_REFRESH  11
#define ACT_ALL      17
#define ACT_EXIT     18

#define ACT_FULL     24

#define ACT_OVIMG     28
#define ACT_DIAG      31
#define ACT_FULLTRN   29
#define PANEL_CHEAT   8
#define PANEL_DELEG   9

#define ACTED_FLAG   0x005E48D0u
#define ACTED_GRAN   4
#define ACTED_N      404

#define RVA_ADD_TRAIN     0x383BFu
#define RVA_TRAIN_ABILITY 0x385BFu
#define RVA_GAME_RAND     0x37659u
#define RVA_ADD_EXP       0x383FEu
#define RVA_REFRESH_POWER 0x38982u
#define RVA_DEV_FARM      0x381D7u
#define RVA_DEV_COMM      0x38189u
#define RVA_DEV_ORDER     0x37FDAu
#define RVA_DEV_WALL      0x3801Cu
#define RVA_DEV_FOOD      0x3806Du
#define RVA_CMD_ORDER     0x4DC32u
#define RVA_CMD_FOOD      0x4D659u
#define RVA_CMD_WALL      0x4D519u
#define RVA_CMD_TROOP     0x4D348u
#define SETTLE_FAC        0x005DDCF8u
#define DONE_BITS         0x005FB924u
#define TROOP_DEV_ARR     0x005FC404u
#define DELEG_ITEMS       6
#define FAC_FUNDS         0x005F6934u
#define CAP_TRP(c)        deleg_cap((c) - 1u, 4)
#define CAP_LVL_ARR       0x005E74A8u
#define CAP_COMM_ARR      0x005F65ACu
#define CAP_WALL_ARR      0x005E8010u
#define CAP_FOOD_T        0x005F79E8u
#define CAP_TRP_ARR       0x005ED638u
#define DCAP_FARM         0x005ED3ECu
#define DCAP_COMM         0x005F65A8u
#define DCAP_WALL         0x005E800Cu
#define DCAP_FOOD         0x005F79E4u
#define DCAP_TRP          0x005ED634u
#define CITY_SIZECAP      0x005E6990u
#define FARM_DEV_ARR      0x005E6994u
#define COMM_DEV_ARR      0x005F7FA8u
#define ORDER_DEV_ARR     0x005DDCFCu
#define WALL_DEV_ARR      0x005FB494u
#define FOOD_DEV_ARR      0x005F6364u
#define FAC_TRP_NOW       0x005F86C4u
#define FAC_TRP_CAP       0x005DCF38u
#define UNREST_ARR        0x005DCFA0u
#define POOL_ARR          0x005DDAACu
#define RVA_FUNDS_ADD     0x37D46u
#define POP_ARR           0x005E74ACu
#define BUDGET_ARR        0x005DDAB0u

#define TRAIN_ARR         0x005EA18Cu
#define CMD_POINT_ARR     0x005EAA5Cu

#define CHAR_TROOP_ARR    0x005F9C6Cu

#define RVA_TROOP_CLAMP   0x38935u

#define RVA_OFFICE_CAP    0x4B574u
#define TRAIN_MAX         99u

#define SAC_TAG "r91"

#define OFFICE_DEMOTE_ADDR 0x004451E3u

#define DEATH_A_BASE_ADDR  0x0043095Fu
#define DEATH_A_JUDGE_ADDR 0x00430993u
#define DEATH_B_LVL_ADDR   0x00430F78u
#define DEATH_B_JUDGE_ADDR 0x00430FA0u

#define NAME_TAB   0x004E7DBCu
#define CHAR_IDX_MAX   866u

#define UNIT_IDX_MAX   403u

#define CTRL_SLOT         0x00612888u
#define MENU_CTRL_LO      0x005DCAD8u
#define MENU_CTRL_HI      0x005DCC00u

#define CHAR_DETAIL_CTRL  0x005DCF18u
#define FACTION_PAGE_CTRL 0x005DCDF8u
#define FACTION_GRID_CURSOR (FACTION_PAGE_CTRL + 8u)

#define CITYPTR_BASE  0x004E7928u
#define CITY_NAME_MAX 148u
#define NAME_VA_LO    0x00504100u
#define NAME_VA_HI    0x00504900u

#define TROOP_ARR   0x005FC400u
#define CITY_MAXIDX 147u

#define CITY_OWNER  0x005E1B08u

#define PLAYER_FAC  0x005FCD60u

#define CITY_POLICY_ARR 0x005FC408u
#define GOV_CITY_ARR    0x005FE0B4u

#define PULL_VIS    5

#define CHAR_LOC_ARR  0x005DD458u

#define RVA_REBUILD_ALL 0x0004AB27u

#define RVA_CMD_TBL_RESET 0x0004B5D0u

#define RVA_INV_DESC    0x0004B661u

#define CHAR_OFFICE_ARR 0x005E79C0u

#define CHAR_FAC_ARR 0x005FD3B4u

#define FACTION_MAX 32u

#define TRAIN_LIST_MAX 200

#define DXBC_VS_LEN 564

#define DXBC_PS_LEN 624

static unsigned int cap_for(int mode, int field)
{
    if (mode == MODE_CITY    && field >= 0 && field < CITY_N)    return CAP_CITY[field];
    if (mode == MODE_FACTION && field >= 0 && field < FACTION_N) return CAP_FAC[field];
    if (mode == MODE_CHAR    && field >= 0 && field < CHAR_N)    return CAP_CHAR[field];
    return 0xFFFFFFFFu;
}

static int text_w(HDC dc, const wchar_t *s)
{
    SIZE sz;
    GetTextExtentPoint32W(dc, s, lstrlenW(s), &sz);
    return sz.cx;
}

static int wrap_text(HDC dc, const wchar_t *txt, int x, int y, int lh, int avail_px)
{
    wchar_t line[96], tok[96];
    int nl = 0;
    const wchar_t *p = txt;
    line[0] = 0;
    if (!txt[0]) return 0;
    while (*p) {
        int t = 0;
        while (p[t] && p[t] != 0x00B7) t++;
        if (p[t] == 0x00B7) {
            t++;
            while (p[t] == L' ') t++;
        }
        if (t > 90) t = 90;
        lstrcpynW(tok, p, t + 1);
        p += t;
        if (line[0] && (text_w(dc, line) + text_w(dc, tok) > avail_px ||
                        lstrlenW(line) + lstrlenW(tok) >= 90)) {
            TextOutW(dc, x, y + nl * lh, line, lstrlenW(line));
            nl++;
            line[0] = 0;
        }
        lstrcatW(line, tok);
    }
    if (line[0]) { TextOutW(dc, x, y + nl * lh, line, lstrlenW(line)); nl++; }
    return nl;
}

static int g_cheat_on = 0;

static COLORREF mode_edge(int m);
static COLORREF mode_title(int m);

static volatile LONG g_panelLive = 1;

static int    g_ovOn    = 0;

static HBITMAP g_dib = NULL;
static void   *g_bits = NULL;
static UINT    g_dibW = 0, g_dibH = 0;

static float  g_scale    = 0.0f;
static float  g_scaleAdj = 1.0f;
static UINT   g_uiW = UI_W0, g_uiH = UI_H0;
static int    g_rowsCur = 7;
static int    g_posX = 6, g_posY = 6;

static int     g_dirty = 1;
static int     g_show  = 1;

static int     g_blendPath = 1;

static LARGE_INTEGER g_qpcFreq = {{ 0, 0 }};
static ULONGLONG     g_ovCost  = 0;
static LONG          g_fpsN    = 0;
static LONG          g_blendN  = 0;
static DWORD         g_fpsWin0 = 0;
static int     g_mode  = MODE_CHAR;
static int     g_cur   = 0;
static unsigned int g_key = 0xFFFFFFFFu;
static unsigned int g_lastVals[12] = { 0 };
static DWORD   g_hkLast = 0;
static LONG    g_frames = 0;
static int     g_logged = 0;

static void logf_(const char *fmt, ...);

static int mod_dir(char *out)
{
    char dir[MAX_PATH], *p;
    out[0] = 0;
    if (!GetModuleFileNameA(NULL, dir, (DWORD)sizeof(dir))) return 0;
    p = strrchr(dir, '\\');
    if (!p) return 0;
    *p = 0;
    lstrcpynA(out, dir, MAX_PATH);
    return 1;
}

static int          g_pull_mode;

static void logf_(const char *fmt, ...);

static volatile LONG g_wheelDir   = 0;
static volatile LONG g_wheelX     = 0;
static volatile LONG g_wheelY     = 0;

static LONG    g_keyHits   = 0;

static int ctl_ok(void)
{
    return (GetAsyncKeyState(VK_CONTROL)   & 0x8000) ||
           (GetAsyncKeyState(VK_LCONTROL)  & 0x8000) ||
           (GetAsyncKeyState(VK_RCONTROL)  & 0x8000);
}
static int alt_ok(void)
{
    return (GetAsyncKeyState(VK_MENU) & 0x8000) ||
           (GetAsyncKeyState(VK_LMENU)& 0x8000) ||
           (GetAsyncKeyState(VK_RMENU)& 0x8000);
}

static unsigned int rd32(unsigned int a);

static int g_demote_on = 1;

static void menu_inject(void)
{
    logf_("[menu] v210-delegpersist 启动（修委任档位被重置三bug：重开不拉满/改档即存/首都命令菜单不跳修改器）");
}

static int g_death_state = -1;

static const char *death_state_name(int s)
{
    return (s == 1) ? "调低" : (s == 2) ? "关掉战死" : "原版";
}

static LRESULT CALLBACK ms_proc(int code, WPARAM w, LPARAM l)
{
    if (code == HC_ACTION && w == WM_MOUSEWHEEL && g_show) {
        MSLLHOOKSTRUCT *ms = (MSLLHOOKSTRUCT *)l;
        short d = (short)HIWORD(ms->mouseData);

        int hx = (int)ms->pt.x - g_posX;
        int hy = (int)ms->pt.y - g_posY;

        if (d != 0 && g_panelLive && hx >= 0 && hx < (int)g_uiW && hy >= 0 && hy < (int)g_uiH) {
            InterlockedExchange(&g_wheelX, ms->pt.x);
            InterlockedExchange(&g_wheelY, ms->pt.y);
            InterlockedExchange(&g_wheelDir, d > 0 ? 1 : -1);
            return 1;
        }
    }
    return CallNextHookEx(NULL, code, w, l);
}

static void logf_(const char *fmt, ...)
{
    static char s_logPath[MAX_PATH] = { 0 };
    if (!s_logPath[0]) {
        if (mod_dir(s_logPath)) lstrcatA(s_logPath, "\\zw3_overlay.log");
        else lstrcpyA(s_logPath, "zw3_overlay.log");
    }
    FILE *f = fopen(s_logPath, "a");
    if (!f) return;
    va_list ap; va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
    fputc('\n', f); fclose(f);
}

static const char *l10(const wchar_t *w)
{
    static char ob[2][64];
    static int ri = 0;
    char *p = ob[ri];
    ri ^= 1;
    p[0] = 0;
    if (!w) return p;
    if (WideCharToMultiByte(65001, 0, w, -1, p, 64, NULL, NULL) <= 0)
        p[0] = 0;
    return p;
}

static unsigned int rd32(unsigned int a) { return *(volatile unsigned int *)a; }
static void wr32(unsigned int a, unsigned int v) { *(volatile unsigned int *)a = v; }

static int  g_guard_on = 1;
static long g_guard_ok = 0;
static long g_guard_blocked = 0;

static int char_idx_valid(unsigned int idx)
{
    unsigned int p;
    if (idx > UNIT_IDX_MAX) return 0;
    p = rd32(NAME_TAB + idx * 4u);
    if (p == 0u || p < 0x00400000u || p > 0x00600000u) return 0;
    return *(volatile unsigned char *)p != 0;
}

static int ctx_writable(int mode, unsigned int num)
{
    unsigned int pg = rd32(PAGE_ID);
    if (mode == MODE_CHAR)    return ((pg == 5u || pg == 2u) && rd32(CTRL_SLOT) == CHAR_DETAIL_CTRL) && char_idx_valid(num);
    if (mode == MODE_FACTION) return (pg == 3u) && (rd32(CTRL_SLOT) == FACTION_PAGE_CTRL) && (num <= 10u);
    if (mode == MODE_CITY)    return (num >= 1u && num <= 200u);
    return 0;
}

static int addr_writable(int mode, unsigned int num, int field, unsigned int addr)
{
    if (mode == MODE_CHAR) {
        if (field < 0 || field >= CHAR_N) return 0;
        return addr == (RUNTIME_BASE + num * 4u + FOFF[field]);
    }
    if (mode == MODE_CITY) {
        if (field < 0 || field >= CITY_N) return 0;
        if (field == 0 && addr == (CITY_FARM2 + num * 4u)) return 1;
        return addr == (CITY_ARR[field] + num * 4u);
    }
    if (mode == MODE_FACTION) {
        if (field < 0 || field >= FACTION_N) return 0;
        return addr == (FARR[field] + num * 4u);
    }
    return 0;
}

static void wr32g(unsigned int a, unsigned int v, int mode, unsigned int num, int field)
{
    unsigned int old;
    if (!g_guard_on) { wr32(a, v); return; }
    if (!ctx_writable(mode, num)) {
        g_guard_blocked++;
        logf_("[guard] 拒绝写入：语境不允许 addr=%08X val=%u mode=%d num=%u field=%d pg=%u（累计拒绝 %ld）",
              a, v, mode, num, field, rd32(PAGE_ID), g_guard_blocked);
        return;
    }
    if (!addr_writable(mode, num, field, a)) {
        g_guard_blocked++;
        logf_("[guard] 拒绝写入：地址越界 addr=%08X val=%u mode=%d num=%u field=%d（累计拒绝 %ld）",
              a, v, mode, num, field, g_guard_blocked);
        return;
    }
    {
        unsigned int capv = cap_for(mode, field);
        if (v > capv) {
            g_guard_blocked++;
            logf_("[guard] 拒绝写入：值越上限 addr=%08X val=%u>cap=%u mode=%d num=%u field=%d（累计拒绝 %ld）",
                  a, v, capv, mode, num, field, g_guard_blocked);
            return;
        }
    }
    old = rd32(a);
    wr32(a, v);
    g_guard_ok++;
    logf_("[write] %08X: %u -> %u (mode=%d num=%u field=%d)",
          a, old, v, mode, num, field);
}

static int detect_mode(unsigned int *numOut)
{
    unsigned int pg = rd32(PAGE_ID);
    unsigned int a  = rd32(IDX_ADDR);
    unsigned int b  = rd32(CITY_MIRROR);

    if (pg == 3u && a >= 1u && a <= 11u && rd32(CTRL_SLOT) == FACTION_PAGE_CTRL
            && a != rd32(FACTION_GRID_CURSOR) + 1u) {

        if (numOut) *numOut = a - 1u;
        return MODE_FACTION;
    }

    if ((pg == 5u || pg == 2u) && a >= 1u && rd32(CTRL_SLOT) == CHAR_DETAIL_CTRL) {

        if (numOut) *numOut = a;
        return MODE_CHAR;
    }

    {
        unsigned int cw = rd32(CTRL_SLOT);
        int in_menu_win = (cw >= MENU_CTRL_LO && cw < MENU_CTRL_HI);
        if (pg != 0u && a == b && a >= 1u && a <= 200u && !in_menu_win) {
            if (numOut) *numOut = a + 1u;
            return MODE_CITY;
        }
    }
    if (numOut) *numOut = 0u;
    return MODE_NONE;
}

static int mode_rows(int mode)
{
    if (mode == MODE_CITY)    return CITY_N;
    if (mode == MODE_FACTION) return FACTION_N;
    if (mode == MODE_CHAR)    return CHAR_N;
    return 0;
}

static const wchar_t *name_of(unsigned int idx)
{
    static wchar_t ob[24];
    unsigned int pv = 0;
    SIZE_T got = 0;
    char raw[24];

    if (idx >= CHAR_IDX_MAX) return NULL;
    if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)(0x004E7DBCu + idx * 4u),
                           &pv, 4, &got) || got != 4)
        return NULL;
    if (pv < 0x00400000u || pv > 0x01100000u) return NULL;
    if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)pv, raw, 23, &got) || got == 0)
        return NULL;
    raw[23] = 0;
    if (MultiByteToWideChar(936, 0, raw, -1, ob, 24) <= 0) return NULL;
    return ob;
}

static wchar_t g_cnBuf[24];

static const wchar_t *city_name_of(unsigned int cidx)
{
    unsigned int pv = 0;
    SIZE_T got = 0;
    char raw[24];

    if (cidx >= CITY_NAME_MAX) return NULL;
    if (!ReadProcessMemory(GetCurrentProcess(),
                           (LPCVOID)(CITYPTR_BASE + cidx * 4u), &pv, 4, &got) || got != 4)
        return NULL;
    if (pv < NAME_VA_LO || pv > NAME_VA_HI) return NULL;
    if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)pv, raw, 23, &got) || got == 0)
        return NULL;
    raw[23] = 0;
    if (MultiByteToWideChar(936, 0, raw, -1, g_cnBuf, 24) <= 0) return NULL;
    return g_cnBuf;
}

static unsigned int g_player_f = 0xFFFFFFFFu;
static unsigned int player_fac(void)
{
    unsigned int f = rd32(PLAYER_FAC);
    if (f < 1u || f > FACTION_MAX) return 0xFFFFFFFFu;
    return f;
}
static int fac_trusted(void) { return g_player_f != 0xFFFFFFFFu; }

static wchar_t g_pull_msg[64] = L"";

static int          g_pull_mode = 0;
static unsigned int g_pull_dst  = 0;
static int          g_pull_cur  = 0;
static int          g_pull_top  = 0;
static int          g_lastScene = SCENE_NONE;
static int          g_pull_n    = 0;
static unsigned int g_pull_city[148];
static unsigned int g_pull_amt[148];
static wchar_t      g_pull_name[148][16];
static unsigned int g_gen_num[866];
static unsigned int g_gen_city[866];
static int          g_gen_n = 0;
static wchar_t      g_pull_dstname[24];
static long         g_pull_ops = 0;
static unsigned char g_deleg[CITY_MAXIDX + 2];
static unsigned char g_deleg_cfg[CITY_MAXIDX + 2];
static unsigned short g_deleg_target[CITY_MAXIDX + 2][DELEG_ITEMS];
static unsigned int  g_deleg_cost[CITY_MAXIDX + 2][DELEG_ITEMS];
static const wchar_t *DELEG_LBL[DELEG_ITEMS] = {
    L"农业", L"商业", L"治安", L"城墙", L"士兵", L"兵粮"
};
static unsigned int deleg_cap(unsigned int ci, int k)
{
    unsigned int v;
    switch (k) {
    case 0:
        return (rd32(CAP_LVL_ARR + ci * 4u) + 1u) * 50u;
    case 1:
        v = rd32(CAP_COMM_ARR + ci * 4u);
        if (v < 10u || v > 2000u) v = 600u;
        return v;
    case 2:
        return 99u;
    case 3:
        v = rd32(CAP_WALL_ARR + ci * 4u);
        if (v > 999u) v = 999u;
        if (v < 10u)  v = 10u;
        return v;
    case 4:
        v = rd32(CAP_TRP_ARR + ci * 4u);
        if (v == 0u || v > 30000u) v = 30000u;
        return v;
    default:
        v = rd32(CAP_FOOD_T + ci * 4u) * 8u / 10u;
        if (v < 10u) v = 10u;
        return v;
    }
}
static unsigned int deleg_dev_addr(int k)
{
    return (k == 3) ? WALL_DEV_ARR : (k == 5) ? FOOD_DEV_ARR : (k == 4) ? TROOP_DEV_ARR : 0u;
}
static unsigned int deleg_dcap(unsigned int c, int k)
{
    static const unsigned int DCAP[DELEG_ITEMS] = {
        DCAP_FARM, DCAP_COMM, 0u, DCAP_WALL, DCAP_TRP, DCAP_FOOD
    };
    unsigned int v;
    if (k == 2) return 99u;
    v = rd32(DCAP[k] + c * 4u);
    if (v == 0u || v > 60000u) v = CAP_CITY[k];
    if (k == 0 || k == 1 || k == 3 || k == 4 || k == 5) {
        unsigned int cur = rd32(CITY_ARR[k] + c * 4u);
        if (v <= cur) v = cur + 40u;
    }
    return v;
}
static int city_is_gate(unsigned int c)
{
    if (c < 1u || c > CITY_MAXIDX) return 0;
    return rd32(CITY_SIZECAP + c * 4u) == 50u &&
           rd32(DCAP_COMM + c * 4u) == 0u;
}
static int deleg_will(unsigned int c, int k)
{
    unsigned int cur, tgt;
    if (c < 1u || c > CITY_MAXIDX) return 0;
    if (!g_deleg[c]) return 0;
    if (k == 0 || k == 1 || k == 2 || k == 4) {
        if (city_is_gate(c)) return 0;
    }
    tgt = g_deleg_target[c][k];
    if (tgt == 0u) return 0;
    cur = rd32(CITY_ARR[k] + c * 4u);
    return cur < tgt;
}static int deleg_fill(unsigned int c, int k, unsigned int val)
{
    unsigned int cap = deleg_dcap(c, k);
    int p = cap ? (int)(val * 100u / cap) : 0;
    if (p > 100) p = 100;
    if (p < 0)   p = 0;
    return p;
}
static int deleg_pct_cache(unsigned int c, int k)
{
    unsigned int cur = rd32(CITY_ARR[k] + c * 4u);
    unsigned int cap = deleg_dcap(c, k);
    int p = cap ? (int)(cur * 100u / cap) : 0;
    if (p > 100) p = 100;
    return p;
}
static int deleg_tier_of(unsigned int c, int k)
{
    unsigned int cap = deleg_dcap(c, k), t = g_deleg_target[c][k];
    int tier;
    if (cap == 0u || t == 0u) return 0;
    tier = (int)((t * 4u + cap / 2u) / cap);
    if (tier < 1) tier = 1;
    if (tier > 4) tier = 4;
    return tier;
}
static void deleg_set_tier(unsigned int c, int k, int tier)
{
    unsigned int cap = deleg_dcap(c, k);
    if (tier <= 0) g_deleg_target[c][k] = 0u;
    else if (tier >= 4) g_deleg_target[c][k] = (unsigned short)cap;
    else g_deleg_target[c][k] = (unsigned short)(cap * (unsigned int)tier / 4u);
    g_deleg_cfg[c] = 1;
}
static COLORREF deleg_zone_color(int tier)
{
    switch (tier) {
    case 1: return UI_COL_Z_RED;
    case 2: return UI_COL_Z_YEL;
    case 3: return UI_COL_Z_TEA;
    default: return UI_COL_Z_WHT;
    }
}
static unsigned int deleg_grow_disp(unsigned int c, int k, unsigned int amt)
{
    unsigned int a   = CITY_ARR[k] + c * 4u;
    unsigned int cur = rd32(a), cap = deleg_dcap(c, k), nv, got;
    if (cur >= cap || amt == 0u) return 0u;
    nv = cur + amt;
    if (nv > cap) nv = cap;
    got = nv - cur;
    if (k == 0) {
        unsigned int u  = UNREST_ARR + (c - 1u) * 4u;
        unsigned int un = rd32(u);
        if (got > un) { got = un; nv = cur + got; }
        if (got == 0u) return 0u;
        wr32(u, un - got);
    }
    wr32(a, nv);
    return got;
}
static int deleg_budget(unsigned int c, int *unk)
{
    static const int HK[4] = { 2, 3, 4, 5 };
    static const unsigned int EST[6] = { 0u, 0u, 105u, 410u, 165u, 410u };
    int i, sum = 0;
    *unk = 0;
    for (i = 0; i < 4; i++) {
        if (!deleg_will(c, HK[i])) continue;
        if (g_deleg_cost[c][HK[i]]) sum += (int)g_deleg_cost[c][HK[i]];
        else if (EST[HK[i]])        sum += (int)EST[HK[i]];
        else *unk = 1;
    }
    return sum;
}
static const int DELEG_HK[4]   = { 2, 5, 3, 4 };
static const unsigned int DELEG_HR[4] = {
    RVA_CMD_ORDER, RVA_CMD_FOOD, RVA_CMD_WALL, RVA_CMD_TROOP
};
static const wchar_t *deleg_block_reason(unsigned int c, int k)
{
    unsigned int ci = c - 1u, cur;
    switch (k) {
    case 0:
        cur = rd32(CITY_ARR[0] + c * 4u);
        if (cur >= deleg_dcap(c, 0)) return L"农·顶";
        if (rd32(UNREST_ARR + ci * 4u) == 0u) return L"农·无乱";
        return NULL;
    case 1:
        cur = rd32(CITY_ARR[1] + c * 4u);
        if (cur >= deleg_dcap(c, 1)) return L"商·顶";
        return NULL;
    case 2:
        return NULL;
    case 3:
        return NULL;
    case 4:
        if (rd32(TROOP_DEV_ARR + ci * 4u) >= 30000u)
            return L"兵·硬顶";
        return NULL;
    default:
        return NULL;
    }
}
static void deleg_note(unsigned int c, wchar_t *out)
{
    static const int KS[6] = { 0, 1, 2, 3, 4, 5 };
    int i, n = 0;
    out[0] = 0;
    for (i = 0; i < 6; i++) {
        const wchar_t *r;
        if (!deleg_will(c, KS[i])) continue;
        r = deleg_block_reason(c, KS[i]);
        if (!r) continue;
        if (n) lstrcpynW(out + lstrlenW(out), L" ", 4);
        lstrcpynW(out + lstrlenW(out), r, 24);
        n++;
    }
}

static unsigned int g_trn_num[TRAIN_LIST_MAX];
static int          g_trn_n = 0;

static int fac_cache_alive(void)
{
    int i, n = 0;
    for (i = 1; i <= (int)UNIT_IDX_MAX; i++) {
        if (rd32(CHAR_FAC_ARR + i * 4u) <= 31u) n++;
        if (n >= 5) return 1;
    }
    return 0;
}

static int gen_dst_ok(void)
{
    if (!fac_trusted()) {
        wsprintfW(g_pull_msg, L"势力状态异常，暂无法召集");
        logf_("[gen] 拒绝：玩家势力读数不可信");
        return 0;
    }
    if (g_pull_dst >= 1u && rd32(CITY_OWNER + (g_pull_dst - 1u) * 4u) != g_player_f) {
        wsprintfW(g_pull_msg, L"本城非我方所属，无法召集");
        logf_("[gen] 拒绝：dst=#%u 归属!=玩家(%u)", g_pull_dst, g_player_f);
        return 0;
    }
    return 1;
}

static void recompute_layout(void)
{
    float f = g_scale * g_scaleAdj;
    g_uiW = (UINT)(UI_W0 * f);
    g_uiH = (UINT)((HDR_H + g_rowsCur * ROW_H + PAD_H) * f);
    if (g_uiW < 120) g_uiW = 120;
    if (g_uiH < 80)  g_uiH = 80;
    g_posX = (int)(MARGIN * f);
    g_posY = (int)(MARGIN * f);
    g_dirty = 1;
}

static void release_dib(void)
{
    if (g_dib) { DeleteObject(g_dib); g_dib = NULL; g_bits = NULL; }
    g_dibW = g_dibH = 0;
}

static int ensure_dib(void)
{
    if (g_dib && g_dibW == g_uiW && g_dibH == g_uiH) return 1;
    release_dib();
    HDC dc = CreateCompatibleDC(NULL);
    BITMAPINFO bi; ZeroMemory(&bi, sizeof(bi));
    bi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth       = (LONG)g_uiW;
    bi.bmiHeader.biHeight      = -(LONG)g_uiH;
    bi.bmiHeader.biPlanes      = 1;
    bi.bmiHeader.biBitCount    = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    g_dib = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &g_bits, NULL, 0);
    DeleteDC(dc);
    if (g_dib) { g_dibW = g_uiW; g_dibH = g_uiH; }
    return g_dib != NULL;
}

static COLORREF mode_bg(int m)
{
    (void)m;
    return UI_COL_BG;
}
static COLORREF mode_edge(int m)
{
    if (m == MODE_CITY)    return UI_COL_EDGE_CITY;
    if (m == MODE_FACTION) return UI_COL_EDGE_FAC;
    return UI_COL_EDGE_MAP;
}
static COLORREF mode_title(int m)
{
    if (m == MODE_CITY)    return UI_COL_TITLE_CITY;
    if (m == MODE_FACTION) return UI_COL_TITLE_FAC;
    return UI_COL_TITLE_MAP;
}

static void office_demote_arm(int on)
{
    DWORD old;
    volatile unsigned char *p;
    unsigned char now;
    (void)on;

    if (!VirtualProtect((void *)OFFICE_DEMOTE_ADDR, 2, PAGE_EXECUTE_READWRITE, &old)) {
        logf_("[demote] VirtualProtect FAIL err=%lu", GetLastError());
        return;
    }
    p = (volatile unsigned char *)(UINT_PTR)OFFICE_DEMOTE_ADDR;
    now = *p;
    if (now != 0x7d && now != 0x74) {
        VirtualProtect((void *)OFFICE_DEMOTE_ADDR, 2, old, &old);
        logf_("[demote] SKIP byte@%08X=%02X", OFFICE_DEMOTE_ADDR, now);
        return;
    }
    *p = 0x74;
    VirtualProtect((void *)OFFICE_DEMOTE_ADDR, 2, old, &old);
    g_demote_on = 1;
    logf_("[demote] ON(je=仅同官排除) gate=%02X ——v105 机制焊死不可关", *p);
}

static int death_policy_apply(int state)
{

    static const unsigned char AB_T[3] = { 0x5A, 0x6A, 0x5A };
    static const unsigned char AJ_T[3] = { 0x7D, 0x7D, 0xEB };
    static const unsigned char BL_T[3] = { 0x0D, 0x12, 0x0D };
    static const unsigned char BJ_T[3][3] = { { 0x0F, 0x9D, 0xC0 },
                                             { 0x0F, 0x9D, 0xC0 },
                                             { 0x33, 0xC0, 0x90 } };

    static const unsigned char AB_K[3] = { 0x5A, 0x64, 0x6A };
    static const unsigned char AJ_K[2] = { 0x7D, 0xEB };
    static const unsigned char BL_K[3] = { 0x0D, 0x11, 0x12 };
    static const unsigned char BJ_K[2][3] = { { 0x0F, 0x9D, 0xC0 }, { 0x33, 0xC0, 0x90 } };
    unsigned char *pa = (unsigned char *)(UINT_PTR)DEATH_A_BASE_ADDR;
    unsigned char *pj = (unsigned char *)(UINT_PTR)DEATH_A_JUDGE_ADDR;
    unsigned char *pl = (unsigned char *)(UINT_PTR)DEATH_B_LVL_ADDR;
    unsigned char *pb = (unsigned char *)(UINT_PTR)DEATH_B_JUDGE_ADDR;
    DWORD o1, o2, o3, o4;
    int i, okA = 0, okJ = 0, okL = 0, okB = 0;

    if (state < 0 || state > 2) return 0;

    for (i = 0; i < 3; i++) if (pa[0] == AB_K[i]) okA = 1;
    for (i = 0; i < 2; i++) if (pj[0] == AJ_K[i]) okJ = 1;
    for (i = 0; i < 3; i++) if (pl[0] == BL_K[i]) okL = 1;
    for (i = 0; i < 2; i++) if (pb[0] == BJ_K[i][0] && pb[1] == BJ_K[i][1] && pb[2] == BJ_K[i][2]) okB = 1;
    if (!okA || !okJ || !okL || !okB) {
        logf_("[death] ⛔字节校验失败：A基数=%02X A判定=%02X B基=%02X B判定=%02X%02X%02X ⇒ 拒绝改动",
              pa[0], pj[0], pl[0], pb[0], pb[1], pb[2]);
        return 0;
    }
    if (!VirtualProtect((void *)DEATH_A_BASE_ADDR, 1, PAGE_EXECUTE_READWRITE, &o1)) return 0;
    if (!VirtualProtect((void *)DEATH_A_JUDGE_ADDR, 1, PAGE_EXECUTE_READWRITE, &o2)) {
        VirtualProtect((void *)DEATH_A_BASE_ADDR, 1, o1, &o1); return 0;
    }
    if (!VirtualProtect((void *)DEATH_B_LVL_ADDR, 1, PAGE_EXECUTE_READWRITE, &o3)) {
        VirtualProtect((void *)DEATH_A_JUDGE_ADDR, 1, o2, &o2);
        VirtualProtect((void *)DEATH_A_BASE_ADDR, 1, o1, &o1); return 0;
    }
    if (!VirtualProtect((void *)DEATH_B_JUDGE_ADDR, 3, PAGE_EXECUTE_READWRITE, &o4)) {
        VirtualProtect((void *)DEATH_B_LVL_ADDR, 1, o3, &o3);
        VirtualProtect((void *)DEATH_A_JUDGE_ADDR, 1, o2, &o2);
        VirtualProtect((void *)DEATH_A_BASE_ADDR, 1, o1, &o1); return 0;
    }
    pa[0] = AB_T[state];
    pj[0] = AJ_T[state];
    pl[0] = BL_T[state];
    pb[0] = BJ_T[state][0]; pb[1] = BJ_T[state][1]; pb[2] = BJ_T[state][2];
    VirtualProtect((void *)DEATH_B_JUDGE_ADDR, 3, o4, &o4);
    VirtualProtect((void *)DEATH_B_LVL_ADDR, 1, o3, &o3);
    VirtualProtect((void *)DEATH_A_JUDGE_ADDR, 1, o2, &o2);
    VirtualProtect((void *)DEATH_A_BASE_ADDR, 1, o1, &o1);
    g_death_state = state;
    logf_("[death] 战死策略 = %s（A基数=%02X A判定=%02X B基=%02X B判定=%02X%02X%02X；被俘路径始终未动）",
          death_state_name(state), pa[0], pj[0], pl[0], pb[0], pb[1], pb[2]);
    return 1;
}

typedef struct {
    unsigned int magic;
    int   death_state;
    int   cheat_on;
    int   ov_on;
    unsigned char deleg[CITY_MAXIDX + 2];
    unsigned short target[CITY_MAXIDX + 2][DELEG_ITEMS];
    unsigned char cfg[CITY_MAXIDX + 2];
} ZYSettings;
#define ZYSET_MAGIC 0x3253595Au
#define ZYSET_MAGIC_OLD 0x3153595Au

static void settings_path(char *out)
{
    char dir[MAX_PATH];
    out[0] = 0;
    if (!mod_dir(dir)) return;
    wsprintfA(out, "%s\\cd-rom\\zyconf", dir);
    CreateDirectoryA(out, NULL);
    if (GetFileAttributesA(out) == INVALID_FILE_ATTRIBUTES) {
        wsprintfA(out, "%s\\zyconf", dir);
        CreateDirectoryA(out, NULL);
        if (GetFileAttributesA(out) == INVALID_FILE_ATTRIBUTES) { out[0] = 0; return; }
    }
    wsprintfA(out + lstrlenA(out), "\\settings.dat");
}
static void settings_save(void)
{
    char path[MAX_PATH];
    ZYSettings s;
    HANDLE h; DWORD wr = 0;
    settings_path(path);
    if (!path[0]) { logf_("[set] ⛔无可用配置目录"); return; }
    s.magic = ZYSET_MAGIC;
    s.death_state = g_death_state;
    s.cheat_on = g_cheat_on;
    s.ov_on = g_ovOn;
    memcpy(s.deleg, g_deleg, sizeof(s.deleg));
    memcpy(s.target, g_deleg_target, sizeof(s.target));
    memcpy(s.cfg, g_deleg_cfg, sizeof(s.cfg));
    memcpy(s.cfg, g_deleg_cfg, sizeof(s.cfg));
    h = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) { logf_("[set] 打开失败 %s err=%lu", path, GetLastError()); return; }
    if (!WriteFile(h, &s, (DWORD)sizeof(s), &wr, NULL) || wr != (DWORD)sizeof(s))
        logf_("[set] 写入不完整 wr=%lu/%u err=%lu", wr, (unsigned)sizeof(s), GetLastError());
    CloseHandle(h);
}
static void settings_load(void)
{
    char path[MAX_PATH];
    static ZYSettings s;
    DWORD hdr_sz = 16u + (CITY_MAXIDX + 2u) +
                   (DWORD)sizeof(unsigned short) * (CITY_MAXIDX + 2u) * DELEG_ITEMS;
    HANDLE h; DWORD rd = 0;
    unsigned int c; int nd = 0, i, old_fmt = 0;
    settings_path(path);
    if (!path[0]) return;
    h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) { logf_("[set] 无配置文件（首次？），保持默认"); return; }
    if (!ReadFile(h, &s, (DWORD)sizeof(s), &rd, NULL)) {
        CloseHandle(h);
        logf_("[set] ⛔读取失败 rd=%lu ⇒ 忽略", rd);
        return;
    }
    CloseHandle(h);
    if (s.magic == ZYSET_MAGIC && rd == (DWORD)sizeof(s)) {
    } else if (s.magic == ZYSET_MAGIC_OLD && rd >= hdr_sz) {
        old_fmt = 1;
    } else {
        logf_("[set] ⛔配置校验失败 rd=%lu magic=%08X ⇒ 忽略", rd, s.magic);
        return;
    }
    if (s.death_state >= 0 && s.death_state <= 2 && s.death_state != g_death_state) {
        if (death_policy_apply(s.death_state)) nd |= 1;
        else logf_("[set] 战死策略恢复失败(字节校验)");
    }
    g_cheat_on = s.cheat_on ? 1 : 0;
    g_ovOn     = s.ov_on ? 1 : 0;
    for (c = 1u; c <= CITY_MAXIDX; c++) {
        g_deleg[c] = s.deleg[c] ? 1 : 0;
        g_deleg_cfg[c] = 0;
        for (i = 0; i < DELEG_ITEMS; i++) {
            unsigned int t = s.target[c][i];
            if (t > deleg_dcap(c, i)) t = deleg_dcap(c, i);
            g_deleg_target[c][i] = (unsigned short)t;
            g_deleg_cost[c][i]   = 0u;
            if (t) g_deleg_cfg[c] = 1;
        }
        if (!old_fmt && s.cfg[c]) g_deleg_cfg[c] = 1;
        if (g_deleg[c]) g_deleg_cfg[c] = 1;
        if (g_deleg[c]) nd |= 2;
    }
    logf_("[set] 已恢复：委任=%s 战死=%d 作弊=%d 覆盖=%d 格式=%s（%s）",
          (nd & 2) ? "有" : "无", s.death_state, g_cheat_on, g_ovOn,
          old_fmt ? "旧(推断cfg)" : "新", path);
    g_dirty = 1;
}

typedef void (__cdecl *REBUILDFN0)(void);
static int rebuild_syms_ok(unsigned int base)
{
    static const struct { unsigned int rva; const unsigned char sig[8]; int n; } T[] = {
        { RVA_REBUILD_ALL, { 0xE8, 0x6E, 0x04, 0x00, 0x00, 0xE8, 0x0A, 0x00 }, 8 },
        { RVA_INV_DESC,    { 0x8B, 0x44, 0x24, 0x04, 0x6B, 0xC0, 0x4C, 0x83 }, 8 },
        { RVA_CMD_TBL_RESET, { 0x68, 0xC0, 0x04, 0x00, 0x00, 0x6A, 0x00, 0x68 }, 8 },
    };
    int i, k;
    if (!base) return 0;
    for (i = 0; i < (int)(sizeof(T) / sizeof(T[0])); i++) {
        const unsigned char *p = (const unsigned char *)(size_t)(base + T[i].rva);
        for (k = 0; k < T[i].n; k++)
            if (p[k] != T[i].sig[k]) {
                logf_("[cache] ⛔签名校验失败 RVA=0x%X 第%d字节 期望%02X 实得%02X ⇒ 拒绝重建调用",
                      T[i].rva, k, T[i].sig[k], p[k]);
                return 0;
            }
    }
    return 1;
}

static int game_cache_refresh(void)
{
    unsigned int base = (unsigned int)(size_t)GetModuleHandleA(NULL);
    if (!base || !rebuild_syms_ok(base)) return 0;
    ((REBUILDFN0)(size_t)(base + RVA_REBUILD_ALL))();
    return 1;
}

typedef int  (__cdecl *TRAINFN_ABILITY)(unsigned int);
typedef unsigned short (__cdecl *TRAINFN_RAND)(void);
typedef void (__cdecl *TRAINFN_ADD2)(unsigned int, unsigned int);
typedef void (__cdecl *TRAINFN_ADD1)(unsigned int);

static int train_syms_ok(unsigned int base)
{
    static const struct { unsigned int rva; const unsigned char sig[8]; int n; } T[] = {
        { RVA_ADD_TRAIN,     { 0x8B, 0x44, 0x24, 0x04 }, 4 },
        { RVA_TRAIN_ABILITY, { 0x56, 0x8B, 0x74, 0x24, 0x08 }, 5 },
        { RVA_GAME_RAND,     { 0x66, 0x83, 0x3D, 0xC4, 0x3F, 0x5E, 0x00, 0x00 }, 8 },
        { RVA_ADD_EXP,       { 0x8B, 0x44, 0x24, 0x04 }, 4 },
        { RVA_REFRESH_POWER, { 0x55, 0x8B, 0xEC, 0x51, 0x8B, 0x45, 0x08 }, 7 },

        { RVA_TROOP_CLAMP,   { 0x56, 0x8B, 0x74, 0x24, 0x08 }, 5 },

        { RVA_OFFICE_CAP,    { 0x8B, 0x44, 0x24, 0x04, 0x8B, 0xC8, 0xC1, 0xE1 }, 8 },
        { RVA_DEV_FARM,      { 0x8B, 0x44, 0x24, 0x04, 0x6A, 0x01, 0x59, 0x3B }, 8 },
        { RVA_DEV_COMM,      { 0x8B, 0x44, 0x24, 0x04, 0x6A, 0x01, 0x5A, 0x3B }, 8 },
        { RVA_DEV_ORDER,     { 0x8B, 0x44, 0x24, 0x04, 0x83, 0xF8, 0x01, 0x7C }, 8 },
        { RVA_DEV_WALL,      { 0x8B, 0x44, 0x24, 0x04, 0x83, 0xF8, 0x01, 0x7C }, 8 },
        { RVA_DEV_FOOD,      { 0x8B, 0x44, 0x24, 0x04, 0x83, 0xF8, 0x01, 0x7C }, 8 },
        { RVA_CMD_ORDER,     { 0x55, 0x8B, 0xEC, 0x51, 0x53, 0x56, 0x57, 0x8B }, 8 },
        { RVA_CMD_FOOD,      { 0x51, 0x55, 0x8B, 0x6C, 0x24, 0x0C, 0x56, 0x8B }, 8 },
        { RVA_CMD_WALL,      { 0x55, 0x8B, 0xEC, 0x51, 0x53, 0x8B, 0x5D, 0x08 }, 8 },
        { RVA_CMD_TROOP,     { 0x55, 0x8B, 0xEC, 0x51, 0x51, 0x8B, 0x45, 0x08 }, 8 },
    };
    int i, k;
    if (!base) return 0;
    for (i = 0; i < (int)(sizeof(T) / sizeof(T[0])); i++) {
        const unsigned char *p = (const unsigned char *)(size_t)(base + T[i].rva);
        for (k = 0; k < T[i].n; k++)
            if (p[k] != T[i].sig[k]) {
                logf_("[sym] ⛔签名校验失败 RVA=0x%X 第%d字节 期望%02X 实得%02X ⇒ 拒绝调用",
                      T[i].rva, k, T[i].sig[k], p[k]);
                return 0;
            }
    }
    return 1;
}

static void get_char(unsigned int idx, unsigned int out[CHAR_N])
{
    int i;
    unsigned int a = RUNTIME_BASE + idx * 4u;
    for (i = 0; i < CHAR_N; i++) out[i] = rd32(a + FOFF[i]);
}

static void get_city(unsigned int idx, unsigned int out[CITY_N])
{
    int i;
    for (i = 0; i < CITY_N; i++) {
        unsigned int v = rd32(CITY_ARR[i] + idx * 4u);
        out[i] = (v <= 999999u) ? v : 0u;
    }
}

static void get_faction(unsigned int idx, unsigned int out[FACTION_N])
{
    int i;
    for (i = 0; i < FACTION_N; i++) {
        unsigned int v = rd32(FARR[i] + idx * 4u);
        out[i] = (v <= 99999999u) ? v : 0u;
    }
}

static void city_name_copy(unsigned int idx, wchar_t *out, int cap)
{
    const wchar_t *s = city_name_of(idx);
    if (s) lstrcpynW(out, s, cap);
    else   wsprintfW(out, L"城#%u", idx);
}

static void pull_sort(void)
{
    unsigned int i;
    int j;
    g_pull_n = 0;
    g_player_f = player_fac();
    if (!fac_trusted()) {
        logf_("[pull] 玩家势力读数不可信(rd=%u) ⇒ 拒绝列兵", rd32(PLAYER_FAC));
        wsprintfW(g_pull_msg, L"势力状态异常，暂无法召集");
        g_dirty = 1;
        return;
    }
    for (i = 1; i <= CITY_MAXIDX; i++) {
        unsigned int v;
        if (i == g_pull_dst) continue;

        if (rd32(CITY_OWNER + (i - 1u) * 4u) != g_player_f) continue;
        v = rd32(TROOP_ARR + i * 4u);

        if (v <= 1000u || v > 999999u) continue;
        if (g_pull_n < 148) {
            g_pull_city[g_pull_n] = i;
            g_pull_amt[g_pull_n]  = v;
            city_name_copy(i, g_pull_name[g_pull_n], 16);
            g_pull_n++;
        }
    }

    for (j = 1; j < g_pull_n; j++) {
        unsigned int c = g_pull_city[j], a = g_pull_amt[j];
        wchar_t nm[16];
        int k = j - 1;
        lstrcpynW(nm, g_pull_name[j], 16);
        while (k >= 0 && g_pull_amt[k] < a) {
            g_pull_city[k + 1] = g_pull_city[k];
            g_pull_amt[k + 1]  = g_pull_amt[k];
            lstrcpynW(g_pull_name[k + 1], g_pull_name[k], 16);
            k--;
        }
        g_pull_city[k + 1] = c; g_pull_amt[k + 1] = a;
        lstrcpynW(g_pull_name[k + 1], nm, 16);
    }
    if (g_pull_cur >= g_pull_n) g_pull_cur = (g_pull_n > 0) ? g_pull_n - 1 : 0;
    if (g_pull_cur < 0) g_pull_cur = 0;
    if (g_pull_top > g_pull_n - PULL_VIS) g_pull_top = (g_pull_n > PULL_VIS) ? g_pull_n - PULL_VIS : 0;
    if (g_pull_top < 0) g_pull_top = 0;
}

static int pull_cost_cmd(int n )
{
    unsigned int pf, left;
    if (n <= 0) return 1;
    pf = player_fac();
    if (pf == 0xFFFFFFFFu) { wsprintfW(g_pull_msg, L"势力状态异常，暂无法召集"); return 0; }
    left = rd32(CMD_POINT_ARR + pf * 4u);
    if (left < 1u) { wsprintfW(g_pull_msg, L"命令点不足，无法召集"); g_dirty = 1; return 0; }
    wr32(CMD_POINT_ARR + pf * 4u, left - 1u);
    logf_("[pull] 扣令牌 1（召集批 %d 项），势力%u 剩余%u", n, pf, left - 1u);
    return 1;
}

static int pull_cmd_have(void)
{
    unsigned int pf = player_fac();
    if (pf == 0xFFFFFFFFu) { wsprintfW(g_pull_msg, L"势力状态异常，暂无法召集"); g_dirty = 1; return 0; }
    if (rd32(CMD_POINT_ARR + pf * 4u) < 1u) {
        wsprintfW(g_pull_msg, L"命令点不足，无法召集"); g_dirty = 1; return 0;
    }
    return 1;
}

static void pull_take(int cur, int amount);
static void pull_enter(unsigned int city_num);

static void pull_dump(int cur)
{
    unsigned int vs;
    int amt;
    if (cur < 0 || cur >= g_pull_n) return;

    if (!fac_trusted() || (g_pull_dst >= 1u &&
        rd32(CITY_OWNER + (g_pull_dst - 1u) * 4u) != g_player_f)) {
        wsprintfW(g_pull_msg, fac_trusted() ? L"本城非我方所属，无法召集" : L"势力状态异常，暂无法召集");
        logf_("[pull] 拒绝：读数可信=%d dst=#%u pf=%u", fac_trusted(), g_pull_dst, g_player_f);
        g_dirty = 1;
        return;
    }
    vs = rd32(TROOP_ARR + g_pull_city[cur] * 4u);
    if (vs > 999999u) { logf_("[pull] 拒绝：源城读数异常 %u", vs); return; }
    amt = (vs > 1000u) ? (int)(vs - 1000u) : 0;
    if (amt <= 0) {
        wsprintfW(g_pull_msg, L"%s 兵力 %u，未超出留驻额 1000，无兵可转", g_pull_name[cur], vs);
        logf_("[pull] %s 兵 %u 不足超额 ⇒ 无事", l10(g_pull_name[cur]), vs);
        g_dirty = 1;
        return;
    }
    g_pull_msg[0] = 0;
    if (!pull_cost_cmd(1)) { g_dirty = 1; return; }
    pull_take(cur, amt);
}

static void pull_all(void)
{
    int done = 0;
    unsigned int total = 0, rounds;
    if (!fac_trusted() || (g_pull_dst >= 1u &&
        rd32(CITY_OWNER + (g_pull_dst - 1u) * 4u) != g_player_f)) {
        wsprintfW(g_pull_msg, fac_trusted() ? L"本城非我方所属，无法召集" : L"势力状态异常，暂无法召集");
        logf_("[pull] 一键全转拒绝：读数可信=%d dst=#%u pf=%u", fac_trusted(), g_pull_dst, g_player_f);
        g_dirty = 1; return;
    }
    if (g_pull_n == 0) { wsprintfW(g_pull_msg, L"没有可供调拨的城池"); g_dirty = 1; return; }
    if (!pull_cmd_have()) { g_dirty = 1; return; }
    for (rounds = 0; rounds < 210u; rounds++) {
        unsigned int vs, amt, src0;
        int oldn;
        if (g_pull_n <= 0) break;
        src0 = g_pull_city[0]; oldn = g_pull_n;
        vs = rd32(TROOP_ARR + src0 * 4u);
        if (vs <= 1000u || vs > 999999u) break;
        amt = vs - 1000u;
        pull_take(0, (int)amt);

        if (g_pull_n == oldn && (g_pull_n == 0 || g_pull_city[0] == src0)) break;
        done++; total += amt;
    }
    if (done > 0) pull_cost_cmd(done);
    wsprintfW(g_pull_msg, L"调拨完成：%d 座城池，共转入 %u 兵", done, total);
    logf_("[pull] 一键全转：%d 城共 %u 兵 -> 本城#%u", done, total, g_pull_dst);
    g_dirty = 1;
}

static void pull_general_sort(void)
{
    int i, j;
    g_gen_n = 0;
    g_player_f = player_fac();
    if (!fac_trusted()) {
        logf_("[gen] 玩家势力读数不可信(rd=%u) ⇒ 拒绝列将", rd32(PLAYER_FAC));
        wsprintfW(g_pull_msg, L"势力状态异常，暂无法召集");
        g_dirty = 1;
        return;
    }
    for (i = 1; i <= (int)UNIT_IDX_MAX; i++) {
        int loc = (int)rd32(CHAR_LOC_ARR + i * 4u);
        unsigned int c;
        if (loc < 0) continue;
        c = (unsigned int)loc + 1u;
        if (c > CITY_MAXIDX) continue;
        if (c == g_pull_dst) continue;

        if (rd32(CITY_OWNER + (c - 1u) * 4u) != g_player_f) continue;
        if (fac_cache_alive()) {
            if (rd32(CHAR_FAC_ARR + i * 4u) != g_player_f) continue;
        } else if (rd32(CHAR_OFFICE_ARR + i * 4u) == 0u) {
            continue;
        }
        if (g_gen_n < 866) { g_gen_num[g_gen_n] = i; g_gen_city[g_gen_n] = c; g_gen_n++; }
    }

    for (j = 1; j < g_gen_n; j++) {
        unsigned int n = g_gen_num[j], ct = g_gen_city[j]; int k = j - 1;
        while (k >= 0 && g_gen_city[k] > ct) {
            g_gen_num[k + 1] = g_gen_num[k]; g_gen_city[k + 1] = g_gen_city[k]; k--;
        }
        g_gen_num[k + 1] = n; g_gen_city[k + 1] = ct;
    }
    if (g_pull_cur >= g_gen_n) g_pull_cur = (g_gen_n > 0) ? g_gen_n - 1 : 0;
    if (g_pull_cur < 0) g_pull_cur = 0;
    if (g_pull_top > g_gen_n - PULL_VIS) g_pull_top = (g_gen_n > PULL_VIS) ? g_gen_n - PULL_VIS : 0;
    if (g_pull_top < 0) g_pull_top = 0;
}

static void pull_general_enter(unsigned int city_num)
{
    if (city_num < 1u || city_num > CITY_MAXIDX) return;
    g_pull_dst = city_num;
    city_name_copy(city_num, g_pull_dstname, 24);
    g_pull_cur = 0; g_pull_top = 0; g_pull_msg[0] = 0;
    g_player_f = player_fac();
    pull_general_sort();
    g_pull_mode = PANEL_GENERAL; g_dirty = 1;
    logf_("[gen] 进入武将召集：本城=%s(#%u) 候选%d将", l10(g_pull_dstname), city_num, g_gen_n);
}
static void pull_general_take(int cur)
{
    const wchar_t *gn;
    unsigned int num, oldc;
    if (cur < 0 || cur >= g_gen_n) return;
    if (!gen_dst_ok()) { g_dirty = 1; return; }
    if (!pull_cost_cmd(1)) { g_dirty = 1; return; }
    num = g_gen_num[cur];
    oldc = (unsigned int)((int)rd32(CHAR_LOC_ARR + num * 4u)) + 1u;
    wr32(CHAR_LOC_ARR + num * 4u, g_pull_dst - 1u);
    g_pull_ops++;
    gn = name_of(num);
    logf_("[gen] 召集 %s(#%u) 城 %u->%u", l10(gn), num, oldc, g_pull_dst);
    wsprintfW(g_pull_msg, L"已召集 %s", gn ? gn : L"?");
    pull_general_sort();
    game_cache_refresh();
    g_dirty = 1;
}
static void pull_general_all(void)
{
    int i, done = 0;
    g_player_f = player_fac();
    if (!gen_dst_ok()) { g_dirty = 1; return; }
    if (!pull_cmd_have()) { g_dirty = 1; return; }
    for (i = 1; i <= (int)UNIT_IDX_MAX; i++) {
        int loc = (int)rd32(CHAR_LOC_ARR + i * 4u);
        unsigned int c;
        if (loc < 0) continue;
        c = (unsigned int)loc + 1u;
        if (c > CITY_MAXIDX || c == g_pull_dst) continue;

        if (rd32(CITY_OWNER + (c - 1u) * 4u) != g_player_f) continue;
        if (fac_cache_alive()) {
            if (rd32(CHAR_FAC_ARR + i * 4u) != g_player_f) continue;
        } else if (rd32(CHAR_OFFICE_ARR + i * 4u) == 0u) {
            continue;
        }
        wr32(CHAR_LOC_ARR + i * 4u, g_pull_dst - 1u);
        done++; g_pull_ops++;
    }
    pull_general_sort();
    if (done > 0) pull_cost_cmd(done);
    if (done > 0) game_cache_refresh();
    wsprintfW(g_pull_msg, L"一键召集完成：%d 将", done);
    logf_("[gen] 一键召集 %d 将 -> 本城#%u", done, g_pull_dst);
    g_dirty = 1;
}

static void g_pull_pick(int idx)
{
    if (idx == 0) {
        g_pull_msg[0] = 0;
        pull_enter(g_pull_dst);
    } else {
        g_pull_msg[0] = 0;
        pull_general_enter(g_pull_dst);
    }
}

static void pull_take(int cur, int amount)
{
    unsigned int src, dst, vs, vd, amt;
    unsigned int as_, ad_;
    int new_s, new_d;

    if (cur < 0 || cur >= g_pull_n) return;
    src = g_pull_city[cur];
    dst = g_pull_dst;
    if (src > CITY_MAXIDX || dst > CITY_MAXIDX || src == dst) {
        logf_("[pull] 拒绝：编号非法 src=%u dst=%u", src, dst);
        return;
    }
    vs = rd32(TROOP_ARR + src * 4u);
    vd = rd32(TROOP_ARR + dst * 4u);
    if (vs > 999999u || vd > 999999u) {
        logf_("[pull] 拒绝：值异常 src=%u dst=%u vs=%u vd=%u", src, dst, vs, vd);
        return;
    }
    if (amount > 0) {
        amt  = (unsigned int)amount;
        if (amt > vs) amt = vs;
        if (amt == 0u) return;
        new_s = (int)(vs - amt);
        new_d = (int)(vd + amt); if (new_d > 999999) new_d = 999999;
    } else {
        amt = (unsigned int)(-amount);
        if (amt > vd) amt = vd;
        if (amt == 0u) return;
        new_s = (int)(vs + amt); if (new_s > 999999) new_s = 999999;
        new_d = (int)(vd - amt);
    }
    wr32(TROOP_ARR + src * 4u, (unsigned int)new_s);
    wr32(TROOP_ARR + dst * 4u, (unsigned int)new_d);
    as_ = rd32(TROOP_ARR + src * 4u);
    ad_ = rd32(TROOP_ARR + dst * 4u);
    g_pull_ops++;
    g_pull_msg[0] = 0;
    logf_("[pull] #%ld %s(%u) %u->%u ; 本城(%u) %u->%u 步长=%d 校验=%s",
          g_pull_ops, l10(g_pull_name[cur]), src, vs, as_, dst, vd, ad_, amount,
          (as_ + ad_ == vs + vd) ? "守恒OK" : "!!不守恒!!");
    pull_sort();
    game_cache_refresh();
    g_dirty = 1;
}

static void pull_menu_open(unsigned int city_num)
{
    if (city_num == 0u || city_num > CITY_MAXIDX) {
        logf_("[pull] 召集：非城市页或编号越界 num=%u", city_num);
        return;
    }
    g_pull_dst = city_num;
    city_name_copy(city_num, g_pull_dstname, 24);
    g_pull_cur = 0;
    g_pull_msg[0] = 0;
    g_pull_mode = PANEL_MAIN;
    g_dirty = 1;
    logf_("[pull] 进入召集主菜单：本城=%s(#%u)", l10(g_pull_dstname), city_num);
}

static void pull_enter(unsigned int city_num)
{
    if (city_num == 0u || city_num > CITY_MAXIDX) {
        logf_("[pull] 非城市详情页或编号越界 num=%u ⇒ 不进入调兵模式", city_num);
        return;
    }
    g_pull_dst = city_num;
    city_name_copy(city_num, g_pull_dstname, 24);
    g_pull_cur = 0; g_pull_top = 0;
    pull_sort();
    g_pull_mode = PANEL_SOLDIER;
    g_dirty = 1;
    logf_("[pull] 进入调兵模式：本城=%s(#%u) 候选 %d 城", l10(g_pull_dstname), city_num, g_pull_n);
}

static void apply_field(int field, int delta)
{
    unsigned int num = 0;
    int mode = detect_mode(&num);
    int rows = mode_rows(mode);

    if (mode == MODE_NONE) return;

    if (!g_cheat_on) return;
    if (field < 0) field = g_cur;
    if (field < 0 || field >= rows) return;

    if (g_guard_on && !ctx_writable(mode, num)) {
        g_guard_blocked++;
        logf_("[guard] 拒绝操作：当前页面不可改 mode=%d num=%u pg=%u（累计拒绝 %ld）",
              mode, num, rd32(PAGE_ID), g_guard_blocked);
        return;
    }

    if (mode == MODE_CITY) {
        if (num > 200u) return;
        if (field <= 2 && city_is_gate(num)) {
            logf_("[guard] 拒绝：关卡#%u 无%s数据，不可修改", num, l10(CNAME[field]));
            return;
        }
        unsigned int a = CITY_ARR[field] + num * 4u;
        unsigned int cap = deleg_dcap(num, field);
        if (delta == 0) {
            wr32g(a, cap, mode, num, field);
            if (field == 0) wr32g(CITY_FARM2 + num * 4u, cap, mode, num, 0);
        } else {
            unsigned int cur = rd32(a);
            unsigned int step = CSTEP[field];
            unsigned int nv = (delta > 0) ? (cur + step) : ((cur > step) ? cur - step : 0u);
            if (nv > cap) nv = cap;
            wr32g(a, nv, mode, num, field);
            if (field == 0) {
                wr32g(CITY_FARM2 + num * 4u, rd32(a), mode, num, 0);
            }
        }
    } else if (mode == MODE_FACTION) {
        if (num > 10u) return;
        unsigned int a = FARR[field] + num * 4u;
        unsigned int cap = CAP_FAC[field];
        if (delta == 0) {
            wr32g(a, cap, mode, num, field);
        } else {
            unsigned int cur = rd32(a);
            unsigned int step = FSTEP[field];
            unsigned int nv = (delta > 0) ? (cur + step) : ((cur > step) ? cur - step : 0u);
            if (nv > cap) nv = cap;
            wr32g(a, nv, mode, num, field);
        }
    } else {
        if (!char_idx_valid(num)) return;
        unsigned int a = RUNTIME_BASE + num * 4u + FOFF[field];
        unsigned int cap = CAP_CHAR[field];
        if (delta == 0) {
            wr32g(a, cap, mode, num, field);
        } else {
            unsigned int cur = rd32(a);
            unsigned int nv;
            if (delta > 0) {
                nv = cur + (unsigned int)delta;
                if (nv > cap) nv = cap;
            } else {
                nv = (cur >= (unsigned int)(-delta)) ? cur + delta : 0u;
            }
            wr32g(a, nv, mode, num, field);
        }
    }
    g_dirty = 1;
}

static void do_refresh_action(int mode, unsigned int num)
{
    unsigned int i, n = ACTED_N, hit = 0;

    if (!ACTED_FLAG) {
        logf_("[refresh] ⛔未定位「已行动」标志数组（ACTED_FLAG=0）⇒ 未写任何内存。"
              "请参考 docs/addresses.json 的 acted_flag 重新定位。");
        return;
    }

    if (ACTED_FLAG < 0x004B0000u || ACTED_FLAG > 0x00640000u) {
        logf_("[refresh] ⛔ACTED_FLAG=0x%08X 超出数据区 ⇒ 拒绝写", ACTED_FLAG);
        return;
    }

    if (mode == MODE_CHAR && num < n) {
        unsigned int a = ACTED_FLAG + num * (unsigned int)ACTED_GRAN;
        unsigned int old = (ACTED_GRAN == 4) ? rd32(a) : (unsigned int)*(volatile unsigned char *)a;
        if (old) {
            if (ACTED_GRAN == 4) *(volatile unsigned int *)a = 0u;
            else *(volatile unsigned char *)a = 0;
            hit = 1;
        }
        logf_("[refresh] 武将 %u：标记 %u -> 0 %s", num, old,
              hit ? "✓已刷新，可再行动" : "(本来就是 0)");
        return;
    }

    for (i = 0; i < n; i++) {
        unsigned int a = ACTED_FLAG + i * (unsigned int)ACTED_GRAN;
        if (ACTED_GRAN == 4) {
            if (rd32(a)) { *(volatile unsigned int *)a = 0u; hit++; }
        } else {
            if (*(volatile unsigned char *)a) { *(volatile unsigned char *)a = 0; hit++; }
        }
    }
    logf_("[refresh] 非武将页 ⇒ 全员清标记：%u 个武将已恢复可行动（数组 0x%08X 长度 %u）",
          hit, ACTED_FLAG, n);
}

static void open_convocation(int mode, unsigned int num)
{
    if (mode == MODE_CITY) { pull_menu_open(num); }
    else {
        unsigned int m = rd32(CITY_MIRROR);
        if (m >= 1u && m <= CITY_MAXIDX) {
            logf_("[pull] 非城市页(mode=%d pg=%u) ⇒ 按镜像城市 #%u 进召集", mode, rd32(PAGE_ID), m + 1u);
            pull_menu_open(m + 1u);
        } else {
            logf_("[pull] 无法确定城市（mode=%d mir=%u）⇒ 先点一座本城再用", mode, m);
        }
    }
}

static int game_train_one(unsigned int idx)
{
    unsigned int base = (unsigned int)(size_t)GetModuleHandleA(NULL);
    unsigned int train, r, delta;
    int ability;
    if (!base) return 0;
    if (!train_syms_ok(base)) return 0;
    if (idx < 1u || idx > 403u) return 0;
    train = rd32(TRAIN_ARR + idx * 4u);
    if (train == 0u) return 0;
    if (train >= TRAIN_MAX) return 0;

    ability = ((TRAINFN_ABILITY)(size_t)(base + RVA_TRAIN_ABILITY))(idx);
    r = (unsigned int)((TRAINFN_RAND)(size_t)(base + RVA_GAME_RAND))();

    delta = (unsigned int)((((long)((r % 41u) + 80u) * (long)ability * 30L)
                            / (long)train) / 100L);
    ((TRAINFN_ADD2)(size_t)(base + RVA_ADD_TRAIN))(idx, delta);
    ((TRAINFN_ADD2)(size_t)(base + RVA_ADD_EXP))(idx, 1u);
    *(volatile unsigned int *)(size_t)(ACTED_FLAG + idx * 4u) |= 1u;
    ((TRAINFN_ADD1)(size_t)(base + RVA_REFRESH_POWER))(idx);
    return 1;
}

static void batch_train_city(void)
{
    unsigned int pf  = rd32(PLAYER_FAC);
    unsigned int mir = rd32(CITY_MIRROR);
    int i, cand = 0, done = 0, skipActed = 0, skipZero = 0, skipFull = 0, skipRange = 0, left;

    if (pf < 1u || pf > 11u) {
        wsprintfW(g_pull_msg, L"尚未确定我方势力");
        g_dirty = 1; logf_("[train] 批量训练中止：pf=%u 无效", pf); return;
    }
    if (mir > CITY_MAXIDX) {
        wsprintfW(g_pull_msg, L"当前不在城池界面");
        g_dirty = 1; logf_("[train] 批量训练中止：mir=%u 越界", mir); return;
    }
    if (rd32(CITY_OWNER + mir * 4u) != pf) {
        wsprintfW(g_pull_msg, L"本城非我方所属");
        g_dirty = 1; logf_("[train] 批量训练中止：#%u 归属=%u 非我方(pf=%u)",
                           mir + 1u, rd32(CITY_OWNER + mir * 4u), pf); return;
    }

    left = (int)rd32(CMD_POINT_ARR + pf * 4u);
    for (i = 1; i <= (int)UNIT_IDX_MAX; i++) {
        if (rd32(CHAR_LOC_ARR + i * 4u) != mir) continue;
        if (rd32(CHAR_FAC_ARR + i * 4u) != pf) continue;
        cand++;
        if (rd32(ACTED_FLAG + i * 4u) & 1u) { skipActed++; continue; }
        if (left <= 0) break;
        if (i > 403) { skipRange++; continue; }
        if (rd32(TRAIN_ARR + i * 4u) == 0u) { skipZero++; continue; }
        if (rd32(TRAIN_ARR + i * 4u) >= TRAIN_MAX) { skipFull++; continue; }
        if (game_train_one((unsigned int)i)) {
            done++; left--;
            wr32(CMD_POINT_ARR + pf * 4u, (unsigned int)left);
        } else {
            skipRange++;
        }
    }

    if (done > 0) wsprintfW(g_pull_msg, L"已训练 %d 人 · 命令点余 %d", done, left);
    else if (cand == 0)      wsprintfW(g_pull_msg, L"本城无我方武将");
    else if (skipActed == cand) wsprintfW(g_pull_msg, L"本城武将均已完成行动");
    else if (left <= 0)      wsprintfW(g_pull_msg, L"命令点不足");
    else                     wsprintfW(g_pull_msg, L"无可训练武将");
    g_dirty = 1;
    logf_("[train] 批量训练 城#%u：候选%d 实训%d 已行动跳过%d 无兵跳过%d 满训跳过%d 超范围跳过%d 命令点剩%d",
          mir + 1u, cand, done, skipActed, skipZero, skipFull, skipRange, left);
    g_pull_ops += done;
}

static void cheat_full_army(int do_troops, int do_train)
{
    unsigned int base = (unsigned int)(size_t)GetModuleHandleA(NULL);
    unsigned int pf, i, capMin = 0xFFFFFFFFu, capMax = 0u, sumCap = 0u;
    int n = 0, skipBad = 0, nKeep = 0;

    pf = rd32(PLAYER_FAC);
    if (pf < 1u || pf > 11u) {
        wsprintfW(g_pull_msg, L"尚未确定我方势力，未作修改");
        g_dirty = 1;
        logf_("[cheat] 中止：玩家势力读数=%u（应为 1..11）⇒ 未写任何内存", pf);
        return;
    }
    if (!base || !train_syms_ok(base)) {
        wsprintfW(g_pull_msg, L"函数签名不符，未改动");
        g_dirty = 1;
        return;
    }
    for (i = 1u; i <= UNIT_IDX_MAX; i++) {
        unsigned int cap, cur;
        if (rd32(CHAR_FAC_ARR + i * 4u) != pf) continue;
        if (rd32(CHAR_LOC_ARR + i * 4u) == 0xFFFFFFFFu) continue;
        cap = (unsigned int)((TRAINFN_ABILITY)(size_t)(base + RVA_OFFICE_CAP))(i);
        if (cap < 500u || cap > 9999u) { skipBad++; continue; }

        if (do_troops) {
            cur = rd32(CHAR_TROOP_ARR + i * 4u);
            if (cur > cap) { cap = cur; nKeep++; }
            wr32(CHAR_TROOP_ARR + i * 4u, cap);
        }
        if (do_train) {
            unsigned int tr = rd32(TRAIN_ARR + i * 4u);
            if (tr < TRAIN_MAX) wr32(TRAIN_ARR + i * 4u, TRAIN_MAX);
        }
        if (do_troops)
            ((TRAINFN_ADD1)(size_t)(base + RVA_TROOP_CLAMP))(i);
        ((TRAINFN_ADD1)(size_t)(base + RVA_REFRESH_POWER))(i);
        if (cap < capMin) capMin = cap;
        if (cap > capMax) capMax = cap;
        sumCap += cap;
        n++;
    }
    if (n == 0) wsprintfW(g_pull_msg, L"无可修改的我方武将（跳过 %d）", skipBad);
    else if (do_troops && do_train) wsprintfW(g_pull_msg, L"已将 %d 名我方武将兵力与训练度调满（兵力 %u~%u · 训练度 99）", n, capMin, capMax);
    else if (do_troops)             wsprintfW(g_pull_msg, L"已将 %d 名我方武将兵力调满（兵力 %u~%u）", n, capMin, capMax);
    else                            wsprintfW(g_pull_msg, L"已将 %d 名我方武将训练度调满（训练度 99）", n);
    g_dirty = 1;
    logf_("[cheat] 己方武将满兵员/满训练度：势力%u 改动%u将（其中%u将原有兵已超官职上限·保持不变）跳过%u 兵%u~%u 合计%u 兵数组0x%X 练数组0x%X（未扣命令点）",
          pf, n, nKeep, skipBad, (n ? capMin : 0u), capMax, sumCap, CHAR_TROOP_ARR, TRAIN_ARR);
}

static void train_list_build(void)
{
    unsigned int pf  = rd32(PLAYER_FAC);
    unsigned int mir = (g_pull_dst >= 1u) ? (g_pull_dst - 1u) : rd32(CITY_MIRROR);
    int i;
    g_trn_n = 0;
    if (pf >= 1u && pf <= 11u) {
        for (i = 1; i <= (int)UNIT_IDX_MAX && g_trn_n < TRAIN_LIST_MAX; i++) {
            if (rd32(CHAR_LOC_ARR + i * 4u) != mir) continue;
            if (rd32(CHAR_FAC_ARR + i * 4u) != pf) continue;
            if (rd32(TRAIN_ARR + i * 4u) >= TRAIN_MAX) continue;
            g_trn_num[g_trn_n++] = (unsigned int)i;
        }
    }
    g_pull_cur = 0; g_pull_top = 0; g_pull_msg[0] = 0; g_dirty = 1;
    if (g_trn_n == 0)
        wsprintfW(g_pull_msg, L"本城无可训练武将（训练度已满者不计）");
    logf_("[train] 训练列表建表：#%u 候选%d将（pf=%u）", mir + 1u, g_trn_n, pf);
}

static void train_pick(int cur)
{
    unsigned int pf, idx;
    const wchar_t *nm;
    int left;
    if (cur < 0 || cur >= g_trn_n) return;
    idx = g_trn_num[cur];
    pf  = rd32(PLAYER_FAC);
    nm  = name_of(idx);
    if (!nm) nm = L"?";
    if (idx < 1u || idx > 403u) {
        wsprintfW(g_pull_msg, L"%s 无法训练（索引超出范围）", nm); g_dirty = 1; return;
    }
    if (rd32(ACTED_FLAG + idx * 4u) & 1u) {
        wsprintfW(g_pull_msg, L"%s 本季已行动", nm); g_dirty = 1; return;
    }
    if (rd32(TRAIN_ARR + idx * 4u) >= TRAIN_MAX) {
        wsprintfW(g_pull_msg, L"%s 训练度已满，无法训练", nm); g_dirty = 1; return;
    }
    if (pf < 1u || pf > 11u) return;
    left = (int)rd32(CMD_POINT_ARR + pf * 4u);
    if (left <= 0) { wsprintfW(g_pull_msg, L"命令点不足"); g_dirty = 1; return; }
    if (!game_train_one(idx)) {
        wsprintfW(g_pull_msg, L"%s 无法训练", nm); g_dirty = 1; return;
    }
    wr32(CMD_POINT_ARR + pf * 4u, (unsigned int)(left - 1));
    wsprintfW(g_pull_msg, L"%s 训练度 %u · 命令点余 %u", nm,
              rd32(TRAIN_ARR + idx * 4u), (unsigned int)(left - 1));
    logf_("[train] 训练 #%u -> 练度%u  命令点剩%d（cmd_pt 写回）",
          idx, rd32(TRAIN_ARR + idx * 4u), left - 1);
    g_pull_ops++;
    g_dirty = 1;
}

static int detect_scene(int mode, unsigned int num,
                        unsigned int pg, unsigned int mir, unsigned int pf,
                        unsigned int ctrl, unsigned int *tcOut)
{
    *tcOut = 0xFFFFFFFFu;
    if (pf < 1u || pf > 11u) return SCENE_TITLE;
    if (mode == MODE_CITY) {
        if (num >= 1u && num <= CITY_MAXIDX) { *tcOut = num - 1u; return SCENE_CITY_DETAIL; }
        return SCENE_OTHER;
    }
    if (mode == MODE_CHAR)    return SCENE_CHAR;
    if (mode == MODE_FACTION) return SCENE_FACTION;

    if (pg >= 1u && pg <= 8u && mir >= 1u && mir <= CITY_MAXIDX
            && ctrl >= MENU_CTRL_LO && ctrl < MENU_CTRL_HI) {

        *tcOut = mir; return SCENE_CITY_CMD;
    }
    return SCENE_OTHER;
}

static int scene_is_convocation(int sc) { return sc == SCENE_CITY_CMD; }

typedef struct {
    const wchar_t *label;
    int  (*state)(void);
    void (*fire)(int mode, unsigned int num);
} MechDesc;

static int mech_death_state(void) { return g_death_state == 2; }
static int cheat_mode_state(void) { return g_cheat_on; }
static void mech_death_fire(int mode, unsigned int num)
{
    int ns = (g_death_state == 2) ? 1 : 2;
    (void)mode; (void)num;
    if (death_policy_apply(ns)) {
        wsprintfW(g_pull_msg, L"战死判定策略：%s", (ns == 2) ? L"已禁用" : L"已启用");
        logf_("[mech] 关闭战死 => %s", (ns == 2) ? "打开（免疫）" : "关闭（调低档）");
        settings_save();
    } else {
        wsprintfW(g_pull_msg, L"切换失败：内存字节校验未通过");
    }
    g_dirty = 1;
}
static void cheat_mode_fire(int mode, unsigned int num)
{
    (void)mode; (void)num;
    g_cheat_on = !g_cheat_on;
    wsprintfW(g_pull_msg, L"作弊模式：%s", g_cheat_on ? L"已开启" : L"已关闭");
    logf_("[cheat] 作弊模式 => %s", g_cheat_on ? "开（进详情页自动弹修改器）" : "关（详情页不弹，让位原生）");
    settings_save();
    g_dirty = 1;
}
static void cheat_troops_fire(int mode, unsigned int num) { (void)mode; (void)num; cheat_full_army(1, 0); }
static void cheat_train_fire(int mode, unsigned int num)  { (void)mode; (void)num; cheat_full_army(0, 1); }
static void cheat_refresh_fire(int mode, unsigned int num)
{
    do_refresh_action(mode, num);
    wsprintfW(g_pull_msg, L"行动状态已刷新"); g_dirty = 1;
}

static unsigned int deleg_gov_of(unsigned int city)
{
    unsigned int g;
    if (city < 1u || city > CITY_MAXIDX) return 0u;
    g = rd32(GOV_CITY_ARR + (city - 1u) * 4u);
    if (g == 0u || g == 0xFFFFFFFFu || g >= CHAR_IDX_MAX) return 0u;
    return g;
}
static void deleg_status_lines(unsigned int city, wchar_t *s0, wchar_t *s1)
{
    unsigned int gov = deleg_gov_of(city);
    if (!g_deleg[city]) {
        if (gov) {
            const wchar_t *gn = name_of(gov);
            if (gn) wsprintfW(s0, L"自营（太守 %s）", gn);
            else    wsprintfW(s0, L"自营（太守 #%u）", gov);
        } else {
            lstrcpynW(s0, L"自营（无太守）", 96);
        }
        lstrcpynW(s1, L"开启委任", 48);
    } else if (gov) {
        const wchar_t *gn = name_of(gov);
        if (gn) wsprintfW(s0, L"委任（太守 %s）", gn);
        else    wsprintfW(s0, L"委任（太守 #%u）", gov);
        lstrcpynW(s1, L"取消委任", 48);
    } else {
        lstrcpynW(s0, L"委任（自动增长）", 96);
        lstrcpynW(s1, L"取消委任", 48);
    }
}
static void deleg_budget_line(unsigned int city, wchar_t *out)
{
    int unk = 0, b = deleg_budget(city, &unk);
    if (unk) wsprintfW(out, L"预算≈%d（?项按估值计，首执后实测校准）", b);
    else     wsprintfW(out, L"预算≈%d", b);
}
static void deleg_budget_log(unsigned int c)
{
    wchar_t nm[24];
    int unk = 0, b = deleg_budget(c, &unk);
    city_name_copy(c, nm, 24);
    logf_("[dbudget] c%u %s 预=%d%s 治%u(估105) 墙%u(估410) 兵%u(估165) 粮%u(估410)",
          c, l10(nm), b, unk ? "(含估项)" : "",
          g_deleg_cost[c][2], g_deleg_cost[c][3], g_deleg_cost[c][4], g_deleg_cost[c][5]);
}
static void deleg_toggle(unsigned int city)
{
    wchar_t nm[24];
    g_player_f = player_fac();
    if (city < 1u || city > CITY_MAXIDX) {
        wsprintfW(g_pull_msg, L"当前不在城池界面"); g_dirty = 1; return;
    }
    if (!fac_trusted()) {
        wsprintfW(g_pull_msg, L"势力状态异常，暂无法委任"); g_dirty = 1; return;
    }
    if (rd32(CITY_OWNER + (city - 1u) * 4u) != g_player_f) {
        wsprintfW(g_pull_msg, L"本城非我方所属，无法委任"); g_dirty = 1; return;
    }
    city_name_copy(city, nm, 24);
    g_deleg[city] = (unsigned char)(g_deleg[city] ? 0 : 1);
    if (g_deleg[city]) {
        int k;
        if (!g_deleg_cfg[city]) {
            for (k = 0; k < DELEG_ITEMS; k++) {
                if (city_is_gate(city) && (k == 0 || k == 1 || k == 2 || k == 4)) {
                    g_deleg_target[city][k] = 0u;
                    continue;
                }
                if (g_deleg_target[city][k] == 0u)
                    g_deleg_target[city][k] = (unsigned short)deleg_dcap(city, k);
            }
            g_deleg_cfg[city] = 1;
        }
        if (deleg_gov_of(city)) wsprintfW(g_pull_msg, L"已委任 %s（有太守）", nm);
        else                    wsprintfW(g_pull_msg, L"已委任 %s（自动）", nm);
        logf_("[deleg] #%u %s 委任开启 pf=%u", city, l10(nm), g_player_f);
    } else {
        wsprintfW(g_pull_msg, L"已取消 %s 委任", nm);
        logf_("[deleg] #%u %s 委任取消", city, l10(nm));
    }
    settings_save();
    game_cache_refresh();
    g_dirty = 1;
}
static void deleg_open(int mode, unsigned int num)
{
    unsigned int city = 0u;
    if (mode == MODE_CITY) city = num;
    else {
        unsigned int m = rd32(CITY_MIRROR);
        if (m >= 1u && m <= CITY_MAXIDX) city = m + 1u;
    }
    if (city < 1u || city > CITY_MAXIDX) {
        wsprintfW(g_pull_msg, L"无法确定目标城池，请先进入一座我方城池"); g_dirty = 1;
        logf_("[deleg] 拒绝：无法确定城市 mode=%d num=%u", mode, num);
        return;
    }
    g_player_f = player_fac();
    if (!fac_trusted()) {
        wsprintfW(g_pull_msg, L"势力状态异常，暂无法委任"); g_dirty = 1;
        logf_("[deleg] 拒绝：玩家势力读数不可信");
        return;
    }
    if (rd32(CITY_OWNER + (city - 1u) * 4u) != g_player_f) {
        wsprintfW(g_pull_msg, L"本城非我方所属，无法委任"); g_dirty = 1;
        logf_("[deleg] 拒绝：#%u 归属=%u 非我方(pf=%u)",
              city, rd32(CITY_OWNER + (city - 1u) * 4u), g_player_f);
        return;
    }
    g_pull_dst = city;
    city_name_copy(city, g_pull_dstname, 24);
    g_pull_cur = 0; g_pull_msg[0] = 0;
    g_pull_mode = PANEL_DELEG; g_dirty = 1;
    logf_("[deleg] 进入委任面板：%s(#%u) 现态=%u 太守=%u",
          l10(g_pull_dstname), city, (unsigned int)g_deleg[city], deleg_gov_of(city));
}
static void deleg_round_end(void)
{
    unsigned int c, pf;
    int did = 0, cut = 0;
    g_player_f = player_fac();
    pf = g_player_f;
    if (pf == 0xFFFFFFFFu) return;
    for (c = 1u; c <= CITY_MAXIDX; c++) {
        unsigned int gov;
        if (!g_deleg[c]) continue;
        if (rd32(CITY_OWNER + (c - 1u) * 4u) != pf) {
            g_deleg[c] = 0;
            cut++;
            logf_("[deleg] 城%u 易主 ⇒ 自动解任", c);
            continue;
        }
        gov = deleg_gov_of(c);
        {
            unsigned int pol, g, pop, bud;
            unsigned int base = (unsigned int)(size_t)GetModuleHandleA(NULL);
            char lb[160];
            int lp = 0;
            int kk;
            for (kk = 0; kk < DELEG_ITEMS; kk++) {
                unsigned int t   = g_deleg_target[c][kk];
                unsigned int cur = rd32(CITY_ARR[kk] + c * 4u);
                if (city_is_gate(c) && (kk == 0 || kk == 1 || kk == 2 || kk == 4)) {
                    if (t) g_deleg_target[c][kk] = 0u;
                    continue;
                }
                if (t && t < cur && deleg_dcap(c, kk) <= cur)
                    g_deleg_target[c][kk] = (unsigned short)deleg_dcap(c, kk);
            }
            if (!base || !train_syms_ok(base)) {
                logf_("[deleg] 城%u 加法函数签名未过 ⇒ 跳过", c);
                continue;
            }
            if (gov) {
                static unsigned int acted_snap[850];
                unsigned int old_sf  = rd32(SETTLE_FAC);
                unsigned int old_pts = rd32(CMD_POINT_ARR + pf * 4u);
                unsigned int old_loc = 0u;
                unsigned int f16, f5, ii, rest = 0u, funds0, funds1;
                unsigned int famt = 0u, camt = 0u;
                int ran = 0;
                TRAINFN_ADD1 hn[4];
                pol  = rd32(RUNTIME_BASE + gov * 4u + FOFF[3]);
                for (ii = 0u; ii < 850u; ii++)
                    acted_snap[ii] = rd32(ACTED_FLAG + ii * ACTED_GRAN);
                if (gov < 850u) old_loc = rd32(CHAR_LOC_ARR + gov * 4u);
                wr32(SETTLE_FAC, pf);
                wr32(CMD_POINT_ARR + pf * 4u, 100u);
                if (gov < 850u) wr32(CHAR_LOC_ARR + gov * 4u, c - 1u);
                f16 = rd32(DONE_BITS + (c - 1u) * 4u);
                funds0 = rd32(FAC_FUNDS + pf * 4u);
                {
                    unsigned int tr_f = (unsigned int)deleg_tier_of(c, 0) + 2u;
                    unsigned int ur   = rd32(UNREST_ARR + (c - 1u) * 4u);
                    unsigned int tr_c = (unsigned int)deleg_tier_of(c, 1) + 2u;
                    if (deleg_will(c, 0))
                        famt = deleg_grow_disp(c, 0, pol * tr_f / 2u * ur / 100u);
                    if (deleg_will(c, 1))
                        camt = deleg_grow_disp(c, 1, pol * tr_c / 2u);
                }
                for (ii = 0u; ii < 4u; ii++) {
                    int k = DELEG_HK[ii];
                    unsigned int fb, fa, dbit;
                    unsigned int old;
                    unsigned int ci = c - 1u;
                    unsigned int bud0, bud1, cur0, cur1, dev0, dev1, act0, act1, cp0, cp1;
                    if (!deleg_will(c, k)) continue;
                    hn[ii] = (TRAINFN_ADD1)(size_t)(base + DELEG_HR[ii]);
                    bud0 = rd32(BUDGET_ARR + ci * 4u);
                    cur0 = rd32(CITY_ARR[k] + c * 4u);
                    dev0 = (k == 2) ? rd32(UNREST_ARR + ci * 4u)
                                    : rd32(deleg_dev_addr(k) + ci * 4u);
                    act0 = (gov < 850u) ? rd32(ACTED_FLAG + gov * ACTED_GRAN) : 0u;
                    cp0  = rd32(CMD_POINT_ARR + pf * 4u);
                    if (gov < 850u) wr32(ACTED_FLAG + gov * ACTED_GRAN, 0u);
                    fb = rd32(FAC_FUNDS + pf * 4u);
                    if (k == 2) {
                        unsigned int pa = POOL_ARR + (c - 1u) * 4u;
                        unsigned int p0 = rd32(pa);
                        if (rd32(UNREST_ARR + (c - 1u) * 4u) < p0) {
                            wr32(pa, 0u);
                            hn[ii](c - 1u);
                            wr32(pa, p0 + rd32(pa));
                        } else {
                            hn[ii](c - 1u);
                        }
                    } else if (k == 3 || k == 5) {
                        unsigned int wa = (k == 3) ? (CAP_WALL_ARR + (c - 1u) * 4u)
                                                   : (CAP_FOOD_T  + (c - 1u) * 4u);
                        unsigned int da = (k == 3) ? (WALL_DEV_ARR + (c - 1u) * 4u)
                                                   : (FOOD_DEV_ARR + (c - 1u) * 4u);
                        unsigned int w0 = rd32(wa);
                        unsigned int lim = deleg_dcap(c, k);
                        unsigned int db = (k == 3) ? 0x8u : 0x10u;
                        if (lim < w0) lim = w0;
                        wr32(wa, lim + 1000u);
                        hn[ii](c - 1u);
                        if (rd32(da) == dev0 && (rd32(DONE_BITS + ci * 4u) & db)) {
                            unsigned int step = pol / 2u + 10u;
                            unsigned int tgt  = g_deleg_target[c][k];
                            if (dev0 + step > tgt) step = tgt - dev0;
                            if (step)
                                ((TRAINFN_ADD2)(size_t)(base +
                                    ((k == 3) ? RVA_DEV_WALL : RVA_DEV_FOOD)))(ci, step);
                        }
                        wr32(wa, w0);
                        {
                            unsigned int v = rd32(da);
                            if (v > lim) wr32(da, lim);
                        }
                    } else if (k == 4) {
                        unsigned int sc = rd32(FAC_TRP_CAP + pf * 4u);
                        unsigned int ba = CAP_TRP_ARR + (c - 1u) * 4u;
                        unsigned int b0 = rd32(ba);
                        unsigned int lim = deleg_dcap(c, 4);
                        wr32(FAC_TRP_CAP + pf * 4u, sc * 40u + 1000000u);
                        if (lim < b0) lim = b0;
                        wr32(ba, lim + 1000u);
                        hn[ii](c - 1u);
                        wr32(ba, b0);
                        wr32(FAC_TRP_CAP + pf * 4u, sc);
                    } else {
                        hn[ii](c - 1u);
                    }
                    fa = rd32(FAC_FUNDS + pf * 4u);
                    bud1 = rd32(BUDGET_ARR + ci * 4u);
                    cur1 = rd32(CITY_ARR[k] + c * 4u);
                    dev1 = (k == 2) ? rd32(UNREST_ARR + ci * 4u)
                                    : rd32(deleg_dev_addr(k) + ci * 4u);
                    act1 = (gov < 850u) ? rd32(ACTED_FLAG + gov * ACTED_GRAN) : 0u;
                    cp1  = rd32(CMD_POINT_ARR + pf * 4u);
                    logf_("[dprobe] c%u k%u cur%u->%u dev%u->%u bud%u->%u "
                          "funds%u->%u cmd%u->%u act%02X->%02X tgt%u",
                          c, k, cur0, cur1, dev0, dev1, bud0, bud1,
                          fb, fa, cp0, cp1, act0, act1, g_deleg_target[c][k]);
                    dbit = (k == 2) ? 0x4u : (k == 3) ? 0x8u : (k == 5) ? 0x10u : 0x40u;
                    if (rd32(DONE_BITS + (c - 1u) * 4u) & dbit) {
                        unsigned int spent = (fb > fa) ? (fb - fa) : 0u;
                        old = g_deleg_cost[c][k];
                        g_deleg_cost[c][k] = old ? (old + spent) / 2u : spent;
                        ran++;
                    }
                }
                f5 = rd32(DONE_BITS + (c - 1u) * 4u);
                if (gov < 850u) wr32(CHAR_LOC_ARR + gov * 4u, old_loc);
                wr32(CMD_POINT_ARR + pf * 4u, old_pts);
                wr32(SETTLE_FAC, old_sf);
                funds1 = rd32(FAC_FUNDS + pf * 4u);
                for (ii = 0u; ii < 850u; ii++) {
                    unsigned int a = ACTED_FLAG + ii * ACTED_GRAN;
                    if (rd32(a) != acted_snap[ii]) { wr32(a, acted_snap[ii]); rest++; }
                }
                {
                    unsigned int bud = rd32(BUDGET_ARR + (c - 1u) * 4u);
                    unsigned int inc = pol * 3u + 9u;
                    if (bud + inc > 4000u) inc = (bud < 4000u) ? 4000u - bud : 0u;
                    if (inc) wr32(BUDGET_ARR + (c - 1u) * 4u, bud + inc);
                }
                lp += wsprintfA(lb + lp, "gov=%u farm+%u comm+%u farm=%u comm=%u "
                                "done %02X->%02X ran=%u funds-%u actors=%u",
                                gov, famt, camt,
                                rd32(CITY_ARR[0] + c * 4u), rd32(CITY_ARR[1] + c * 4u),
                                f16, f5, ran,
                                funds0 > funds1 ? funds0 - funds1 : 0u, rest);
                logf_("[deleg] gov city%u %s", c, lb);
                deleg_budget_log(c);
                game_cache_refresh();
                did++;
                continue;
            } else {
                pop = rd32(POP_ARR + (c - 1u) * 4u);
                bud = rd32(BUDGET_ARR + (c - 1u) * 4u);
                g = pop * bud / 100u;
                if (g == 0u) { logf_("[deleg] city%u pop=%u bud=%u g=0 跳过", c, pop, bud); continue; }
                if (deleg_will(c, 0)) deleg_grow_disp(c, 0, g);
                if (deleg_will(c, 1)) deleg_grow_disp(c, 1, g / 3u);
                if (deleg_will(c, 2)) ((TRAINFN_ADD2)(size_t)(base + RVA_DEV_ORDER))(c - 1u, g / 5u ? g / 5u : 1u);
                if (deleg_will(c, 3)) {
                    unsigned int wa = CAP_WALL_ARR + (c - 1u) * 4u, w0 = rd32(wa);
                    if (w0 < deleg_dcap(c, 3)) wr32(wa, deleg_dcap(c, 3) + 1000u);
                    ((TRAINFN_ADD2)(size_t)(base + RVA_DEV_WALL))(c - 1u, g / 10u ? g / 10u : 1u);
                    wr32(wa, w0);
                }
                if (deleg_will(c, 5)) {
                    unsigned int wa = CAP_FOOD_T + (c - 1u) * 4u, w0 = rd32(wa);
                    if (w0 < deleg_dcap(c, 5)) wr32(wa, deleg_dcap(c, 5) + 1000u);
                    ((TRAINFN_ADD2)(size_t)(base + RVA_DEV_FOOD))(c - 1u, g / 10u ? g / 10u : 1u);
                    wr32(wa, w0);
                }
                if (deleg_will(c, 4)) {
                    unsigned int a = TROOP_DEV_ARR + (c - 1u) * 4u;
                    unsigned int v = rd32(a), n = v + g;
                    if (n > CAP_TRP(c)) n = CAP_TRP(c);
                    if (n != v) wr32(a, n);
                }
                if (bud >= g) wr32(BUDGET_ARR + (c - 1u) * 4u, bud - g);
                logf_("[dprobe] auto c%u ord%u/%u wall%u/%u trp%u/%u food%u/%u",
                      c,
                      rd32(CITY_ARR[2] + c * 4u), g_deleg_target[c][2],
                      rd32(CITY_ARR[3] + c * 4u), g_deleg_target[c][3],
                      rd32(CITY_ARR[4] + c * 4u), g_deleg_target[c][4],
                      rd32(CITY_ARR[5] + c * 4u), g_deleg_target[c][5]);
                lp += wsprintfA(lb + lp, "pop=%u bud=%u g=%u", pop, bud, g);
                logf_("[deleg] auto city%u %s", c, lb);
                deleg_budget_log(c);
            }
            did++;
        }
    }
    if (did) { game_cache_refresh(); g_dirty = 1; logf_("[deleg] 本季代办 %d 城", did); }
    if (cut) { game_cache_refresh(); g_dirty = 1; }
}
static void deleg_on_load(void)
{
    settings_load();
}
static const MechDesc MECHS_MECH[] = {
    { L"禁用战死判定", mech_death_state, mech_death_fire },
};
static const MechDesc MECHS_CHEAT[] = {
    { L"作弊模式",           cheat_mode_state, cheat_mode_fire },
    { L"我方武将兵力调满",   NULL,             cheat_troops_fire },
    { L"我方武将训练度调满", NULL,             cheat_train_fire },
    { L"刷新我方武将行动",   NULL,             cheat_refresh_fire },
};

typedef struct {
    const char *name;
    void (*on_round_end)(void);
    void (*on_scene_change)(int from, int to);
    int  (*panel_entries)(const MechDesc **out);
    void (*on_game_load)(void);
} GameRule;

static int death_entries(const MechDesc **out)
{
    *out = MECHS_MECH;
    return (int)(sizeof(MECHS_MECH)/sizeof(MECHS_MECH[0]));
}
static const GameRule rule_death_policy = { "death_policy", NULL, NULL, death_entries, NULL };
static const GameRule rule_delegation = { "city_delegation", deleg_round_end, NULL, NULL, deleg_on_load };
static const GameRule * const g_rules[] = { &rule_death_policy, &rule_delegation };
#define NRULES ((int)(sizeof(g_rules)/sizeof(g_rules[0])))

static int mech_all_items(MechDesc *out, int cap)
{
    int i, n = 0;
    for (i = 0; i < NRULES; i++) {
        const MechDesc *arr;
        int c, k;
        if (!g_rules[i]->panel_entries) continue;
        c = g_rules[i]->panel_entries(&arr);
        for (k = 0; k < c && n < cap; k++) out[n++] = arr[k];
    }
    return n;
}

typedef struct {
    const char *tag;
    unsigned lo, hi;
    int  auto_on_change;
} DiagSection;
static const DiagSection DIAGS[] = {
    { "uictrl",  0x00561500u, 0x00562400u, 1 },
    { "seg1_ai", 0x005D0000u, 0x005E1000u, 0 },
    { "seg1_ui", 0x005E1000u, 0x005F0000u, 0 },
    { "cmd",     0x005F6000u, 0x005FE400u, 0 },
    { "desc_d1", 0x00600E00u, 0x00601400u, 1 },
    { "desc_d2", 0x00612400u, 0x00612800u, 0 },
};
#define NDIAGS ((int)(sizeof(DIAGS)/sizeof(DIAGS[0])))
static LONG g_diagSeq[NDIAGS] = { 0 };

static void diag_dump_one(int i)
{
    char dir[MAX_PATH], sub[MAX_PATH], path[MAX_PATH];
    HANDLE h; DWORD wr;
    unsigned sz;
    if (!mod_dir(dir)) return;
    sz = DIAGS[i].hi - DIAGS[i].lo;
    g_diagSeq[i] = (g_diagSeq[i] + 1) % 8;
    wsprintfA(sub, "%s\\zydiag", dir);
    CreateDirectoryA(sub, NULL);
    wsprintfA(sub, "%s\\zydiag\\%s", dir, DIAGS[i].tag);
    CreateDirectoryA(sub, NULL);
    wsprintfA(path, "%s\\%d.BIN", sub, (int)g_diagSeq[i]);
    h = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) { logf_("[diag] open fail %s err=%lu", path, GetLastError()); return; }
    wr = 0;
    if (!WriteFile(h, (void *)(size_t)DIAGS[i].lo, sz, &wr, NULL) || wr != sz)
        logf_("[diag] %s short write wr=%lu/%u err=%lu", DIAGS[i].tag, wr, sz, GetLastError());
    CloseHandle(h);
    logf_("[diag] zydiag\\%s\\%d.BIN %08X..%08X %luB", DIAGS[i].tag, (int)g_diagSeq[i],
          DIAGS[i].lo, DIAGS[i].hi, (unsigned long)wr);
}
static void diag_dump_all(void)
{
    int i;
    for (i = 0; i < NDIAGS; i++) diag_dump_one(i);
}
static void diag_scene_hook(int from, int to)
{
    int i;
    (void)from; (void)to;
    for (i = 0; i < NDIAGS; i++)
        if (DIAGS[i].auto_on_change) diag_dump_one(i);
}

static void panel_step(int scene, unsigned int tc, int mode, unsigned int num,
                     unsigned int pg, unsigned int idx, unsigned int mir, unsigned int ctrl)
{

    static int g_pendScene = -1, g_pendN = 0;
    int i, conv, was_conv, entering, confirmed;

    if (scene != g_pendScene) { g_pendScene = scene; g_pendN = 1; return; }
    if (g_pendN < 1) { g_pendN++; return; }
    confirmed = g_pendScene;
    if (confirmed == g_lastScene) return;

    logf_("[scene] %d->%d mode=%d num=%u pg=%u idx=%u mir=%u ctrl=%X",
          g_lastScene, confirmed, mode, num, pg, idx, mir, ctrl);

    conv     = scene_is_convocation(confirmed);
    was_conv = scene_is_convocation(g_lastScene);
    entering = conv && !was_conv;
    diag_scene_hook(g_lastScene, confirmed);

    for (i = 0; i < NRULES; i++) {
        if (g_rules[i]->on_scene_change) g_rules[i]->on_scene_change(g_lastScene, confirmed);
        if (g_lastScene == SCENE_TITLE && confirmed != SCENE_TITLE && g_rules[i]->on_game_load)
            g_rules[i]->on_game_load();
    }
    if (!conv) {

        int is_map = (confirmed == SCENE_OTHER && mode == MODE_NONE);
        if (is_map) {
            if (g_show && g_pull_mode != PANEL_MAP) {
                g_pull_cur    = 0;
                g_pull_msg[0] = 0;
                g_pull_mode   = PANEL_MAP;
                g_dirty       = 1;
                logf_("[panel] 大地图 ⇒ 自动弹主菜单");
            }
        } else if (g_pull_mode) {

            g_pull_mode = PANEL_OFF;
            g_pull_msg[0] = 0; g_dirty = 1;
            logf_("[panel] 离开召集场景(scene=%d) ⇒ 收起", confirmed);
        }
    } else if (entering) {

        if ((g_pull_mode == PANEL_OFF || g_pull_mode == PANEL_MAP)
                && g_show && tc <= CITY_MAXIDX) {
            unsigned int pf = rd32(PLAYER_FAC);
            unsigned int o  = rd32(CITY_OWNER + tc * 4u);
            if (o == pf) {
                g_pull_dst = tc + 1u;
                city_name_copy(g_pull_dst, g_pull_dstname, 24);
                g_pull_cur = 0; g_pull_msg[0] = 0; g_pull_mode = PANEL_FUNC; g_dirty = 1;
                logf_("[panel] 弹城市菜单（召集/训练）：tc=%u owner=%u pf=%u",
                      tc, o, pf);
            }
        }
    }

    g_lastScene = confirmed;
}

static unsigned g_lastSeason = 0xFFFFFFFFu;
static void rules_round_check(void)
{
    unsigned seas = rd32(0x005DD454u);
    int i;
    if (g_lastSeason != 0xFFFFFFFFu && seas != g_lastSeason)
        for (i = 0; i < NRULES; i++)
            if (g_rules[i]->on_round_end) g_rules[i]->on_round_end();
    g_lastSeason = seas;
}

static int menu_nav(int act, int mode, int rows, unsigned int num,
                    const MechDesc *items, int nm, int exit_cur, const char *exit_log)
{
    (void)rows;
    if (g_pull_cur < 0 || g_pull_cur >= nm) g_pull_cur = 0;
    if (act == ACT_CONFIRM || act == ACT_INC) {
        items[g_pull_cur].fire(mode, num);
        return 1;
    }
    if (act == ACT_UP || act == ACT_DOWN) {
        g_pull_cur = (g_pull_cur + ((act == ACT_DOWN) ? 1 : -1) + nm) % nm;
        g_dirty = 1; return 1;
    }
    if (act == ACT_EXIT) {
        g_pull_mode = PANEL_MAP; g_pull_cur = exit_cur;
        g_pull_msg[0] = 0; g_dirty = 1;
        logf_("%s", exit_log);
        return 1;
    }
    if (act == ACT_DEC) { g_dirty = 1; return 1; }
    return 0;
}

static void paint(void)
{
    if (!ensure_dib()) return;

    float f = g_scale * g_scaleAdj;
    unsigned int num = 0;
    int mode = detect_mode(&num);

    HDC dc = CreateCompatibleDC(NULL);
    HGDIOBJ ob = SelectObject(dc, g_dib);

    RECT r = { 0, 0, (LONG)g_uiW, (LONG)g_uiH };
    HBRUSH bg = CreateSolidBrush(mode_bg(mode));
    FillRect(dc, &r, bg);

    HPEN pen = CreatePen(PS_SOLID, (int)(1.5f * f), mode_edge(mode));
    HGDIOBJ op  = SelectObject(dc, pen);
    HGDIOBJ obr = SelectObject(dc, GetStockObject(NULL_BRUSH));
    Rectangle(dc, 1, 1, (int)g_uiW - 1, (int)g_uiH - 1);
    SelectObject(dc, op); SelectObject(dc, obr); DeleteObject(pen);

    SetBkMode(dc, TRANSPARENT);

    HFONT fT = CreateFontW((int)(-17 * f), 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
                           OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                           DEFAULT_PITCH, L"SimHei");
    HFONT fB = CreateFontW((int)(-15 * f), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
                           OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                           DEFAULT_PITCH, L"SimHei");
    HFONT fS = CreateFontW((int)(-12 * f), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
                           OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                           DEFAULT_PITCH, L"SimHei");

    wchar_t buf[128];

    HGDIOBJ of = SelectObject(dc, fT);
    SetTextColor(dc, mode_title(mode));

    if (g_pull_mode == PANEL_FUNC || g_pull_mode == PANEL_MAP) {
        int k, nitems;

        static const wchar_t *conv_items[3] = { L"召集", L"训练", L"委任" };
        static const wchar_t *snap_items[2] = { L"机制", L"作弊" };
        const wchar_t * const *items;
        if (g_pull_mode == PANEL_FUNC) {
            items = conv_items; nitems = 3;
            if (g_pull_dstname[0]) wsprintfW(buf, L"城市菜单→%s", g_pull_dstname);
            else                   wsprintfW(buf, L"城市菜单");
        } else {
            items = snap_items; nitems = 2;
            wsprintfW(buf, L"菜单");
        }
        TextOutW(dc, (int)(10 * f), (int)(UI_TITLE_Y * f), buf, lstrlenW(buf));
        if (g_pull_cur < 0 || g_pull_cur >= nitems) g_pull_cur = 0;
        SelectObject(dc, fB);
        for (k = 0; k < nitems; k++) {
            int y = (int)((HDR_H + k * ROW_H) * f);
            if (k == g_pull_cur) {
                RECT hr = { (int)(4 * f), y - (int)(2 * f),
                            (LONG)g_uiW - (int)(5 * f), y + (int)(21 * f) };
                HBRUSH hb = CreateSolidBrush(UI_COL_ROWHL);
                FillRect(dc, &hr, hb); DeleteObject(hb);
                RECT lr = { (int)(4 * f), y - (int)(2 * f), (int)(7 * f), y + (int)(21 * f) };
                HBRUSH lb = CreateSolidBrush(UI_COL_BAR);
                FillRect(dc, &lr, lb); DeleteObject(lb);
            }
            SetTextColor(dc, (k == g_pull_cur) ? UI_COL_TXT : UI_COL_TXTDIM);
            TextOutW(dc, (int)(UI_TEXT_X * f), y, items[k], lstrlenW(items[k]));

        }
        SelectObject(dc, fS);
        {
            int yv = (int)((HDR_H + nitems * ROW_H + UI_HINT_GAP) * f);
            int av = (int)g_uiW - (int)(20 * f);
            int lh = (int)(UI_HINT_LH * f);
            int nl;
            SetTextColor(dc, UI_COL_HINT);
            nl = wrap_text(dc, L"Ctrl+\x2191\x2193=选择 · Ctrl+Enter=进入 · Ctrl+E=退出", (int)(UI_HINT_X * f), yv, lh, av);
            if (g_pull_msg[0]) {
                SetTextColor(dc, UI_COL_MSG);
                wrap_text(dc, g_pull_msg, (int)(UI_HINT_X * f), yv + nl * lh, lh, av);
            }
        }
        SelectObject(dc, of);
        DeleteObject(fT); DeleteObject(fB); DeleteObject(fS);
        SelectObject(dc, ob);
        DeleteDC(dc);
        {
            unsigned char *p = (unsigned char *)g_bits;
            int n = (int)(g_uiW * g_uiH), kk;
            for (kk = 0; kk < n; kk++) p[kk * 4 + 3] = 255;
        }
        return;
    }

    if (g_pull_mode == PANEL_MECH) {
        MechDesc its[8];
        int k, nm = mech_all_items(its, 8), on = mech_death_state();
        wsprintfW(buf, L"机制");
        TextOutW(dc, (int)(10 * f), (int)(UI_TITLE_Y * f), buf, lstrlenW(buf));
        if (g_pull_cur < 0 || g_pull_cur >= nm) g_pull_cur = 0;
        SelectObject(dc, fB);
        for (k = 0; k < nm; k++) {
            int y = (int)((HDR_H + k * ROW_H) * f);
            if (k == g_pull_cur) {
                RECT hr = { (int)(4 * f), y - (int)(2 * f),
                            (LONG)g_uiW - (int)(5 * f), y + (int)(21 * f) };
                HBRUSH hb = CreateSolidBrush(UI_COL_ROWHL);
                FillRect(dc, &hr, hb); DeleteObject(hb);
                RECT lr = { (int)(4 * f), y - (int)(2 * f), (int)(7 * f), y + (int)(21 * f) };
                HBRUSH lb = CreateSolidBrush(UI_COL_BAR);
                FillRect(dc, &lr, lb); DeleteObject(lb);
            }
            SetTextColor(dc, (k == g_pull_cur) ? UI_COL_TXT : UI_COL_TXTDIM);
            TextOutW(dc, (int)(UI_TEXT_X * f), y, its[k].label, lstrlenW(its[k].label));
            if (its[k].state) {
                int st = its[k].state();
                SetTextAlign(dc, TA_RIGHT | TA_TOP);
                SetTextColor(dc, st ? UI_COL_BAR : UI_COL_OFFDIM);
                TextOutW(dc, (int)(g_uiW - 12 * f), y, st ? L"开启" : L"关闭", 2);
                SetTextAlign(dc, TA_LEFT | TA_TOP);
            }

        }
        SelectObject(dc, fS);
        {
            int yv = (int)((HDR_H + nm * ROW_H + UI_HINT_GAP) * f);
            int av = (int)g_uiW - (int)(20 * f);
            int lh = (int)(UI_HINT_LH * f);
            int nl;
            SetTextColor(dc, UI_COL_HINT);
            if (on) lstrcpyW(buf, L"Ctrl+Enter=切换 · 战死判定：已禁用");
            else    lstrcpyW(buf, L"Ctrl+Enter=切换 · 战死判定：调低档");
            nl = wrap_text(dc, buf, (int)(UI_HINT_X * f), yv, lh, av);
            if (g_pull_msg[0]) {
                SetTextColor(dc, UI_COL_MSG);
                wrap_text(dc, g_pull_msg, (int)(UI_HINT_X * f), yv + nl * lh, lh, av);
            }
        }
        SelectObject(dc, of);
        DeleteObject(fT); DeleteObject(fB); DeleteObject(fS);
        SelectObject(dc, ob);
        DeleteDC(dc);
        {
            unsigned char *p = (unsigned char *)g_bits;
            int n = (int)(g_uiW * g_uiH), kk;
            for (kk = 0; kk < n; kk++) p[kk * 4 + 3] = 255;
        }
        return;
    }

    if (g_pull_mode == PANEL_DELEG) {
        wchar_t s0[96], s1[48], bud[96];
        int on = g_deleg[g_pull_dst] ? 1 : 0;
        int k;
        deleg_status_lines(g_pull_dst, s0, s1);
        deleg_budget_line(g_pull_dst, bud);
        if (g_pull_cur < 0 || g_pull_cur > DELEG_ITEMS) g_pull_cur = 0;
        wsprintfW(buf, L"委任→%s", g_pull_dstname);
        TextOutW(dc, (int)(10 * f), (int)(UI_TITLE_Y * f), buf, lstrlenW(buf));
        {
            int yAct = (int)(HDR_H * f);
            RECT hr = { (int)(4 * f), yAct - (int)(2 * f),
                        (LONG)g_uiW - (int)(5 * f), yAct + (int)(ROW_H - 4) * f };
            HBRUSH hb;
            if (g_pull_cur == 0) {
                hb = CreateSolidBrush(UI_COL_ROWHL); FillRect(dc, &hr, hb); DeleteObject(hb);
                { RECT lr = { (int)(4 * f), yAct - (int)(2 * f), (int)(7 * f),
                              yAct + (int)(ROW_H - 4) * f };
                  hb = CreateSolidBrush(UI_COL_BAR); FillRect(dc, &lr, hb); DeleteObject(hb); }
            }
            SelectObject(dc, fB);
            SetTextColor(dc, on ? UI_COL_STEPDOWN : UI_COL_TXT);
            TextOutW(dc, (int)(UI_TEXT_X * f), yAct, s1, lstrlenW(s1));
        }
        SelectObject(dc, fS);
        SetTextColor(dc, UI_COL_VALCUR);
        TextOutW(dc, (int)(UI_TEXT_X * f), (int)((HDR_H + ROW_H) * f), s0, lstrlenW(s0));
        {
            int tx0 = (int)(40 * f);
            int tx1 = (int)(g_uiW - 46 * f);
            int tw  = tx1 - tx0;
            for (k = 0; k < DELEG_ITEMS; k++) {
                int y  = (int)((HDR_H + (k + 2) * ROW_H) * f);
                unsigned int cur = rd32(CITY_ARR[k] + g_pull_dst * 4u);
                unsigned int tgt = g_deleg_target[g_pull_dst][k];
                int will = deleg_will(g_pull_dst, k);
                int gate_off = city_is_gate(g_pull_dst) &&
                               (k == 0 || k == 1 || k == 2 || k == 4);
                int tier_cur = (deleg_pct_cache(g_pull_dst, k) >= 75) ? 4 :
                               (deleg_pct_cache(g_pull_dst, k) >= 50) ? 3 :
                               (deleg_pct_cache(g_pull_dst, k) >= 25) ? 2 : 1;
                int tier_tgt = deleg_tier_of(g_pull_dst, k);
                int fx = tx0 + deleg_fill(g_pull_dst, k, cur) * tw / 100;
                int cx = tx0 + deleg_fill(g_pull_dst, k, tgt) * tw / 100;
                int q;
                RECT tr;
                HBRUSH hb;
                wchar_t nm[24];
                if (k + 1 == g_pull_cur) {
                    RECT hr = { (int)(4 * f), y - (int)(2 * f),
                                (LONG)g_uiW - (int)(5 * f), y + (int)(ROW_H - 6) * f };
                    hb = CreateSolidBrush(UI_COL_ROWHL);
                    FillRect(dc, &hr, hb); DeleteObject(hb);
                }
                SetTextColor(dc, gate_off ? UI_COL_OFFDIM :
                               (on ? (will ? UI_COL_STEPUP : UI_COL_TXTDIM) : UI_COL_OFFDIM));
                TextOutW(dc, (int)(UI_TEXT_X * f), y, DELEG_LBL[k], lstrlenW(DELEG_LBL[k]));
                tr.left = tx0; tr.top = y + (int)(6 * f);
                tr.right = tx1; tr.bottom = y + (int)(12 * f);
                hb = CreateSolidBrush(UI_COL_HINT); FillRect(dc, &tr, hb); DeleteObject(hb);
                if (!gate_off) {
                for (q = 1; q <= 4; q++) {
                    int qx = tx0 + tw * q / 4;
                    RECT qr = { qx - (int)(1 * f), y + (int)((q == tier_tgt && on) ? 1 : 4) * f,
                                qx + (int)(2 * f), y + (int)(14 * f) };
                    hb = CreateSolidBrush((q == tier_tgt && on) ? UI_COL_Z_WHT : UI_COL_OFFDIM);
                    FillRect(dc, &qr, hb); DeleteObject(hb);
                }
                tr.right = fx;
                hb = CreateSolidBrush((tgt > 0u && cur >= tgt) ? UI_COL_OVER :
                                      (on ? deleg_zone_color(tier_cur) : UI_COL_TXTDIM));
                FillRect(dc, &tr, hb); DeleteObject(hb);
                {
                    RECT cr = { cx - (int)(1 * f), y + (int)(3 * f),
                                cx + (int)(3 * f), y + (int)(15 * f) };
                    hb = CreateSolidBrush(tier_tgt ? deleg_zone_color(tier_tgt)
                                                   : UI_COL_VALCUR);
                    FillRect(dc, &cr, hb); DeleteObject(hb);
                }
                }
                {
                    wchar_t nm[24];
                    if (gate_off) lstrcpynW(nm, L"—", 24);
                    else          wsprintfW(nm, L"%u", tgt);
                    SetTextColor(dc, (k + 1) == g_pull_cur ? UI_COL_VALCUR : UI_COL_TXTDIM);
                    TextOutW(dc, (int)g_uiW - (int)(42 * f), y, nm, lstrlenW(nm));
                }
            }
        }
        {
            int yy = (int)((HDR_H + (DELEG_ITEMS + 3) * ROW_H) * f);
            int av = (int)g_uiW - (int)(20 * f);
            int lh = (int)(UI_HINT_LH * f);
            int nl;
            wchar_t note[96];
            deleg_note(g_pull_dst, note);
            SetTextColor(dc, on ? UI_COL_MSG : UI_COL_HINT);
            nl = wrap_text(dc, bud, (int)(UI_HINT_X * f), yy, lh, av);
            yy += nl * lh;
            if (on && note[0]) {
                wchar_t bl[120];
                SetTextColor(dc, UI_COL_TXTDIM);
                wsprintfW(bl, L"拦: %s", note);
                nl = wrap_text(dc, bl, (int)(UI_HINT_X * f), yy, lh, av);
                yy += nl * lh;
            }
            SetTextColor(dc, UI_COL_HINT);
            nl = wrap_text(dc, on ? L"Ctrl+\x2190\x2192=切档 · Ctrl+\x2191\x2193=选条"
                                  : L"Enter开启后可调目标",
                           (int)(UI_HINT_X * f), yy, lh, av);
            yy += nl * lh;
            if (on) {
                nl = wrap_text(dc, L"Ctrl+Enter=开关 · Ctrl+E=退出",
                               (int)(UI_HINT_X * f), yy, lh, av);
                yy += nl * lh;
            }
            if (g_pull_msg[0]) {
                yy += lh / 2;
                SetTextColor(dc, UI_COL_MSG);
                wrap_text(dc, g_pull_msg, (int)(UI_HINT_X * f), yy, lh, av);
            }
        }
        SelectObject(dc, of);
        DeleteObject(fT); DeleteObject(fB); DeleteObject(fS);
        SelectObject(dc, ob);
        DeleteDC(dc);
        {
            unsigned char *p = (unsigned char *)g_bits;
            int n = (int)(g_uiW * g_uiH), kk;
            for (kk = 0; kk < n; kk++) p[kk * 4 + 3] = 255;
        }
        return;
    }

    if (g_pull_mode == PANEL_CHEAT) {
        int k, nm = (int)(sizeof(MECHS_CHEAT)/sizeof(MECHS_CHEAT[0]));
        wsprintfW(buf, L"作弊");
        TextOutW(dc, (int)(10 * f), (int)(UI_TITLE_Y * f), buf, lstrlenW(buf));
        if (g_pull_cur < 0 || g_pull_cur >= nm) g_pull_cur = 0;
        SelectObject(dc, fB);
        for (k = 0; k < nm; k++) {
            int y = (int)((HDR_H + k * ROW_H) * f);
            if (k == g_pull_cur) {
                RECT hr = { (int)(4 * f), y - (int)(2 * f),
                            (LONG)g_uiW - (int)(5 * f), y + (int)(21 * f) };
                HBRUSH hb = CreateSolidBrush(UI_COL_ROWHL);
                FillRect(dc, &hr, hb); DeleteObject(hb);
                RECT lr = { (int)(4 * f), y - (int)(2 * f), (int)(7 * f), y + (int)(21 * f) };
                HBRUSH lb = CreateSolidBrush(UI_COL_BAR);
                FillRect(dc, &lr, lb); DeleteObject(lb);
            }
            SetTextColor(dc, (k == g_pull_cur) ? UI_COL_TXT : UI_COL_TXTDIM);
            TextOutW(dc, (int)(UI_TEXT_X * f), y, MECHS_CHEAT[k].label, lstrlenW(MECHS_CHEAT[k].label));

            if (MECHS_CHEAT[k].state) {
                int st = MECHS_CHEAT[k].state();
                const wchar_t *sw = st ? L"开启" : L"关闭";
                SetTextAlign(dc, TA_RIGHT | TA_TOP);
                SetTextColor(dc, st ? UI_COL_BAR : UI_COL_OFFDIM);
                TextOutW(dc, (int)(g_uiW - 12 * f), y, sw, 2);
                SetTextAlign(dc, TA_LEFT | TA_TOP);
            }
        }
        SelectObject(dc, fS);
        {
            int yv = (int)((HDR_H + nm * ROW_H + UI_HINT_GAP) * f);
            int av = (int)g_uiW - (int)(20 * f);
            int lh = (int)(UI_HINT_LH * f);
            int nl;
            SetTextColor(dc, UI_COL_HINT);
            nl = wrap_text(dc, L"Ctrl+\x2191\x2193=选择 · Ctrl+Enter=执行 · Ctrl+E=退出", (int)(UI_HINT_X * f), yv, lh, av);
            if (g_pull_msg[0]) {
                SetTextColor(dc, UI_COL_MSG);
                wrap_text(dc, g_pull_msg, (int)(UI_HINT_X * f), yv + nl * lh, lh, av);
            }
        }
        SelectObject(dc, of);
        DeleteObject(fT); DeleteObject(fB); DeleteObject(fS);
        SelectObject(dc, ob);
        DeleteDC(dc);
        {
            unsigned char *p = (unsigned char *)g_bits;
            int n = (int)(g_uiW * g_uiH), kk;
            for (kk = 0; kk < n; kk++) p[kk * 4 + 3] = 255;
        }
        return;
    }

    if (g_pull_mode == PANEL_TRAIN) {
        int k;
        unsigned int pf = rd32(PLAYER_FAC);
        int left = (pf >= 1u && pf <= 11u) ? (int)rd32(CMD_POINT_ARR + pf * 4u) : 0;
        wsprintfW(buf, L"训练→%s", g_pull_dstname);
        TextOutW(dc, (int)(10 * f), (int)(UI_TITLE_Y * f), buf, lstrlenW(buf));
        SelectObject(dc, fB);
        for (k = 0; k < PULL_VIS; k++) {
            int li = g_pull_top + k;
            int y  = (int)((HDR_H + k * ROW_H) * f);
            wchar_t sbuf[96];
            int acted = 0, bad = 0;
            if (li < g_trn_n) {
                unsigned int idx = g_trn_num[li];
                const wchar_t *gn = name_of(idx);
                unsigned int tr   = rd32(TRAIN_ARR + idx * 4u);
                acted = (rd32(ACTED_FLAG + idx * 4u) & 1u) != 0;
                bad   = (idx > 403u);
                if (bad)        wsprintfW(sbuf, L"%s 训练 %u 异常", gn ? gn : L"?", tr);
                else if (acted) wsprintfW(sbuf, L"%s 训练 %u 已行动", gn ? gn : L"?", tr);
                else            wsprintfW(sbuf, L"%s 训练 %u", gn ? gn : L"?", tr);
            } else {
                lstrcpynW(sbuf, L"—", 96);
            }
            if (li == g_pull_cur && li < g_trn_n) {
                RECT hr = { (int)(4 * f), y - (int)(2 * f),
                            (LONG)g_uiW - (int)(5 * f), y + (int)(21 * f) };
                HBRUSH hb = CreateSolidBrush(UI_COL_ROWHL);
                FillRect(dc, &hr, hb); DeleteObject(hb);
                RECT lr = { (int)(4 * f), y - (int)(2 * f), (int)(7 * f), y + (int)(21 * f) };
                HBRUSH lb = CreateSolidBrush(UI_COL_BAR);
                FillRect(dc, &lr, lb); DeleteObject(lb);
            }
            if (li == g_pull_cur && li < g_trn_n)      SetTextColor(dc, UI_COL_TXT);
            else if (acted || bad)                     SetTextColor(dc, UI_COL_OFFDIM);
            else                                       SetTextColor(dc, UI_COL_TXTDIM);
            TextOutW(dc, (int)(UI_TEXT_X * f), y, sbuf, lstrlenW(sbuf));
        }
        SelectObject(dc, fS);
        {
            int yv = (int)((HDR_H + PULL_VIS * ROW_H + UI_HINT_GAP) * f);
            int av = (int)g_uiW - (int)(20 * f);
            int lh = (int)(UI_HINT_LH * f);
            int nl;
            SetTextColor(dc, UI_COL_HINT);
            nl = wrap_text(dc, L"Ctrl+\x2191\x2193=选择武将 · Ctrl+Enter=训练（消耗 1 命令点） · Ctrl+A=全部训练", (int)(UI_HINT_X * f), yv, lh, av);
            wsprintfW(buf, L"(武将 %d · 命令点 %d)", g_trn_n, left);
            TextOutW(dc, (int)(UI_HINT_X * f), yv + nl * lh, buf, lstrlenW(buf));
            if (g_pull_msg[0]) {
                SetTextColor(dc, UI_COL_MSG);
                wrap_text(dc, g_pull_msg, (int)(UI_HINT_X * f), yv + (nl + 1) * lh, lh, av);
            }
        }
        SelectObject(dc, of);
        DeleteObject(fT); DeleteObject(fB); DeleteObject(fS);
        SelectObject(dc, ob);
        DeleteDC(dc);
        {
            unsigned char *p = (unsigned char *)g_bits;
            int n = (int)(g_uiW * g_uiH), kk;
            for (kk = 0; kk < n; kk++) p[kk * 4 + 3] = 255;
        }
        return;
    }

    if (g_pull_mode == PANEL_MAIN) {
        int k;
        static const wchar_t *items[2] = { L"士兵", L"武将" };
        wsprintfW(buf, L"召集→%s", g_pull_dstname);
        TextOutW(dc, (int)(10 * f), (int)(UI_TITLE_Y * f), buf, lstrlenW(buf));
        SelectObject(dc, fB);
        for (k = 0; k < 2; k++) {
            int y = (int)((HDR_H + k * ROW_H) * f);
            if (k == g_pull_cur) {
                RECT hr = { (int)(4 * f), y - (int)(2 * f),
                            (LONG)g_uiW - (int)(5 * f), y + (int)(21 * f) };
                HBRUSH hb = CreateSolidBrush(UI_COL_ROWHL);
                FillRect(dc, &hr, hb); DeleteObject(hb);
                RECT lr = { (int)(4 * f), y - (int)(2 * f), (int)(7 * f), y + (int)(21 * f) };
                HBRUSH lb = CreateSolidBrush(UI_COL_BAR);
                FillRect(dc, &lr, lb); DeleteObject(lb);
            }
            SetTextColor(dc, (k == g_pull_cur) ? UI_COL_TXT : UI_COL_TXTDIM);
            TextOutW(dc, (int)(UI_TEXT_X * f), y, items[k], lstrlenW(items[k]));
        }
        SelectObject(dc, fS);
        {
            int yv = (int)((HDR_H + 2 * ROW_H + UI_HINT_GAP) * f);
            SetTextColor(dc, UI_COL_HINT);
            {
                int av = (int)g_uiW - (int)(20 * f);
                int lh = (int)(UI_HINT_LH * f);
                int nl;
                nl = wrap_text(dc, L"Ctrl+\x2191\x2193=选择 · Ctrl+Enter=进入 · Ctrl+E=退出", (int)(UI_HINT_X * f), yv, lh, av);
                if (g_pull_msg[0]) {
                    SetTextColor(dc, UI_COL_MSG);
                    wrap_text(dc, g_pull_msg, (int)(UI_HINT_X * f), yv + nl * lh, lh, av);
                }
            }
        }
        SelectObject(dc, of);
        DeleteObject(fT); DeleteObject(fB); DeleteObject(fS);
        SelectObject(dc, ob);
        DeleteDC(dc);
        {
            unsigned char *p = (unsigned char *)g_bits;
            int n = (int)(g_uiW * g_uiH), kk;
            for (kk = 0; kk < n; kk++) p[kk * 4 + 3] = 255;
        }
        return;
    }

    if (g_pull_mode == PANEL_SOLDIER) {
        int k;
        wsprintfW(buf, L"召集·士兵→%s", g_pull_dstname);
        TextOutW(dc, (int)(10 * f), (int)(UI_TITLE_Y * f), buf, lstrlenW(buf));
        SelectObject(dc, fB);
        for (k = 0; k < PULL_VIS; k++) {
            int li = g_pull_top + k;
            int y  = (int)((HDR_H + k * ROW_H) * f);
            wchar_t sbuf[96];
            if (li < g_pull_n) {
                if (g_pull_amt[li] > 1000u)
                    wsprintfW(sbuf, L"%s %u (+%u)", g_pull_name[li], g_pull_amt[li],
                              (unsigned)(g_pull_amt[li] - 1000u));
                else
                    wsprintfW(sbuf, L"%s %u", g_pull_name[li], g_pull_amt[li]);
            } else {
                lstrcpynW(sbuf, L"—", 96);
            }
            if (li == g_pull_cur && li < g_pull_n) {
                RECT hr = { (int)(4 * f), y - (int)(2 * f),
                            (LONG)g_uiW - (int)(5 * f), y + (int)(21 * f) };
                HBRUSH hb = CreateSolidBrush(UI_COL_ROWHL);
                FillRect(dc, &hr, hb); DeleteObject(hb);
                RECT lr = { (int)(4 * f), y - (int)(2 * f), (int)(7 * f), y + (int)(21 * f) };
                HBRUSH lb = CreateSolidBrush(UI_COL_BAR);
                FillRect(dc, &lr, lb); DeleteObject(lb);
            }
            SetTextColor(dc, (li == g_pull_cur) ? UI_COL_TXT : UI_COL_TXTDIM);
            TextOutW(dc, (int)(UI_TEXT_X * f), y, sbuf, lstrlenW(sbuf));
        }
        SelectObject(dc, fS);
        {
            int yv = (int)((HDR_H + PULL_VIS * ROW_H + UI_HINT_GAP) * f);
            int av = (int)g_uiW - (int)(20 * f);
            int lh = (int)(UI_HINT_LH * f);
            int nl;
            SetTextColor(dc, UI_COL_HINT);
            nl = wrap_text(dc, L"Ctrl+\x2191\x2193=选择城池 · Ctrl+Enter=调拨至本城 · Ctrl+A=全部调拨", (int)(UI_HINT_X * f), yv, lh, av);
            wsprintfW(buf, L"(合计 %d 城)", g_pull_n);
            TextOutW(dc, (int)(UI_HINT_X * f), yv + nl * lh, buf, lstrlenW(buf));
            if (g_pull_msg[0]) {
                SetTextColor(dc, UI_COL_MSG);
                wrap_text(dc, g_pull_msg, (int)(UI_HINT_X * f), yv + (nl + 1) * lh, lh, av);
            }
        }
        SelectObject(dc, of);
        DeleteObject(fT); DeleteObject(fB); DeleteObject(fS);
        SelectObject(dc, ob);
        DeleteDC(dc);
        {
            unsigned char *p = (unsigned char *)g_bits;
            int n = (int)(g_uiW * g_uiH), kk;
            for (kk = 0; kk < n; kk++) p[kk * 4 + 3] = 255;
        }
        return;
    }

    if (g_pull_mode == PANEL_GENERAL) {
        int k;
        wsprintfW(buf, L"召集·武将→%s", g_pull_dstname);
        TextOutW(dc, (int)(10 * f), (int)(UI_TITLE_Y * f), buf, lstrlenW(buf));
        SelectObject(dc, fB);
        for (k = 0; k < PULL_VIS; k++) {
            int li = g_pull_top + k;
            int y  = (int)((HDR_H + k * ROW_H) * f);
            wchar_t sbuf[96];
            if (li < g_gen_n) {
                const wchar_t *gn = name_of(g_gen_num[li]);
                const wchar_t *gc = city_name_of(g_gen_city[li]);
                wsprintfW(sbuf, L"%s  %s", gn ? gn : L"?", gc ? gc : L"?");
            } else {
                lstrcpynW(sbuf, L"—", 96);
            }
            if (li == g_pull_cur && li < g_gen_n) {
                RECT hr = { (int)(4 * f), y - (int)(2 * f),
                            (LONG)g_uiW - (int)(5 * f), y + (int)(21 * f) };
                HBRUSH hb = CreateSolidBrush(UI_COL_ROWHL);
                FillRect(dc, &hr, hb); DeleteObject(hb);
                RECT lr = { (int)(4 * f), y - (int)(2 * f), (int)(7 * f), y + (int)(21 * f) };
                HBRUSH lb = CreateSolidBrush(UI_COL_BAR);
                FillRect(dc, &lr, lb); DeleteObject(lb);
            }
            SetTextColor(dc, (li == g_pull_cur) ? UI_COL_TXT : UI_COL_TXTDIM);
            TextOutW(dc, (int)(UI_TEXT_X * f), y, sbuf, lstrlenW(sbuf));
        }
        SelectObject(dc, fS);
        {
            int yv = (int)((HDR_H + PULL_VIS * ROW_H + UI_HINT_GAP) * f);
            int av = (int)g_uiW - (int)(20 * f);
            int lh = (int)(UI_HINT_LH * f);
            int nl;
            SetTextColor(dc, UI_COL_HINT);
            nl = wrap_text(dc, L"Ctrl+\x2191\x2193=选择武将 · Ctrl+Enter=召至本城 · Ctrl+A=全部召集", (int)(UI_HINT_X * f), yv, lh, av);
            wsprintfW(buf, L"(可召集 %d 人)", g_gen_n);
            TextOutW(dc, (int)(UI_HINT_X * f), yv + nl * lh, buf, lstrlenW(buf));
            if (g_pull_msg[0]) {
                SetTextColor(dc, UI_COL_MSG);
                wrap_text(dc, g_pull_msg, (int)(UI_HINT_X * f), yv + (nl + 1) * lh, lh, av);
            }
        }
        SelectObject(dc, of);
        DeleteObject(fT); DeleteObject(fB); DeleteObject(fS);
        SelectObject(dc, ob);
        DeleteDC(dc);
        {
            unsigned char *p = (unsigned char *)g_bits;
            int n = (int)(g_uiW * g_uiH), kk;
            for (kk = 0; kk < n; kk++) p[kk * 4 + 3] = 255;
        }
        return;
    }
    if (mode == MODE_CITY) {
        const wchar_t *cn = city_name_of(num);
        if (cn) wsprintfW(buf, L"城市 %s #%u", cn, num);
        else    wsprintfW(buf, L"城市 #%u", num);
    } else if (mode == MODE_FACTION) {
        wsprintfW(buf, L"势力 #%u", num);
    } else if (mode == MODE_CHAR) {
        const wchar_t *cn = name_of(num);
        if (cn) wsprintfW(buf, L"%s #%u", cn, num);
        else    wsprintfW(buf, L"武将 #%u", num);
    } else {
        wsprintfW(buf, L"—  打开详情页 —");
    }

    TextOutW(dc, (int)(10 * f), (int)(UI_TITLE_Y * f), buf, lstrlenW(buf));

    SelectObject(dc, fB);
    int i, rows = mode_rows(mode);
    if (g_cur >= rows) g_cur = rows - 1;
    if (g_cur < 0) g_cur = 0;

    unsigned int vals[8];
    if (mode == MODE_CITY)         get_city(num, vals);
    else if (mode == MODE_FACTION) get_faction(num, vals);
    else                           get_char(num, vals);

    for (i = 0; i < rows; i++) {
        int y = (int)((HDR_H + i * ROW_H) * f);

        int iscur = (mode != MODE_NONE) && (i == g_cur);
        const wchar_t *nm;
        nm = (mode == MODE_CITY) ? CNAME[i]
           : (mode == MODE_FACTION) ? FFNAME[i] : FNAME[i];

        if (iscur) {
            RECT hr = { (int)(4 * f), y - (int)(2 * f),
                        (LONG)g_uiW - (int)(5 * f), y + (int)(21 * f) };
            HBRUSH hb = CreateSolidBrush(UI_COL_ROWHL);
            FillRect(dc, &hr, hb); DeleteObject(hb);
            RECT lr = { (int)(4 * f), y - (int)(2 * f), (int)(7 * f), y + (int)(21 * f) };
            HBRUSH lb = CreateSolidBrush(mode_edge(mode));
            FillRect(dc, &lr, lb); DeleteObject(lb);
        }

        SetTextColor(dc, iscur ? UI_COL_TXT : UI_COL_TXTDIM);
        TextOutW(dc, (int)(UI_TEXT_X * f), y, nm, lstrlenW(nm));
        {

            SetTextColor(dc, iscur ? UI_COL_VALCUR : UI_COL_MSG);
            wsprintfW(buf, L"%u", vals[i]);
            SetTextAlign(dc, TA_RIGHT | TA_TOP);
            TextOutW(dc, (int)(108 * f), y, buf, lstrlenW(buf));
            SetTextAlign(dc, TA_LEFT | TA_TOP);
        }

        {
            COLORREF oc = SetTextColor(dc, UI_COL_STEPUP);
            TextOutW(dc, (int)(116 * f), y, L"+", 1);
            SetTextColor(dc, UI_COL_STEPDOWN);
            TextOutW(dc, (int)(132 * f), y, L"-", 1);
            SetTextColor(dc, oc);
        }
    }

    SelectObject(dc, fS);
    SetTextColor(dc, UI_COL_HINT);
    if (mode == MODE_NONE) {
        wsprintfW(buf, L"Ctrl+Enter=打开菜单 · Alt+Enter=隐藏面板 · Alt+\x2190\x2192=缩放");
    } else {
        wsprintfW(buf, L"Ctrl+\x2191\x2193=选择字段 · Ctrl+\x2190\x2192=增减数值 · Ctrl+A=拉满至上限");
    }
    {
        int av = (int)g_uiW - (int)(20 * f);
        int lh = (int)(UI_HINT_LH * f);
        int yv = (int)((HDR_H + rows * ROW_H + UI_HINT_GAP) * f);
        int nl = wrap_text(dc, buf, (int)(UI_HINT_X * f), yv, lh, av);
        if (g_pull_msg[0]) {
            SetTextColor(dc, UI_COL_MSG);
            wrap_text(dc, g_pull_msg, (int)(UI_HINT_X * f), yv + nl * lh, lh, av);
        }
    }

    SelectObject(dc, of);
    DeleteObject(fT); DeleteObject(fB); DeleteObject(fS);
    SelectObject(dc, ob);
    DeleteDC(dc);

    {
        unsigned char *p = (unsigned char *)g_bits;
        int n = (int)(g_uiW * g_uiH), k;
        for (k = 0; k < n; k++) p[k * 4 + 3] = 255;
    }
}

static void do_action(int act, int mode, int rows, unsigned int num);
static int nav_func_map(int act, int mode, int rows, unsigned int num)
{
    if (!(g_pull_mode == PANEL_FUNC || g_pull_mode == PANEL_MAP)) return 0;
        int snap = (g_pull_mode == PANEL_MAP);
        int nitems = snap ? 2 : 3;
        if (act == ACT_UP || act == ACT_DOWN) {
            g_pull_cur = (g_pull_cur + ((act == ACT_DOWN) ? 1 : -1) + nitems) % nitems;
            g_dirty = 1; return 1;
        }
        if (act == ACT_CONFIRM) {
            if (snap && g_pull_cur == 0) {
                g_pull_mode = PANEL_MECH; g_pull_cur = 0; g_pull_msg[0] = 0; g_dirty = 1;
                logf_("[mech] 进入机制菜单（来源=大地图，当前战死档=%s）", death_state_name(g_death_state));
            } else if (snap && g_pull_cur == 1) {
                g_pull_mode = PANEL_CHEAT; g_pull_cur = 0; g_pull_msg[0] = 0; g_dirty = 1;
                logf_("[cheat] 进入作弊菜单（来源=大地图）");
            } else if (!snap && g_pull_cur == 0) {
                open_convocation(mode, num);
            } else if (!snap && g_pull_cur == 1) {

                unsigned int mir = rd32(CITY_MIRROR);
                if (g_pull_dst < 1u) g_pull_dst = mir + 1u;
                city_name_copy(g_pull_dst, g_pull_dstname, 24);
                train_list_build();
                g_pull_mode = PANEL_TRAIN; g_dirty = 1;
                logf_("[train] 进入训练列表：本城=%u dst=%u", mir, g_pull_dst);
            } else if (!snap && g_pull_cur == 2) {
                deleg_open(mode, num);
                return 1;
            }
            return 1;
        }
        if (act == ACT_EXIT) { g_pull_mode = PANEL_OFF; g_dirty = 1; return 1; }

    return 0;
}

static int nav_mech(int act, int mode, int rows, unsigned int num)
{
    MechDesc its[8];
    int nm;
    if (!(g_pull_mode == PANEL_MECH)) return 0;
    nm = mech_all_items(its, 8);
    if (nm <= 0) return 1;
    return menu_nav(act, mode, rows, num, its, nm,
                    0, "[mech] 退出机制菜单 ⇒ 回大地图菜单");
}

static int nav_deleg(int act, int mode, int rows, unsigned int num)
{
    (void)rows;
    if (!(g_pull_mode == PANEL_DELEG)) return 0;
    if (act == ACT_UP)   { g_pull_cur = (g_pull_cur + DELEG_ITEMS) % (DELEG_ITEMS + 1); g_dirty = 1; return 1; }
    if (act == ACT_DOWN) { g_pull_cur = (g_pull_cur + 1) % (DELEG_ITEMS + 1); g_dirty = 1; return 1; }
    if (g_deleg[g_pull_dst] && g_pull_cur >= 1 && g_pull_cur <= DELEG_ITEMS &&
        (act == ACT_INC || act == ACT_DEC)) {
        int k = g_pull_cur - 1;
        int tier;
        if (city_is_gate(g_pull_dst) && (k == 0 || k == 1 || k == 2 || k == 4)) {
            wsprintfW(g_pull_msg, L"关卡无法设置此项目");
            g_dirty = 1;
            return 1;
        }
        tier = deleg_tier_of(g_pull_dst, k) + ((act == ACT_INC) ? 1 : -1);
        if (tier < 0) tier = 0;
        if (tier > 4) tier = 4;
        deleg_set_tier(g_pull_dst, k, tier);
        g_deleg_cfg[g_pull_dst] = 1;
        settings_save();
        g_dirty = 1;
        return 1;
    }
    if (act == ACT_CONFIRM || act == ACT_INC) {
        if (g_pull_cur == 0 || !g_deleg[g_pull_dst]) {
            deleg_toggle(g_pull_dst);
        } else {
            g_dirty = 1;
        }
        return 1;
    }
    if (act == ACT_DEC) { g_dirty = 1; return 1; }
    if (act == ACT_EXIT) {
        g_pull_mode = PANEL_FUNC; g_pull_cur = 2; g_pull_msg[0] = 0; g_dirty = 1;
        settings_save();
        logf_("[deleg] 退出委任面板 ⇒ 回城市菜单");
        return 1;
    }
    (void)mode; (void)num;
    return 0;
}

static int nav_cheat(int act, int mode, int rows, unsigned int num)
{
    if (!(g_pull_mode == PANEL_CHEAT)) return 0;
    return menu_nav(act, mode, rows, num, MECHS_CHEAT,
                    (int)(sizeof(MECHS_CHEAT)/sizeof(MECHS_CHEAT[0])),
                    1, "[cheat] 退出作弊菜单 ⇒ 回大地图菜单");
}

static int nav_train(int act, int mode, int rows, unsigned int num)
{
    if (!(g_pull_mode == PANEL_TRAIN)) return 0;
        if (act == ACT_CONFIRM || act == ACT_INC) { train_pick(g_pull_cur); return 1; }
        if (act == ACT_ALL) { batch_train_city(); return 1; }
        if (act == ACT_EXIT) {
            g_pull_mode = PANEL_FUNC; g_pull_cur = 0; g_pull_msg[0] = 0; g_dirty = 1;
            logf_("[train] 退出训练列表 ⇒ 回城市菜单");
            return 1;
        }
        if (act == ACT_UP || act == ACT_DOWN) {
            if (g_trn_n > 0) {
                g_pull_cur = (g_pull_cur + ((act == ACT_DOWN) ? 1 : -1) + g_trn_n) % g_trn_n;
                if (g_pull_cur < g_pull_top) g_pull_top = g_pull_cur;
                if (g_pull_cur >= g_pull_top + PULL_VIS) g_pull_top = g_pull_cur - PULL_VIS + 1;
            }
            g_dirty = 1; return 1;
        }

    return 0;
}

static int nav_main(int act, int mode, int rows, unsigned int num)
{
    if (!(g_pull_mode == PANEL_MAIN)) return 0;
        if (act == ACT_UP || act == ACT_DOWN) {
            g_pull_cur = (g_pull_cur + ((act == ACT_DOWN) ? 1 : -1) + 2) % 2;
            g_dirty = 1; return 1;
        }
        if (act == ACT_CONFIRM)        { g_pull_pick(g_pull_cur); return 1; }
        if (act == ACT_EXIT) {
            g_pull_mode = 0; g_dirty = 1;
            logf_("[pull] 退出召集（累计操作 %ld 笔）", g_pull_ops);
            return 1;
        }
        return 1;
    return 0;
}

static int nav_soldier(int act, int mode, int rows, unsigned int num)
{
    if (!(g_pull_mode == PANEL_SOLDIER)) return 0;
        if (act == ACT_ALL) { pull_all(); return 1; }
        if (act == ACT_CONFIRM) { pull_dump(g_pull_cur); return 1; }
        if (act == ACT_EXIT) {
            g_pull_mode = PANEL_MAIN; g_pull_cur = 0; g_pull_msg[0] = 0; g_dirty = 1;
            return 1;
        }
        if (act == ACT_UP || act == ACT_DOWN) {
            if (g_pull_n > 0) {
                g_pull_cur = (g_pull_cur + ((act == ACT_DOWN) ? 1 : -1) + g_pull_n) % g_pull_n;
                if (g_pull_cur < g_pull_top) g_pull_top = g_pull_cur;
                if (g_pull_cur >= g_pull_top + PULL_VIS) g_pull_top = g_pull_cur - PULL_VIS + 1;
            }
            g_dirty = 1; return 1;
        }
        if (act == ACT_INC) { pull_dump(g_pull_cur); return 1; }
        return 1;
    return 0;
}

static int nav_general(int act, int mode, int rows, unsigned int num)
{
    if (!(g_pull_mode == PANEL_GENERAL)) return 0;
        if (act == ACT_ALL) { pull_general_all(); return 1; }
        if (act == ACT_CONFIRM) { pull_general_take(g_pull_cur); return 1; }
        if (act == ACT_EXIT) {
            g_pull_mode = PANEL_MAIN; g_pull_cur = 0; g_pull_msg[0] = 0; g_dirty = 1;
            return 1;
        }
        if (act == ACT_UP || act == ACT_DOWN) {
            if (g_gen_n > 0) {
                g_pull_cur = (g_pull_cur + ((act == ACT_DOWN) ? 1 : -1) + g_gen_n) % g_gen_n;
                if (g_pull_cur < g_pull_top) g_pull_top = g_pull_cur;
                if (g_pull_cur >= g_pull_top + PULL_VIS) g_pull_top = g_pull_cur - PULL_VIS + 1;
            }
            g_dirty = 1; return 1;
        }
        if (act == ACT_INC) { pull_general_take(g_pull_cur); return 1; }
        return 1;
    return 0;
}

static const struct { int mode; int rows; } PANEL_ROWS[] = {
    { PANEL_MAP,     2 },
    { PANEL_FUNC,    3 },
    { PANEL_DELEG,   11 },
    { PANEL_MECH,    1 },
    { PANEL_CHEAT,   4 },
    { PANEL_MAIN,    2 },
    { PANEL_TRAIN,   PULL_VIS + 1 },
    { PANEL_SOLDIER, PULL_VIS + 1 },
    { PANEL_GENERAL, PULL_VIS + 1 },
};
static int panel_rows(int mode)
{
    int i;
    for (i = 0; i < (int)(sizeof(PANEL_ROWS)/sizeof(PANEL_ROWS[0])); i++)
        if (g_pull_mode == PANEL_ROWS[i].mode) return PANEL_ROWS[i].rows;
    return g_pull_mode ? PULL_VIS : mode_rows(mode);
}

static int nav_dispatch(int act, int mode, int rows, unsigned int num)
{
    int i;
    static int (* const NAVS[])(int,int,int,unsigned int) = { nav_func_map, nav_mech, nav_cheat, nav_train, nav_main, nav_soldier, nav_general, nav_deleg };
    for (i = 0; i < (int)(sizeof(NAVS)/sizeof(NAVS[0])); i++)
        if (NAVS[i](act, mode, rows, num)) return 1;
    return 0;
}

static void do_action(int act, int mode, int rows, unsigned int num)
{

    if (act == ACT_DIAG) {
        diag_dump_all();
        wsprintfW(g_pull_msg, L"诊断数据已保存（%d 段）", NDIAGS);
        g_dirty = 1;
        return;
    }

    if (act == ACT_OVIMG) {
        g_ovOn = !g_ovOn;
        logf_("[ovimg] Ctrl+P => %s", g_ovOn ? "显示覆盖" : "隐藏覆盖");
        settings_save();
        return;
    }

    if (nav_dispatch(act, mode, rows, num)) return;

    if (act == ACT_ALL && g_pull_mode == 0) act = ACT_MAXFIELD;
    switch (act) {
    case ACT_UP:
        if (rows > 0) { g_cur = (g_cur <= 0) ? rows - 1 : g_cur - 1; g_dirty = 1; }
        break;
    case ACT_DOWN:
        if (rows > 0) { g_cur = (g_cur + 1) % rows; g_dirty = 1; }
        break;
    case ACT_MAXFIELD:
        apply_field(-1, 0); break;
    case ACT_INC:  apply_field(-1, +1); break;
    case ACT_DEC:  apply_field(-1, -1); break;
    case ACT_CONFIRM:
        if (g_pull_mode == 0) {

            if (g_lastScene == SCENE_CITY_CMD) {
                g_pull_mode = PANEL_FUNC;
            } else if (g_lastScene == SCENE_OTHER && mode == MODE_NONE) {
                g_pull_mode = PANEL_MAP;
            } else {
                g_pull_mode = PANEL_OFF;
            }
            g_pull_cur = 0; g_pull_msg[0] = 0; g_dirty = 1;
            logf_("[menu] 回车开菜单：scene=%d mode=%d ⇒ %s", g_lastScene, mode,
                  (g_pull_mode == PANEL_FUNC) ? "召集"
                  : (g_pull_mode == PANEL_MAP) ? "菜单" : "不弹（非大地图/非城池）");
        }
        break;

    case ACT_EXIT:
        if (g_pull_mode != PANEL_OFF) { g_pull_mode = PANEL_OFF; g_dirty = 1; }
        break;
    case ACT_TOGGLE:   g_show = !g_show; g_dirty = 1; break;
    case ACT_ZOOM_OUT:
        g_scaleAdj *= 0.85f; if (g_scaleAdj < 0.4f) g_scaleAdj = 0.4f;
        recompute_layout(); break;
    case ACT_ZOOM_IN:
        g_scaleAdj *= 1.18f; if (g_scaleAdj > 5.0f) g_scaleAdj = 5.0f;
        recompute_layout(); break;

    case ACT_REFRESH:  do_refresh_action(mode, num); g_dirty = 1; return;

    case ACT_FULL:
        cheat_full_army(1, 0);
        return;
    case ACT_FULLTRN:
        cheat_full_army(0, 1);
        return;
    default: return;
    }
    InterlockedIncrement(&g_keyHits);
    logf_("[key] act=%d mode=%d num=%u cur=%d rows=%d hits=%ld",
          act, mode, num, g_cur, rows, (long)g_keyHits);
}

static void fps_window_log(void)
{
    DWORD now = GetTickCount();
    if (!g_fpsWin0) { g_fpsWin0 = now; return; }
    if (now - g_fpsWin0 < 2000) return;
    {
        unsigned long winMs = (unsigned long)(now - g_fpsWin0);
        double fps = (double)g_fpsN * 1000.0 / (double)winMs;
        unsigned long ovMs = (unsigned long)(g_qpcFreq.QuadPart
            ? (g_ovCost * 1000ULL / (ULONGLONG)g_qpcFreq.QuadPart) : 0);
        double pct = (winMs > 0) ? ((double)ovMs * 100.0 / (double)winMs) : 0.0;
        logf_("[fps] %.1ffps frames=%ld/%lums ov_cost=%lums(%.0f%%) blend=%ld/%ld switch=%s",
              fps, (long)g_fpsN, winMs, ovMs, pct, (long)g_blendN, (long)g_fpsN,
              (g_blendPath == 2 ? "gpu" : (g_blendPath == 1 ? "cpu" : "opaque")));
    }
    g_fpsWin0 = now; g_ovCost = 0; g_fpsN = 0; g_blendN = 0;
}

static volatile LONG g_hotAct    = ACT_NONE;

static volatile LONG g_kdown[256];

static volatile LONG g_repOk     = 0;

static volatile LONG g_holdAct   = 0;
static volatile LONG g_holdVK    = 0;
static volatile LONG  g_holdTick = 0;

static volatile LONG g_repFire   = 0;

static volatile LONG g_repStart  = 0;
static volatile LONG g_repLast   = 0;
static volatile LONG g_repBurst  = 0;

static HHOOK   g_mhook     = NULL;
static HHOOK   g_hook      = NULL;
static int     g_hookStart = 0;

static DWORD   g_lastDiag  = 0xFFFFFFFFu;

typedef struct { DWORD vk; int need_ctrl; int need_alt; int act; } KeyDef;
#define NKEYS 15
static const KeyDef KEYS[NKEYS] = {
    { VK_UP,     1,0, ACT_UP      },
    { VK_DOWN,   1,0, ACT_DOWN    },
    { VK_RIGHT,  1,0, ACT_INC     },
    { VK_LEFT,   1,0, ACT_DEC     },
    { 'A',       1,0, ACT_ALL     },
    { 'R',       1,0, ACT_REFRESH },
    { 'T',       1,0, ACT_FULLTRN },
    { 'S',       1,0, ACT_FULL    },
    { 'E',       1,0, ACT_EXIT    },
    { 'P',       1,0, ACT_OVIMG   },
    { VK_RETURN, 1,0, ACT_CONFIRM },
    { VK_RIGHT,  0,1, ACT_ZOOM_IN },
    { VK_LEFT,   0,1, ACT_ZOOM_OUT},
    { VK_RETURN, 0,1, ACT_TOGGLE  },
    { 'D',       1,0, ACT_DIAG    },
};
static int keys_lookup(DWORD vk, int ctrl, int alt)
{
    int i;
    for (i = 0; i < NKEYS; i++)
        if (KEYS[i].vk == vk && KEYS[i].need_ctrl == ctrl && KEYS[i].need_alt == alt)
            return KEYS[i].act;
    return ACT_NONE;
}
static int act_of(DWORD vk)
{
    int act = keys_lookup(vk, 1, 0);
    if (act == ACT_ALL && vk == 'A' && !g_pull_mode) return ACT_MAXFIELD;
    return act;
}

static int altact_of(DWORD vk)
{
    return keys_lookup(vk, 0, 1);
}

static LRESULT CALLBACK kb_proc(int code, WPARAM w, LPARAM l)
{
    if (code == HC_ACTION) {
        KBDLLHOOKSTRUCT *k = (KBDLLHOOKSTRUCT *)l;
        DWORD vk = k->vkCode;

        if (w == WM_KEYUP || w == WM_SYSKEYUP) {
            if (vk < 256) InterlockedExchange(&g_kdown[vk], 0);

            if ((vk == VK_LEFT || vk == VK_RIGHT) && vk == (DWORD)g_holdVK)
                InterlockedExchange(&g_holdAct, 0);
            return CallNextHookEx(NULL, code, w, l);
        }
        if (w == WM_KEYDOWN || w == WM_SYSKEYDOWN) {

            DWORD now = GetTickCount();
            int was = (vk < 256) ? (int)InterlockedExchange(&g_kdown[vk], 1) : 0;
            static DWORD tLast[256];
            int rep = ((k->flags & 0x40000000u) != 0) || was ||
                      (vk < 256 && now - tLast[vk] < 250);
            if (vk < 256) tLast[vk] = now;

            int act = alt_ok() ? altact_of(vk) : (ctl_ok() ? act_of(vk) : ACT_NONE);

            if (act == ACT_NONE && !alt_ok() &&
                (vk == VK_LEFT || vk == VK_RIGHT) &&
                g_holdAct && vk == (DWORD)g_holdVK &&
                (LONG)(now - g_holdTick) < 600) {
                act = (int)g_holdAct;
                rep = 1;
            }

            if (act != ACT_NONE) {

                if ((act == ACT_INC || act == ACT_DEC) &&
                    act_of(vk) == act && ctl_ok()) {
                    InterlockedExchange(&g_holdVK, (LONG)vk);
                    InterlockedExchange(&g_holdAct, act);
                    InterlockedExchange(&g_holdTick, (LONG)now);

                    InterlockedExchange(&g_repStart, (LONG)now);
                    InterlockedExchange(&g_repBurst, 0);
                } else if (g_holdAct && vk == (DWORD)g_holdVK &&
                           (LONG)(now - g_holdTick) < 600) {
                    InterlockedExchange(&g_holdTick, (LONG)now);
                }

                int allow = ((k->flags & 0x40000000u) == 0);
                if (allow) {
                    InterlockedExchange(&g_hotAct, act);
                    if (rep) InterlockedIncrement(&g_repFire);
                } else if (rep) {
                }
                return 1;
            }
        }
    }
    return CallNextHookEx(NULL, code, w, l);
}

static DWORD WINAPI kb_thread(LPVOID p)
{
    MSG m;
    (void)p;
    g_hook = SetWindowsHookExW(WH_KEYBOARD_LL, kb_proc, NULL, 0);
    g_mhook = SetWindowsHookExW(WH_MOUSE_LL, ms_proc, NULL, 0);
    logf_("[overlay] v52 kbd hook = %s", g_hook ? "ok" : "FAIL");
    logf_("[overlay] v52 mouse(wheel) hook = %s", g_mhook ? "ok" : "FAIL");
    {
        HANDLE hTimer;
        LARGE_INTEGER due;
        DWORD r;
        int heals = 0;
        due.QuadPart = -300000000LL;
        hTimer = CreateWaitableTimerW(NULL, FALSE, NULL);
        if (hTimer) SetWaitableTimer(hTimer, &due, 30000, NULL, NULL, FALSE);
        logf_("[overlay] v53 self-heal timer = %s", hTimer ? "ok" : "FAIL");
        for (;;) {
            if (!hTimer) {
                if (!GetMessageW(&m, NULL, 0, 0)) break;
                TranslateMessage(&m); DispatchMessageW(&m);
                continue;
            }
            r = MsgWaitForMultipleObjects(1, &hTimer, FALSE, INFINITE, QS_ALLINPUT);
            if (r == WAIT_OBJECT_0) {
                if (g_hook)  { UnhookWindowsHookEx(g_hook);  g_hook  = NULL; }
                if (g_mhook) { UnhookWindowsHookEx(g_mhook); g_mhook = NULL; }
                g_hook  = SetWindowsHookExW(WH_KEYBOARD_LL, kb_proc, NULL, 0);
                g_mhook = SetWindowsHookExW(WH_MOUSE_LL, ms_proc, NULL, 0);
                heals++;
                if (heals == 1 || (heals % 20) == 0)
                    logf_("[overlay] v53 hook heal #%d kbd=%s mouse=%s",
                          heals, g_hook ? "ok" : "FAIL", g_mhook ? "ok" : "FAIL");
                continue;
            }
            while (PeekMessageW(&m, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&m); DispatchMessageW(&m);
            }
        }
        if (hTimer) { CancelWaitableTimer(hTimer); CloseHandle(hTimer); }
    }
    return 0;
}

static void poll_hotkeys(void)
{
    DWORD now = GetTickCount();
    unsigned int num = 0;
    int i, mode, rows, alt, ctrl;
    if (now - g_hkLast < 150) return;
    mode = detect_mode(&num);
    rows = mode_rows(mode);
    alt = alt_ok();
    if (!alt) {
        ctrl = ctl_ok();
        if (!ctrl) return;
    }
    for (i = 0; i < NKEYS; i++) {
        if (alt) { if (!KEYS[i].need_alt) continue; }
        else     { if (!KEYS[i].need_ctrl) continue; }
        if (GetAsyncKeyState((int)KEYS[i].vk) & 0x8000) {
            g_hkLast = now;
            do_action(KEYS[i].act, mode, rows, num);
            return;
        }
    }
}

static void handle_wheel(void)
{
    LONG d = InterlockedExchange(&g_wheelDir, 0);
    unsigned int num = 0;
    int mode, rows, i, rel;
    float f;
    int x, y;

    if (d == 0 || !g_show) return;
    if (g_pull_mode != 0) return;

    f = g_scale * g_scaleAdj;
    x = (int)InterlockedCompareExchange(&g_wheelX, 0, 0) - g_posX;
    y = (int)InterlockedCompareExchange(&g_wheelY, 0, 0) - g_posY;
    if (x < 0 || x >= (int)g_uiW || y < 0 || y >= (int)g_uiH) return;

    rel = y - (int)(HDR_H * f);
    if (rel < 0) return;
    i = rel / (int)(ROW_H * f);
    if (i < 0) return;

    mode = detect_mode(&num);
    rows = mode_rows(mode);
    if (mode == MODE_NONE || i >= rows) return;

    g_cur += (d > 0 ? -1 : 1);
    if (g_cur < 0) g_cur = 0;
    if (g_cur >= rows) g_cur = rows - 1;
    g_dirty = 1;
    logf_("[wheel] 光标 -> %d（rows=%d；⛔未改数值）", g_cur, rows);
}

static void handle_hotkeys(void)
{
    LONG act;
    handle_wheel();
    act = InterlockedExchange(&g_hotAct, ACT_NONE);
    {
        unsigned int num = 0;
        int mode = detect_mode(&num);

        InterlockedExchange(&g_repOk,
            (g_pull_mode == 0 && mode != MODE_NONE && g_cheat_on) ? 1 : 0);

        {
            static DWORD tDiag = 0;
            DWORD now = GetTickCount();
            int kdR = (int)g_kdown[VK_RIGHT], kdL = (int)g_kdown[VK_LEFT];
            if (now - tDiag >= 500 && (kdR || kdL || g_repBurst > 0)) {
                tDiag = now;
                logf_("[rep] kdR=%d kdL=%d latch=%d/%d repOk=%d fire=%ld burst=%ld start=%d",
                      kdR, kdL, (int)g_holdAct, (int)g_holdVK, (int)g_repOk,
                      (long)g_repFire, (long)g_repBurst, g_repStart ? 1 : 0);
            }
        }
        if (act != ACT_NONE) {
            do_action((int)act, mode, mode_rows(mode), num);
            return;
        }

        {
            DWORD now = GetTickCount();
            LONG hAct = g_holdAct;
            int    hVk = (int)g_holdVK;
            if (g_repOk && hAct && hVk < 256 && g_kdown[hVk]) {
                LONG st = g_repStart;
                if (st == 0) InterlockedExchange(&g_repStart, (LONG)now);
                else if ((LONG)(now - st) >= 450) {
                    LONG last = g_repLast;
                    if (last == 0 || (LONG)(now - last) >= 90) {
                        if (g_repBurst < 400) {
                            InterlockedExchange(&g_repLast, (LONG)now);
                            InterlockedIncrement(&g_repBurst);
                            InterlockedIncrement(&g_repFire);
                            do_action((int)hAct, mode, mode_rows(mode), num);
                            return;
                        }
                    }
                }
            } else {

                InterlockedExchange(&g_repStart, 0);
                InterlockedExchange(&g_repLast, 0);
                InterlockedExchange(&g_repBurst, 0);
            }
        }
    }
    if (!g_hook) poll_hotkeys();
}

static PresentFn  g_origPresent  = NULL;
static Present1Fn g_origPresent1 = NULL;

static ID3D11Device        *g_dev   = NULL;
static ID3D11DeviceContext *g_ctx   = NULL;
static ID3D11Texture2D     *g_uiTex = NULL;
static UINT g_texW = 0, g_texH = 0;
static DXGI_FORMAT g_texFmt = DXGI_FORMAT_UNKNOWN;

static ID3D11Texture2D *g_stgTex = NULL;
static ID3D11Texture2D *g_uploadTex = NULL;
static UINT g_uploadW = 0, g_uploadH = 0;
static DXGI_FORMAT g_uploadFmt = (DXGI_FORMAT)0;
static int  g_uploadDirty = 0;
static UINT g_stgW = 0, g_stgH = 0;
static DXGI_FORMAT g_stgFmt = DXGI_FORMAT_UNKNOWN;

static int   g_gpuTried = 0, g_gpuBad = 0;
static void  gb_ensure(void);
static ID3D11VertexShader       *g_gpuVs   = NULL;
static ID3D11PixelShader        *g_gpuPs   = NULL;
static ID3D11InputLayout        *g_gpuLay  = NULL;
static ID3D11Buffer             *g_gpuVb   = NULL;
static ID3D11BlendState         *g_gpuBlend = NULL;
static ID3D11DepthStencilState  *g_gpuNoDepth = NULL;
static ID3D11SamplerState       *g_gpuSamp = NULL;
static ID3D11RasterizerState    *g_gpuRs = NULL;
static ID3D11ShaderResourceView *g_gpuSrv  = NULL;
static ID3D11Texture2D          *g_gpuSrvTex = NULL;
static UINT g_gpuSrvW = 0, g_gpuSrvH = 0;

static BYTE  *g_ovBuf   = NULL;
static UINT   g_ovBufW = 0, g_ovBufH = 0;
static ID3D11Texture2D *g_ovDstTex = NULL;
static UINT g_ovDstW = 0, g_ovDstH = 0;
static DXGI_FORMAT g_ovDstFmt = DXGI_FORMAT_UNKNOWN;
static char   g_ovPath[MAX_PATH] = { 0 };
static char   g_ovDir[MAX_PATH]  = { 0 };
static DWORD  g_ovSize  = 0;
static FILETIME g_ovMtime = { 0, 0 };
static int    g_ovHave  = 0;

static int    g_ovFileOn = 1;
static int    g_ovDirty = 0;
static int    g_ovReady = 0;
static DWORD  g_ovNextPoll = 0;

static int ovimg_load(void)
{
    HANDLE f; DWORD sz, rd = 0; BYTE *raw = NULL, *pix;
    BITMAPINFOHEADER *bh;
    int w, h, H, topdown, sy;
    UINT bpp, stride, off, need;
    BYTE *nb;
    f = CreateFileA(g_ovPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (f == INVALID_HANDLE_VALUE) return 0;
    sz = GetFileSize(f, NULL);
    if (sz < 60 || sz > 32u * 1024u * 1024u) { logf_("[ovimg] 文件大小异常 %lu", (unsigned long)sz); CloseHandle(f); return 0; }
    raw = (BYTE *)malloc(sz);
    if (!raw) { CloseHandle(f); return 0; }
    if (!ReadFile(f, raw, sz, &rd, NULL) || rd != sz) { free(raw); CloseHandle(f); return 0; }
    CloseHandle(f);

    if (raw[0] != 'B' || raw[1] != 'M') { logf_("[ovimg] 非 BM 魔数"); free(raw); return 0; }
    bh = (BITMAPINFOHEADER *)(raw + 14);
    if (bh->biSize != 40)      { logf_("[ovimg] biSize=%lu(仅支持 BITMAPINFOHEADER)", (unsigned long)bh->biSize); free(raw); return 0; }
    if (bh->biCompression != 0){ logf_("[ovimg] 压缩位=%lu(仅支持 BI_RGB)", (unsigned long)bh->biCompression); free(raw); return 0; }
    w = (int)bh->biWidth;
    h = (int)bh->biHeight;
    bpp = (UINT)bh->biBitCount;
    if (bpp != 24 && bpp != 32) { logf_("[ovimg] 位深=%lu(仅支持 24/32)", (unsigned long)bpp); free(raw); return 0; }
    if (h == 0) { free(raw); return 0; }
    H = h < 0 ? -h : h; topdown = (h < 0);
    if (w <= 0 || w > OVW_MAX || H > OVH_MAX) {
        logf_("[ovimg] 尺寸越界 %dx%d(上限 %dx%d)", w, H, OVW_MAX, OVH_MAX); free(raw); return 0;
    }
    off = *(UINT *)(raw + 10);
    if (off < 54 || off > sz) { logf_("[ovimg] bfOffBits=%lu 异常", (unsigned long)off); free(raw); return 0; }
    stride = ((UINT)w * (bpp / 8u) + 3u) & ~3u;
    need = off + stride * (UINT)H;
    if (need > sz) { logf_("[ovimg] 数据截断 need=%lu size=%lu", (unsigned long)need, (unsigned long)sz); free(raw); return 0; }
    pix = raw + off;
    nb = (BYTE *)malloc((size_t)w * (size_t)H * 4u);
    if (!nb) { free(raw); return 0; }
    for (sy = 0; sy < H; sy++) {
        BYTE *srow = pix + (size_t)sy * stride;
        BYTE *drow = nb + (size_t)(topdown ? sy : (H - 1 - sy)) * (size_t)w * 4u;
        if (bpp == 24) {
            int x;
            for (x = 0; x < w; x++) {
                drow[x * 4 + 0] = srow[x * 3 + 0];
                drow[x * 4 + 1] = srow[x * 3 + 1];
                drow[x * 4 + 2] = srow[x * 3 + 2];
                drow[x * 4 + 3] = 255;
            }
        } else {
            memcpy(drow, srow, (size_t)w * 4u);
        }
    }
    if (g_ovBuf) free(g_ovBuf);
    g_ovBuf = nb; g_ovBufW = (UINT)w; g_ovBufH = (UINT)H;
    g_ovDirty = 1; g_ovHave = 1;
    free(raw);
    return 1;
}

static void ovimg_poll(DWORD now)
{
    HANDLE f;
    WIN32_FILE_ATTRIBUTE_DATA fa;
    int isNew;
    if ((int)(now - g_ovNextPoll) < 0) return;
    g_ovNextPoll = now + 250u;
    if (!g_ovReady) return;

    {
        char p[MAX_PATH];
        wsprintfA(p, "%s\\OFF", g_ovDir);
        f = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        g_ovFileOn = (f == INVALID_HANDLE_VALUE) ? 1 : 0;
        if (f != INVALID_HANDLE_VALUE) CloseHandle(f);
    }

    if (!GetFileAttributesExA(g_ovPath, GetFileExInfoStandard, &fa)) {
        if (g_ovHave) { logf_("[ovimg] SCREEN.BMP 消失 ⇒ 清除覆盖"); g_ovHave = 0; }
        return;
    }
    isNew = (!g_ovHave || fa.nFileSizeLow != g_ovSize ||
             fa.ftLastWriteTime.dwLowDateTime  != g_ovMtime.dwLowDateTime ||
             fa.ftLastWriteTime.dwHighDateTime != g_ovMtime.dwHighDateTime);
    if (!isNew) return;
    g_ovSize = fa.nFileSizeLow;
    g_ovMtime = fa.ftLastWriteTime;
    if (ovimg_load())
        logf_("[ovimg] 新帧 %ux%u (%lu bytes)", g_ovBufW, g_ovBufH, (unsigned long)fa.nFileSizeLow);

}

static void ovimg_ensure_tex(D3D11_TEXTURE2D_DESC *d)
{
    if (g_ovDstTex && g_ovDstW == d->Width && g_ovDstH == d->Height && g_ovDstFmt == d->Format) return;
    if (g_ovDstTex) { ID3D11Texture2D_Release(g_ovDstTex); g_ovDstTex = NULL; }
    {
        D3D11_TEXTURE2D_DESC td;
        ZeroMemory(&td, sizeof(td));
        td.Width = d->Width; td.Height = d->Height;
        td.MipLevels = 1; td.ArraySize = 1; td.Format = d->Format;
        td.SampleDesc.Count = 1;
        td.Usage = D3D11_USAGE_DYNAMIC;
        td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        if (FAILED(ID3D11Device_CreateTexture2D(g_dev, &td, NULL, &g_ovDstTex)) || !g_ovDstTex) {
            g_ovDstTex = NULL;
            logf_("[ovimg] 目标纹理创建失败 %ux%u", d->Width, d->Height);
            return;
        }
        g_ovDstW = d->Width; g_ovDstH = d->Height; g_ovDstFmt = d->Format;
        g_ovDirty = 1;
    }
}

static void ovimg_upload(D3D11_TEXTURE2D_DESC *d)
{
    D3D11_MAPPED_SUBRESOURCE ms;
    int swapRB;
    UINT y;
    if (!g_ovDstTex || !g_ovBuf || !g_ovHave) return;
    if (FAILED(ID3D11DeviceContext_Map(g_ctx, (ID3D11Resource *)g_ovDstTex, 0,
                                       D3D11_MAP_WRITE_DISCARD, 0, &ms))) return;
    swapRB = (d->Format == DXGI_FORMAT_R8G8B8A8_UNORM ||
              d->Format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
    {
        UINT sw = g_ovBufW, sh = g_ovBufH;
        UINT dw = d->Width, dh = d->Height;
        for (y = 0; y < dh; y++) {
            UINT sy = (UINT)(((unsigned long long)y * sh) / dh);
            BYTE *s = g_ovBuf + (size_t)sy * sw * 4u;
            BYTE *t = (BYTE *)ms.pData + (size_t)y * ms.RowPitch;
            UINT x;
            if (swapRB) {
                for (x = 0; x < dw; x++) {
                    UINT sx = (UINT)(((unsigned long long)x * sw) / dw);
                    BYTE *p = s + sx * 4u;
                    t[x * 4 + 0] = p[2];
                    t[x * 4 + 1] = p[1];
                    t[x * 4 + 2] = p[0];
                    t[x * 4 + 3] = 255;
                }
            } else {
                for (x = 0; x < dw; x++) {
                    UINT sx = (UINT)(((unsigned long long)x * sw) / dw);
                    memcpy(t + x * 4u, s + sx * 4u, 4u);
                }
            }
        }
    }
    ID3D11DeviceContext_Unmap(g_ctx, (ID3D11Resource *)g_ovDstTex, 0);
    g_ovDirty = 0;
}

static void ovimg_init(void)
{
    char dir[MAX_PATH];
    g_ovPath[0] = 0; g_ovDir[0] = 0;
    if (!mod_dir(dir)) return;
    wsprintfA(g_ovDir, "%s\\%s", dir, OV_DIR);
    CreateDirectoryA(g_ovDir, NULL);
    wsprintfA(g_ovPath, "%s\\%s", g_ovDir, OV_FILE);
    g_ovReady = 1;
    logf_("[ovimg] channel ready: %s", g_ovPath);
}

static void ovimg_draw(ID3D11Texture2D *bb, D3D11_TEXTURE2D_DESC *d)
{
    if (!g_ovHave || !g_ovFileOn || !g_ovOn) return;
    ovimg_ensure_tex(d);
    if (!g_ovDstTex) return;
    if (g_ovDirty) ovimg_upload(d);
    ID3D11DeviceContext_CopySubresourceRegion(g_ctx, (ID3D11Resource *)bb, 0, 0, 0, 0,
                                              (ID3D11Resource *)g_ovDstTex, 0, NULL);
}

static const unsigned char g_dxbc_vs[] = {
    68,88,66,67,242,9,168,215,19,221,238,17,
    56,228,89,53,98,123,140,173,1,0,0,0,
    52,2,0,0,5,0,0,0,52,0,0,0,
    128,0,0,0,180,0,0,0,12,1,0,0,
    184,1,0,0,82,68,69,70,68,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    28,0,0,0,0,4,254,255,0,129,0,0,
    28,0,0,0,77,105,99,114,111,115,111,102,
    116,32,40,82,41,32,72,76,83,76,32,83,
    104,97,100,101,114,32,67,111,109,112,105,108,
    101,114,32,49,48,46,49,0,73,83,71,78,
    44,0,0,0,1,0,0,0,8,0,0,0,
    32,0,0,0,0,0,0,0,0,0,0,0,
    3,0,0,0,0,0,0,0,3,3,0,0,
    80,79,83,73,84,73,79,78,0,171,171,171,
    79,83,71,78,80,0,0,0,2,0,0,0,
    8,0,0,0,56,0,0,0,0,0,0,0,
    1,0,0,0,3,0,0,0,0,0,0,0,
    15,0,0,0,68,0,0,0,0,0,0,0,
    0,0,0,0,3,0,0,0,1,0,0,0,
    3,12,0,0,83,86,95,80,79,83,73,84,
    73,79,78,0,84,69,88,67,79,79,82,68,
    0,171,171,171,83,72,68,82,164,0,0,0,
    64,0,1,0,41,0,0,0,95,0,0,3,
    50,16,16,0,0,0,0,0,103,0,0,4,
    242,32,16,0,0,0,0,0,1,0,0,0,
    101,0,0,3,50,32,16,0,1,0,0,0,
    54,0,0,5,50,32,16,0,0,0,0,0,
    70,16,16,0,0,0,0,0,54,0,0,8,
    194,32,16,0,0,0,0,0,2,64,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,128,63,50,0,0,15,50,32,16,0,
    1,0,0,0,70,16,16,0,0,0,0,0,
    2,64,0,0,0,0,0,63,0,0,0,191,
    0,0,0,0,0,0,0,0,2,64,0,0,
    0,0,0,63,0,0,0,63,0,0,0,0,
    0,0,0,0,62,0,0,1,83,84,65,84,
    116,0,0,0,4,0,0,0,0,0,0,0,
    0,0,0,0,3,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,2,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
};

static const unsigned char g_dxbc_ps[] = {
    68,88,66,67,191,208,35,146,136,175,224,45,
    178,165,245,252,184,22,65,81,1,0,0,0,
    112,2,0,0,5,0,0,0,52,0,0,0,
    196,0,0,0,28,1,0,0,80,1,0,0,
    244,1,0,0,82,68,69,70,136,0,0,0,
    0,0,0,0,0,0,0,0,2,0,0,0,
    28,0,0,0,0,4,255,255,0,129,0,0,
    96,0,0,0,92,0,0,0,3,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,1,0,0,0,
    94,0,0,0,2,0,0,0,5,0,0,0,
    4,0,0,0,255,255,255,255,0,0,0,0,
    1,0,0,0,13,0,0,0,115,0,116,0,
    77,105,99,114,111,115,111,102,116,32,40,82,
    41,32,72,76,83,76,32,83,104,97,100,101,
    114,32,67,111,109,112,105,108,101,114,32,49,
    48,46,49,0,73,83,71,78,80,0,0,0,
    2,0,0,0,8,0,0,0,56,0,0,0,
    0,0,0,0,1,0,0,0,3,0,0,0,
    0,0,0,0,15,0,0,0,68,0,0,0,
    0,0,0,0,0,0,0,0,3,0,0,0,
    1,0,0,0,3,3,0,0,83,86,95,80,
    79,83,73,84,73,79,78,0,84,69,88,67,
    79,79,82,68,0,171,171,171,79,83,71,78,
    44,0,0,0,1,0,0,0,8,0,0,0,
    32,0,0,0,0,0,0,0,0,0,0,0,
    3,0,0,0,0,0,0,0,15,0,0,0,
    83,86,95,84,65,82,71,69,84,0,171,171,
    83,72,68,82,156,0,0,0,64,0,0,0,
    39,0,0,0,90,0,0,3,0,96,16,0,
    0,0,0,0,88,24,0,4,0,112,16,0,
    0,0,0,0,85,85,0,0,98,16,0,3,
    50,16,16,0,1,0,0,0,101,0,0,3,
    242,32,16,0,0,0,0,0,104,0,0,2,
    1,0,0,0,72,0,0,11,242,0,16,0,
    0,0,0,0,70,16,16,0,1,0,0,0,
    70,126,16,0,0,0,0,0,0,96,16,0,
    0,0,0,0,1,64,0,0,0,0,0,0,
    54,0,0,5,114,32,16,0,0,0,0,0,
    70,2,16,0,0,0,0,0,54,0,0,5,
    130,32,16,0,0,0,0,0,1,64,0,0,
    176,170,42,63,62,0,0,1,83,84,65,84,
    116,0,0,0,4,0,0,0,1,0,0,0,
    0,0,0,0,2,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,2,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
};

static void gb_ensure(void)
{
    D3D11_INPUT_ELEMENT_DESC il;
    D3D11_BUFFER_DESC bd; D3D11_SUBRESOURCE_DATA si;
    D3D11_BLEND_DESC bld; D3D11_DEPTH_STENCIL_DESC dsd; D3D11_SAMPLER_DESC spd;
    D3D11_RASTERIZER_DESC rsd;
    static const float quad[8] = { -1.f,-1.f, 1.f,-1.f, -1.f,1.f, 1.f,1.f };
    HRESULT ok = S_OK;
    UINT i;
    if (g_gpuTried) return;
    g_gpuTried = 1;
    if (ID3D11Device_GetFeatureLevel(g_dev) < D3D_FEATURE_LEVEL_10_0) {
        logf_("[gpu] 特性级别不足 ⇒ GPU 混合不可用，永久 CPU 混合");
        g_gpuBad = 1; return;
    }
    ZeroMemory(&il, sizeof(il));
    il.SemanticName = "POSITION"; il.SemanticIndex = 0;
    il.Format = DXGI_FORMAT_R32G32_FLOAT; il.InputSlot = 0;
    il.AlignedByteOffset = 0; il.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    il.InstanceDataStepRate = 0;
    ok = ID3D11Device_CreateInputLayout(g_dev, &il, 1, g_dxbc_vs, DXBC_VS_LEN, &g_gpuLay);
    if (SUCCEEDED(ok)) ok = ID3D11Device_CreateVertexShader(g_dev, g_dxbc_vs, DXBC_VS_LEN, NULL, &g_gpuVs);
    if (SUCCEEDED(ok)) ok = ID3D11Device_CreatePixelShader(g_dev, g_dxbc_ps, DXBC_PS_LEN, NULL, &g_gpuPs);
    if (SUCCEEDED(ok)) {
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = sizeof(quad); bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        ZeroMemory(&si, sizeof(si)); si.pSysMem = quad;
        ok = ID3D11Device_CreateBuffer(g_dev, &bd, &si, &g_gpuVb);
    }
    if (SUCCEEDED(ok)) {
        ZeroMemory(&bld, sizeof(bld));
        bld.AlphaToCoverageEnable = FALSE;
        bld.IndependentBlendEnable = FALSE;
        for (i = 0; i < 8; i++) {
            bld.RenderTarget[i].BlendEnable = TRUE;
            bld.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            bld.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            bld.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
            bld.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
            bld.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
            bld.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            bld.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        }
        ok = ID3D11Device_CreateBlendState(g_dev, &bld, &g_gpuBlend);
    }
    if (SUCCEEDED(ok)) {
        ZeroMemory(&dsd, sizeof(dsd));
        dsd.DepthEnable = FALSE; dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        dsd.DepthFunc = D3D11_COMPARISON_ALWAYS; dsd.StencilEnable = FALSE;
        ok = ID3D11Device_CreateDepthStencilState(g_dev, &dsd, &g_gpuNoDepth);
    }
    if (SUCCEEDED(ok)) {
        ZeroMemory(&spd, sizeof(spd));
        spd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        spd.AddressU = spd.AddressV = spd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        spd.MaxLOD = D3D11_FLOAT32_MAX;
        ok = ID3D11Device_CreateSamplerState(g_dev, &spd, &g_gpuSamp);
    }
    if (SUCCEEDED(ok)) {

        ZeroMemory(&rsd, sizeof(rsd));
        rsd.FillMode = D3D11_FILL_SOLID;
        rsd.CullMode = D3D11_CULL_NONE;
        rsd.FrontCounterClockwise = FALSE;
        rsd.DepthClipEnable = TRUE; rsd.ScissorEnable = TRUE;
        ok = ID3D11Device_CreateRasterizerState(g_dev, &rsd, &g_gpuRs);
    }
    if (FAILED(ok) || !g_gpuVs || !g_gpuPs || !g_gpuLay || !g_gpuVb ||
        !g_gpuBlend || !g_gpuNoDepth || !g_gpuSamp || !g_gpuRs) {
        logf_("[gpu] 资源创建失败 hr=0x%08lX ⇒ GPU 路封死（保持 CPU 混合）", (unsigned long)ok);
        g_gpuBad = 1;
    } else {
        logf_("[gpu] DXBC 资源就绪 vs=%p ps=%p vb=%p（A=%u 已烘进字节码）",
              (void *)g_gpuVs, (void *)g_gpuPs, (void *)g_gpuVb, (unsigned)PANEL_A);
    }
}

static int gb_blend(ID3D11Texture2D *bb)
{
    ID3D11DeviceContext *ctx = g_ctx;
    ID3D11RenderTargetView *rtv = NULL;
    ID3D11ShaderResourceView *svSrv[16];
    ID3D11SamplerState *svSmp[16];

    ID3D11RenderTargetView *svRtv = NULL; ID3D11DepthStencilView *svDsv = NULL;
    ID3D11BlendState *svBlend = NULL; FLOAT svBf[4] = {0,0,0,0}; UINT svMask = 0;
    ID3D11DepthStencilState *svDs = NULL; UINT svRef = 0;
    ID3D11RasterizerState *svRs = NULL;
    ID3D11VertexShader *svVs = NULL; ID3D11GeometryShader *svGs = NULL;
    ID3D11PixelShader *svPs = NULL; ID3D11HullShader *svHs = NULL;
    ID3D11DomainShader *svDm = NULL; ID3D11InputLayout *svLay = NULL;
    ID3D11Buffer *svVb0 = NULL; ID3D11Buffer *svIb = NULL;
    UINT svStride = 0, svOff = 0, svIbOff = 0;
    DXGI_FORMAT svIbFmt = DXGI_FORMAT_UNKNOWN;
    D3D11_PRIMITIVE_TOPOLOGY svTop = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    D3D11_VIEWPORT svVp[1]; UINT svNvp = 0;
    D3D11_RECT svSr[1]; UINT svNsr = 0;
    HRESULT hr;
    UINT k;
    if (!g_gpuTried) gb_ensure();
    if (g_gpuBad) return 0;
    if (ID3D11Device_GetFeatureLevel(g_dev) < D3D_FEATURE_LEVEL_10_0) return 0;

    if (!g_gpuSrv || g_gpuSrvTex != g_uiTex || g_gpuSrvW != g_uiW || g_gpuSrvH != g_uiH) {
        if (g_gpuSrv) { ID3D11ShaderResourceView_Release(g_gpuSrv); g_gpuSrv = NULL; g_gpuSrvTex = NULL; }
        hr = ID3D11Device_CreateShaderResourceView(g_dev, (ID3D11Resource *)g_uiTex, NULL, &g_gpuSrv);
        if (FAILED(hr) || !g_gpuSrv) { logf_("[gpu] SRV 创建失败 hr=0x%08lX", (unsigned long)hr); return 0; }
        g_gpuSrvTex = g_uiTex; g_gpuSrvW = g_uiW; g_gpuSrvH = g_uiH;
    }
    hr = ID3D11Device_CreateRenderTargetView(g_dev, (ID3D11Resource *)bb, NULL, &rtv);
    if (FAILED(hr) || !rtv) { logf_("[gpu] RTV 创建失败 hr=0x%08lX", (unsigned long)hr); return 0; }

    ID3D11DeviceContext_OMGetRenderTargets(ctx, 1, &svRtv, &svDsv);
    ID3D11DeviceContext_OMGetBlendState(ctx, &svBlend, svBf, &svMask);
    ID3D11DeviceContext_OMGetDepthStencilState(ctx, &svDs, &svRef);
    ID3D11DeviceContext_RSGetState(ctx, &svRs);
    ID3D11DeviceContext_RSGetViewports(ctx, &svNvp, NULL);
    if (svNvp > 1) svNvp = 1;
    ID3D11DeviceContext_RSGetViewports(ctx, &svNvp, svVp);
    ID3D11DeviceContext_RSGetScissorRects(ctx, &svNsr, NULL);
    if (svNsr > 1) svNsr = 1;
    ID3D11DeviceContext_RSGetScissorRects(ctx, &svNsr, svSr);
    ID3D11DeviceContext_VSGetShader(ctx, &svVs, NULL, NULL);
    ID3D11DeviceContext_GSGetShader(ctx, &svGs, NULL, NULL);
    ID3D11DeviceContext_PSGetShader(ctx, &svPs, NULL, NULL);
    ID3D11DeviceContext_HSGetShader(ctx, &svHs, NULL, NULL);
    ID3D11DeviceContext_DSGetShader(ctx, &svDm, NULL, NULL);
    ID3D11DeviceContext_IAGetInputLayout(ctx, &svLay);
    ID3D11DeviceContext_IAGetVertexBuffers(ctx, 0, 1, &svVb0, &svStride, &svOff);
    ID3D11DeviceContext_IAGetIndexBuffer(ctx, &svIb, &svIbFmt, &svIbOff);
    ID3D11DeviceContext_IAGetPrimitiveTopology(ctx, &svTop);
    for (k = 0; k < 16; k++) { svSrv[k] = NULL; svSmp[k] = NULL; }
    ID3D11DeviceContext_PSGetShaderResources(ctx, 0, 16, svSrv);
    ID3D11DeviceContext_PSGetSamplers(ctx, 0, 16, svSmp);

    {
        D3D11_VIEWPORT vp; D3D11_RECT sr;
        ID3D11Buffer *vbs[1]; ID3D11ShaderResourceView *useSrv; ID3D11SamplerState *useSmp[1];
        UINT stride = 8, off = 0;
        FLOAT bf[4] = { 0.f, 0.f, 0.f, 0.f };
        vp.TopLeftX = (float)g_posX; vp.TopLeftY = (float)g_posY;
        vp.Width = (float)g_uiW; vp.Height = (float)g_uiH;
        vp.MinDepth = 0.f; vp.MaxDepth = 1.f;
        sr.left = (LONG)g_posX; sr.top = (LONG)g_posY;
        sr.right = (LONG)g_posX + (LONG)g_uiW; sr.bottom = (LONG)g_posY + (LONG)g_uiH;
        useSrv = g_gpuSrv; useSmp[0] = g_gpuSamp; vbs[0] = g_gpuVb;
        ID3D11DeviceContext_OMSetRenderTargets(ctx, 1, &rtv, NULL);
        ID3D11DeviceContext_OMSetBlendState(ctx, g_gpuBlend, bf, 0xFFFFFFFFu);
        ID3D11DeviceContext_OMSetDepthStencilState(ctx, g_gpuNoDepth, 0);
        ID3D11DeviceContext_RSSetState(ctx, g_gpuRs);
        ID3D11DeviceContext_RSSetViewports(ctx, 1, &vp);
        ID3D11DeviceContext_RSSetScissorRects(ctx, 1, &sr);
        ID3D11DeviceContext_VSSetShader(ctx, g_gpuVs, NULL, 0);
        ID3D11DeviceContext_GSSetShader(ctx, NULL, NULL, 0);
        ID3D11DeviceContext_HSSetShader(ctx, NULL, NULL, 0);
        ID3D11DeviceContext_DSSetShader(ctx, NULL, NULL, 0);
        ID3D11DeviceContext_PSSetShader(ctx, g_gpuPs, NULL, 0);
        ID3D11DeviceContext_PSSetShaderResources(ctx, 0, 1, &useSrv);
        ID3D11DeviceContext_PSSetSamplers(ctx, 0, 1, useSmp);
        ID3D11DeviceContext_IASetInputLayout(ctx, g_gpuLay);
        ID3D11DeviceContext_IASetVertexBuffers(ctx, 0, 1, vbs, &stride, &off);
        ID3D11DeviceContext_IASetIndexBuffer(ctx, NULL, DXGI_FORMAT_UNKNOWN, 0);
        ID3D11DeviceContext_IASetPrimitiveTopology(ctx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        ID3D11DeviceContext_Draw(ctx, 4, 0);
    }

    ID3D11DeviceContext_OMSetRenderTargets(ctx, 1, &svRtv, svDsv);
    ID3D11DeviceContext_OMSetBlendState(ctx, svBlend, svBf, svMask);
    ID3D11DeviceContext_OMSetDepthStencilState(ctx, svDs, svRef);
    ID3D11DeviceContext_RSSetState(ctx, svRs);
    if (svNvp) ID3D11DeviceContext_RSSetViewports(ctx, svNvp, svVp);
    else       ID3D11DeviceContext_RSSetViewports(ctx, 0, NULL);
    if (svNsr) ID3D11DeviceContext_RSSetScissorRects(ctx, svNsr, svSr);
    else       ID3D11DeviceContext_RSSetScissorRects(ctx, 0, NULL);
    ID3D11DeviceContext_VSSetShader(ctx, svVs, NULL, 0);
    ID3D11DeviceContext_GSSetShader(ctx, svGs, NULL, 0);
    ID3D11DeviceContext_HSSetShader(ctx, svHs, NULL, 0);
    ID3D11DeviceContext_DSSetShader(ctx, svDm, NULL, 0);
    ID3D11DeviceContext_PSSetShader(ctx, svPs, NULL, 0);
    ID3D11DeviceContext_PSSetShaderResources(ctx, 0, 16, svSrv);
    ID3D11DeviceContext_PSSetSamplers(ctx, 0, 16, svSmp);
    ID3D11DeviceContext_IASetInputLayout(ctx, svLay);
    { ID3D11Buffer *rb[1]; rb[0] = svVb0;
      ID3D11DeviceContext_IASetVertexBuffers(ctx, 0, 1, rb, &svStride, &svOff); }
    ID3D11DeviceContext_IASetIndexBuffer(ctx, svIb, svIbFmt, svIbOff);
    ID3D11DeviceContext_IASetPrimitiveTopology(ctx, svTop);

    if (svRtv) ID3D11RenderTargetView_Release(svRtv);
    if (svDsv) ID3D11DepthStencilView_Release(svDsv);
    if (svBlend) ID3D11BlendState_Release(svBlend);
    if (svDs) ID3D11DepthStencilState_Release(svDs);
    if (svRs) ID3D11RasterizerState_Release(svRs);
    if (svVs) ID3D11VertexShader_Release(svVs);
    if (svGs) ID3D11GeometryShader_Release(svGs);
    if (svPs) ID3D11PixelShader_Release(svPs);
    if (svHs) ID3D11HullShader_Release(svHs);
    if (svDm) ID3D11DomainShader_Release(svDm);
    if (svLay) ID3D11InputLayout_Release(svLay);
    if (svVb0) ID3D11Buffer_Release(svVb0);
    if (svIb) ID3D11Buffer_Release(svIb);
    for (k = 0; k < 16; k++) {
        if (svSrv[k]) ID3D11ShaderResourceView_Release(svSrv[k]);
        if (svSmp[k]) ID3D11SamplerState_Release(svSmp[k]);
    }
    ID3D11RenderTargetView_Release(rtv);
    return 1;
}

typedef struct {
    void (*init)(void);
    void (*poll)(DWORD now);
    void (*draw)(ID3D11Texture2D *bb, D3D11_TEXTURE2D_DESC *d);
} FrameExt;
static const FrameExt FRAME_EXTS[] = {
    { ovimg_init, ovimg_poll, ovimg_draw },
};
#define NFE ((int)(sizeof(FRAME_EXTS)/sizeof(FRAME_EXTS[0])))

static void on_frame(IDXGISwapChain *sc)
{

    if (!g_dev) {
        if (FAILED(IDXGISwapChain_GetDevice(sc, &IID_ID3D11Device, (void **)&g_dev)) || !g_dev) {
            g_dev = NULL;
            return;
        }
        ID3D11Device_GetImmediateContext(g_dev, &g_ctx);
        logf_("[overlay] v4 device ok build=v210-delegpersist dev=%p ctx=%p sac=%s",
              (void *)g_dev, (void *)g_ctx, SAC_TAG);
        {
            int fe;
            for (fe = 0; fe < NFE; fe++)
                if (FRAME_EXTS[fe].init) FRAME_EXTS[fe].init();
        }

        if (g_death_state < 0) death_policy_apply(1);
    }
    if (!g_ctx) return;

    ID3D11Texture2D *bb = NULL;
    if (FAILED(IDXGISwapChain_GetBuffer(sc, 0, &IID_ID3D11Texture2D, (void **)&bb)) || !bb)
        return;

    D3D11_TEXTURE2D_DESC d;
    ID3D11Texture2D_GetDesc(bb, &d);

    if (g_scale <= 0.0f) {
        float border = (d.Width - d.Height * 4.0f / 3.0f) / 2.0f;
        if (border < 150.0f) border = 150.0f;
        g_scale = (border - MARGIN * 2) / (float)UI_W0;
        if (g_scale > 5.0f) g_scale = 5.0f;
        if (g_scale < 1.0f) g_scale = 1.0f;
        recompute_layout();
        logf_("[overlay] v4 backbuffer %ux%u, border=%.0f -> scale %.2f, panel %ux%u",
              d.Width, d.Height, border, g_scale, g_uiW, g_uiH);
    }

    handle_hotkeys();
    {
        int fe;
        DWORD now = GetTickCount();
        for (fe = 0; fe < NFE; fe++)
            if (FRAME_EXTS[fe].poll) FRAME_EXTS[fe].poll(now);
    }

    if (!g_hookStart) { g_hookStart = 1; CreateThread(NULL, 0, kb_thread, NULL, 0, NULL); }

    unsigned int num = 0;
    int mode = detect_mode(&num);

    {
        unsigned int pg_now  = rd32(PAGE_ID);
        unsigned int mir_now = rd32(CITY_MIRROR);
        unsigned int pf      = rd32(PLAYER_FAC);
        unsigned int idx_now = rd32(IDX_ADDR);
        unsigned int ctrl    = rd32(CTRL_SLOT);
        unsigned int tc;
        int scene = detect_scene(mode, num, pg_now, mir_now, pf, ctrl, &tc);
        panel_step(scene, tc, mode, num, pg_now, idx_now, mir_now, ctrl);
        rules_round_check();
    }

    {
        unsigned int pg = rd32(PAGE_ID), ia = rd32(IDX_ADDR), mi = rd32(CITY_MIRROR);
        DWORD pk = (pg << 20) ^ (ia << 4) ^ mi;
        if (pk != g_lastDiag) {
            g_lastDiag = pk;
            logf_("[diag] pg=%u idx=%u mir=%u -> mode=%d num=%u", pg, ia, mi, mode, num);
        }
    }

    {
        int _want = panel_rows(mode);        if (_want != g_rowsCur) {
            g_rowsCur = _want;
            recompute_layout();
        }
    }

    unsigned int vals[12];
    memset(vals, 0, sizeof(vals));
    if (mode == MODE_CITY)         get_city(num, vals);
    else if (mode == MODE_FACTION) get_faction(num, vals);
    else if (mode == MODE_CHAR)    get_char(num, vals);

    unsigned int key = (unsigned int)mode * 0x1000000u + num * 16u + (unsigned int)g_cur;
    if (key != g_key || memcmp(vals, g_lastVals, sizeof(vals)) != 0) {
        g_key = key;
        memcpy(g_lastVals, vals, sizeof(vals));
        g_mode = mode;
        g_dirty = 1;
    }
    g_mode = mode;

    {
        int fe;
        for (fe = 0; fe < NFE; fe++)
            if (FRAME_EXTS[fe].draw) FRAME_EXTS[fe].draw(bb, &d);
    }

    if (!g_cheat_on && mode != MODE_NONE && g_pull_mode == 0) {
        InterlockedExchange(&g_panelLive, 0);
        ID3D11Texture2D_Release(bb);
        return;
    }
    InterlockedExchange(&g_panelLive, g_show ? 1 : 0);

    if (!g_show) { ID3D11Texture2D_Release(bb); return; }

    if (!g_uiTex || g_texW != g_uiW || g_texH != g_uiH || g_texFmt != d.Format ||
        !g_uploadTex || g_uploadW != g_uiW || g_uploadH != g_uiH || g_uploadFmt != d.Format) {
        if (g_uiTex) { ID3D11Texture2D_Release(g_uiTex); g_uiTex = NULL; }
        if (g_uploadTex) { ID3D11Texture2D_Release(g_uploadTex); g_uploadTex = NULL; }
        D3D11_TEXTURE2D_DESC ud;
        ZeroMemory(&ud, sizeof(ud));
        ud.Width            = g_uiW;
        ud.Height           = g_uiH;
        ud.MipLevels        = 1;
        ud.ArraySize        = 1;
        ud.Format           = d.Format;
        ud.SampleDesc.Count = 1;
        ud.Usage            = D3D11_USAGE_DEFAULT;
        ud.BindFlags        = D3D11_BIND_SHADER_RESOURCE;
        HRESULT hr = ID3D11Device_CreateTexture2D(g_dev, &ud, NULL, &g_uiTex);
        if (FAILED(hr) || !g_uiTex) {
            g_uiTex = NULL;
            logf_("[overlay] CreateTexture2D failed hr=0x%08lX", (unsigned long)hr);
            ID3D11Texture2D_Release(bb);
            return;
        }
        ud.Usage = D3D11_USAGE_STAGING; ud.BindFlags = 0;
        ud.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        hr = ID3D11Device_CreateTexture2D(g_dev, &ud, NULL, &g_uploadTex);
        if (FAILED(hr) || !g_uploadTex) {
            g_uploadTex = NULL;
            logf_("[overlay] upload staging 创建失败 hr=0x%08lX", (unsigned long)hr);
            ID3D11Texture2D_Release(g_uiTex); g_uiTex = NULL;
            ID3D11Texture2D_Release(bb);
            return;
        }
        g_texW = g_uiW; g_texH = g_uiH; g_texFmt = d.Format;
        g_uploadW = g_uiW; g_uploadH = g_uiH; g_uploadFmt = d.Format;
        g_dirty = 1; g_uploadDirty = 1;
    }

    int swapRB = (d.Format == DXGI_FORMAT_R8G8B8A8_UNORM ||
                  d.Format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
    {
        int needUpload = 0;
        if (g_dirty) { paint(); g_dirty = 0; needUpload = 1; }
        else if (g_uploadDirty) { g_uploadDirty = 0; needUpload = 1; }
        if (needUpload) {
        D3D11_MAPPED_SUBRESOURCE ms;
        if (SUCCEEDED(ID3D11DeviceContext_Map(g_ctx, (ID3D11Resource *)g_uploadTex, 0,
                                              D3D11_MAP_WRITE, 0, &ms))) {
        BYTE *src = (BYTE *)g_bits;
        UINT y;
        for (y = 0; y < g_uiH; y++) {
            BYTE *s = src + (size_t)y * g_uiW * 4;
            BYTE *t = (BYTE *)ms.pData + (size_t)y * ms.RowPitch;
            if (swapRB) {
                UINT x;
                for (x = 0; x < g_uiW; x++) {
                    t[x * 4 + 0] = s[x * 4 + 2];
                    t[x * 4 + 1] = s[x * 4 + 1];
                    t[x * 4 + 2] = s[x * 4 + 0];
                    t[x * 4 + 3] = 255;
                }
            } else {
                memcpy(t, s, (size_t)g_uiW * 4);
            }
        }
        ID3D11DeviceContext_Unmap(g_ctx, (ID3D11Resource *)g_uploadTex, 0);

        ID3D11DeviceContext_CopySubresourceRegion(g_ctx, (ID3D11Resource *)g_uiTex, 0,
                                                  0, 0, 0,
                                                  (ID3D11Resource *)g_uploadTex, 0, NULL);
        }
        }
    }

    {
            int blended = 0;
            int inRect = (d.SampleDesc.Count == 1 && PANEL_A < 255 &&
                          (UINT)g_posX + g_uiW <= d.Width && (UINT)g_posY + g_uiH <= d.Height);

            if (inRect && !g_gpuBad && gb_blend(bb)) {
                blended = 1; g_blendPath = 2; g_blendN++;
            }

            int fmt_ok = (d.Format == DXGI_FORMAT_B8G8R8A8_UNORM ||
                          d.Format == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB ||
                          d.Format == DXGI_FORMAT_R8G8B8A8_UNORM ||
                          d.Format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
            if (!blended && inRect && fmt_ok) {
                if (!g_stgTex || g_stgW != g_uiW || g_stgH != g_uiH || g_stgFmt != d.Format) {
                    if (g_stgTex) { ID3D11Texture2D_Release(g_stgTex); g_stgTex = NULL; }
                    D3D11_TEXTURE2D_DESC sd;
                    ZeroMemory(&sd, sizeof(sd));
                    sd.Width = g_uiW; sd.Height = g_uiH; sd.MipLevels = 1; sd.ArraySize = 1;
                    sd.Format = d.Format; sd.SampleDesc.Count = 1;
                    sd.Usage = D3D11_USAGE_STAGING;
                    sd.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
                    if (SUCCEEDED(ID3D11Device_CreateTexture2D(g_dev, &sd, NULL, &g_stgTex))) {
                        g_stgW = g_uiW; g_stgH = g_uiH; g_stgFmt = d.Format;
                    } else { g_stgTex = NULL; g_stgW = 0; g_stgH = 0; }
                }
                if (g_stgTex) {
                    D3D11_BOX bx;
                    bx.left = (UINT)g_posX; bx.top = (UINT)g_posY; bx.front = 0;
                    bx.right = (UINT)g_posX + g_uiW; bx.bottom = (UINT)g_posY + g_uiH; bx.back = 1;
                    ID3D11DeviceContext_CopySubresourceRegion(g_ctx, (ID3D11Resource *)g_stgTex,
                                                              0, 0, 0, 0,
                                                              (ID3D11Resource *)bb, 0, &bx);
                    D3D11_MAPPED_SUBRESOURCE sm;
                    if (SUCCEEDED(ID3D11DeviceContext_Map(g_ctx, (ID3D11Resource *)g_stgTex, 0,
                                                          D3D11_MAP_READ_WRITE, 0, &sm))) {
                        BYTE *gpx = (BYTE *)sm.pData;
                        BYTE *ppx = (BYTE *)g_bits;
                        UINT yy, xx;
                        const int IA = (int)PANEL_A, DA = 255 - (int)PANEL_A;
                        for (yy = 0; yy < g_uiH; yy++) {
                            BYTE *dst = gpx + (size_t)yy * sm.RowPitch;
                            BYTE *srcp = ppx + (size_t)yy * g_uiW * 4;
                            for (xx = 0; xx < g_uiW; xx++) {
                                BYTE *dn = dst + xx * 4, *pn = srcp + xx * 4;

                                if (swapRB) {
                                    dn[0] = (BYTE)((pn[2] * IA + dn[0] * DA) / 255);
                                    dn[1] = (BYTE)((pn[1] * IA + dn[1] * DA) / 255);
                                    dn[2] = (BYTE)((pn[0] * IA + dn[2] * DA) / 255);
                                } else {
                                    dn[0] = (BYTE)((pn[0] * IA + dn[0] * DA) / 255);
                                    dn[1] = (BYTE)((pn[1] * IA + dn[1] * DA) / 255);
                                    dn[2] = (BYTE)((pn[2] * IA + dn[2] * DA) / 255);
                                }
                            }
                        }
                        ID3D11DeviceContext_Unmap(g_ctx, (ID3D11Resource *)g_stgTex, 0);
                        ID3D11DeviceContext_CopySubresourceRegion(g_ctx, (ID3D11Resource *)bb, 0,
                                                                  (UINT)g_posX, (UINT)g_posY, 0,
                                                                  (ID3D11Resource *)g_stgTex, 0, NULL);
                        blended = 1;
                        g_blendPath = 1;
                        g_blendN++;
                    }
                }
            }
            if (!blended) {
                g_blendPath = 0;
                ID3D11DeviceContext_CopySubresourceRegion(g_ctx, (ID3D11Resource *)bb, 0,
                                                          (UINT)g_posX, (UINT)g_posY, 0,
                                                          (ID3D11Resource *)g_uiTex, 0, NULL);
            }
        }

    ID3D11Texture2D_Release(bb);

    g_frames++;
    if (!g_logged && g_frames > 120) {
        g_logged = 1;
        logf_("[overlay] v6 贴图 %ld 帧 ok, %s num=%u cur=%d vals=%u/%u/%u/%u/%u",
              (long)g_frames,
              mode == MODE_CITY ? "CITY" : (mode == MODE_FACTION ? "FACTION"
                                    : (mode == MODE_CHAR ? "CHAR" : "NONE")),
              num, g_cur,
              vals[0], vals[1], vals[2], vals[3], vals[4]);
    }
}

static HRESULT WINAPI myPresent(IDXGISwapChain *sc, UINT sync, UINT flags)
{
    LARGE_INTEGER c0, c1;
    HRESULT hr;
    if (!g_qpcFreq.QuadPart) QueryPerformanceFrequency(&g_qpcFreq);
    QueryPerformanceCounter(&c0);
    on_frame(sc);
    QueryPerformanceCounter(&c1);
    g_ovCost += (ULONGLONG)(c1.QuadPart - c0.QuadPart);
    g_fpsN++;
    hr = g_origPresent(sc, sync, flags);
    fps_window_log();
    return hr;
}

static HRESULT WINAPI myPresent1(void *sc, UINT sync, UINT flags, const void *pp)
{
    LARGE_INTEGER c0, c1;
    HRESULT hr;
    if (!g_qpcFreq.QuadPart) QueryPerformanceFrequency(&g_qpcFreq);
    QueryPerformanceCounter(&c0);
    on_frame((IDXGISwapChain *)sc);
    QueryPerformanceCounter(&c1);
    g_ovCost += (ULONGLONG)(c1.QuadPart - c0.QuadPart);
    g_fpsN++;
    hr = g_origPresent1(sc, sync, flags, pp);
    fps_window_log();
    return hr;
}

static HWND make_dummy_window(void)
{
    WNDCLASSEXA wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize        = sizeof(wc);
    wc.lpfnWndProc   = DefWindowProcA;
    wc.hInstance     = GetModuleHandleA(NULL);
    wc.lpszClassName = "ZWOverlayDummy";
    RegisterClassExA(&wc);
    return CreateWindowExA(0, "ZWOverlayDummy", "", WS_OVERLAPPEDWINDOW,
                           0, 0, 8, 8, NULL, NULL, wc.hInstance, NULL);
}

static int install_hooks(void)
{
    HWND h = make_dummy_window();
    if (!h) { logf_("[overlay] dummy window failed"); return 0; }

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount       = 1;
    sd.BufferDesc.Width  = 8;
    sd.BufferDesc.Height = 8;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow      = h;
    sd.SampleDesc.Count  = 1;
    sd.Windowed          = TRUE;
    sd.SwapEffect        = DXGI_SWAP_EFFECT_DISCARD;

    IDXGISwapChain *sc = NULL;
    ID3D11Device *dev = NULL;
    ID3D11DeviceContext *ctx = NULL;
    D3D_FEATURE_LEVEL fl;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
                                               NULL, 0, D3D11_SDK_VERSION, &sd, &sc,
                                               &dev, &fl, &ctx);
    if (FAILED(hr)) {
        hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, 0,
                                           NULL, 0, D3D11_SDK_VERSION, &sd, &sc,
                                           &dev, &fl, &ctx);
    }
    if (FAILED(hr) || !sc) {
        logf_("[overlay] dummy swapchain failed hr=0x%08lX", (unsigned long)hr);
        DestroyWindow(h);
        return 0;
    }

    void **vt = *(void ***)sc;
    DWORD old;
    if (VirtualProtect(&vt[8], sizeof(void *) * 15, PAGE_READWRITE, &old)) {
        g_origPresent = (PresentFn)vt[8];
        vt[8] = (void *)myPresent;
        if (vt[22]) {
            g_origPresent1 = (Present1Fn)vt[22];
            vt[22] = (void *)myPresent1;
        }
        VirtualProtect(&vt[8], sizeof(void *) * 15, old, &old);
        logf_("[overlay] hooked Present orig=%p Present1 orig=%p",
              (void *)g_origPresent, (void *)g_origPresent1);
    } else {
        logf_("[overlay] VirtualProtect failed");
    }

    IDXGISwapChain_Release(sc);
    if (dev) ID3D11Device_Release(dev);
    if (ctx) ID3D11DeviceContext_Release(ctx);
    DestroyWindow(h);
    return g_origPresent != NULL;
}

static DWORD WINAPI init_thread(LPVOID param)
{
    (void)param;
    logf_("---- [overlay] v4 init pid=%lu ----", (unsigned long)GetCurrentProcessId());
    int i;
    for (i = 0; i < 40; i++) {
        if (install_hooks()) { menu_inject(); office_demote_arm(1); return 0; }
        Sleep(500);
    }
    logf_("[overlay] FATAL: hooks not installed");
    return 1;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved)
{
    (void)reserved;
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinst);
        CreateThread(NULL, 0, init_thread, NULL, 0, NULL);
    }
    return TRUE;
}
