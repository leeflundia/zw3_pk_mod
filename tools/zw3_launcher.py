# -*- coding: utf-8 -*-
"""郑问之三国志 MOD —— 开发调试工具：启动 / 注入 / 状态 一体化

用法：
    python zw3_launcher.py status    查看游戏与面板状态
    python zw3_launcher.py inject    只注入（游戏需已运行）
    python zw3_launcher.py start     启动游戏并自动注入

环境变量：
    ZW3_GAME_EXE   游戏主程序完整路径（start 命令必需；本脚本不含任何机器路径）
    ZW3_ASCII_DIR  注入中转目录（需纯 ASCII 路径；默认取系统临时目录）
    ZW3_NEEDLE     进程名匹配子串（默认 CHENUEN）

依赖：pip install psutil
构建产物：先跑仓库根目录的 build.sh → build/ 下生成 inject.exe 与 zy_overlay14.dll

为什么需要"防重复注入"：
    DLL 一旦注入就无法卸载（Windows 限制），重复注入会让多个 overlay 叠加绘制，
    画面会出现多重面板。本脚本会检查模块列表，已加载则拒绝再次注入并提示重启游戏。

说明：本工具用于"注入式"开发调试；正式体验（玩家方式）是发布包内的
      d3d11/dxgi 垫片自动加载链，无需本脚本。面板热键见 docs/ARCHITECTURE.md §4.2。
"""
import os
import subprocess
import sys
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
BUILD = os.path.join(REPO, 'build')

INJECTOR = os.path.join(BUILD, 'inject.exe')
DLL = os.path.join(BUILD, 'zy_overlay14.dll')

ASCII_DIR = os.environ.get('ZW3_ASCII_DIR') or tempfile.gettempdir()
DLL_ASCII = os.path.join(ASCII_DIR, 'zw_overlay_inject.dll')

GAME_EXE = os.environ.get('ZW3_GAME_EXE', '')
NEEDLE = os.environ.get('ZW3_NEEDLE', 'CHENUEN')


def is_ascii_path(p):
    try:
        p.encode('ascii')
        return True
    except UnicodeEncodeError:
        return False


def dll_locked():
    """判据：DLL 若已被进程加载，会以映像方式映射 ⇒ 无法以写方式打开。
    （比 EnumProcessModules 简单可靠，且不受 64/32 位数跨界限制。）"""
    if not os.path.exists(DLL_ASCII):
        return False
    try:
        f = open(DLL_ASCII, 'r+b')
        f.close()
        return False
    except OSError:
        return True


def find_pid():
    try:
        import psutil
    except ImportError:
        print('缺少依赖：pip install psutil')
        sys.exit(5)
    for p in psutil.process_iter(['pid', 'name']):
        if NEEDLE in (p.info['name'] or '').upper():
            return p.info['pid']
    return 0


def status():
    pid = find_pid()
    if not pid:
        print('游戏未运行')
        return 1
    print('游戏 pid=%d' % pid)
    if dll_locked():
        print('面板：已注入生效中')
        print('⇒ 若要换新版，需**退出游戏重开**再注入（DLL 无法热卸载）。')
    else:
        print('面板：未注入（可执行 inject）')
    return 0


def inject():
    pid = find_pid()
    if not pid:
        print('❌ 游戏未运行，先启动游戏（或用 start 子命令）')
        return 2
    if dll_locked():
        print('⚠ 已注入过，拒绝重复注入（会造成多重面板叠加）。换新版请先退出游戏。')
        return 3

    if not os.path.exists(DLL):
        print('❌ 找不到构建产物 %s —— 先在仓库根目录执行 build.sh' % DLL)
        return 6
    if not is_ascii_path(DLL_ASCII):
        print('❌ 中转路径含非 ASCII 字符：%s\n   请设置环境变量 ZW3_ASCII_DIR 指向纯英文目录。' % DLL_ASCII)
        return 7

    if (not os.path.exists(DLL_ASCII)) or (
            os.path.getsize(DLL_ASCII) != os.path.getsize(DLL)):
        os.makedirs(os.path.dirname(DLL_ASCII), exist_ok=True)
        try:
            with open(DLL, 'rb') as f:
                data = f.read()
            with open(DLL_ASCII, 'wb') as f:
                f.write(data)
            print('已同步 DLL 到 %s' % DLL_ASCII)
        except Exception as e:
            print('⚠ 同步 DLL 失败（可能被占用）：%s' % e)

    r = subprocess.run([INJECTOR, NEEDLE, DLL_ASCII], capture_output=True)
    out = (r.stdout or b'').decode('gbk', 'ignore').strip()
    print(out)
    return 0 if 'injected' in out else 4


def start():
    if not GAME_EXE:
        print('❌ 未设置游戏路径。请先设置环境变量，例如：')
        print('   set ZW3_GAME_EXE=D:\\Games\\ZW3\\CHENUEN_c.exe')
        print('   （或使用 Git Bash：export ZW3_GAME_EXE="/d/Games/ZW3/CHENUEN_c.exe"）')
        return 8
    if not os.path.exists(GAME_EXE):
        print('❌ 游戏主程序不存在：%s' % GAME_EXE)
        return 8

    pid = find_pid()
    if not pid:
        print('启动游戏：%s' % GAME_EXE)
        subprocess.Popen([GAME_EXE], cwd=os.path.dirname(GAME_EXE))
        for _ in range(30):
            time.sleep(1)
            if find_pid():
                break
        print('游戏已启动 pid=%d' % find_pid())
    else:
        print('游戏已在运行 pid=%d' % pid)
    time.sleep(3)
    return inject()


def main():
    cmd = sys.argv[1] if len(sys.argv) > 1 else 'status'
    if cmd == 'status':
        return status()
    if cmd == 'inject':
        return inject()
    if cmd == 'start':
        return start()
    print(__doc__)
    return 0


if __name__ == '__main__':
    sys.exit(main())
