#!/usr/bin/env bash
set -e
cd "$(dirname "$0")"
REPO="$(pwd)"
OUT="$REPO/build"

MSYS_ROOT="${MSYS_ROOT:-/c/msys64}"
MINGW="$MSYS_ROOT/mingw32"
CC1="$(ls "$MINGW"/lib/gcc/i686-w64-mingw32/*/cc1.exe 2>/dev/null | head -1 || true)"
if [ -z "$CC1" ]; then
    echo "[错误] 找不到 mingw32 工具链：$MINGW"
    echo "       请安装 msys64 + mingw-w64-i686-gcc，或用 MSYS_ROOT 指定路径"
    exit 1
fi
GCCDIR="$(dirname "$CC1")"
AS="$MINGW/bin/as.exe"
LD="$MINGW/bin/ld.exe"
GCC="$MINGW/bin/gcc.exe"
export PATH="$MINGW/bin:$PATH"
echo "[工具链] $CC1"

WORK="${ZW3_BUILD_DIR:-}"
if [ -z "$WORK" ]; then
    case "$REPO" in
        *[!\ -~]*) WORK="/tmp/zw3build" ;;
        *)         WORK="$REPO/.build-tmp" ;;
    esac
fi
case "$WORK" in
    *[!\ -~]*)
        echo "[错误] 构建暂存目录含非 ASCII 字符：$WORK"
        echo "       mingw 工具链读不了中文路径。仓库位于中文目录时，"
        echo "       请用 ZW3_BUILD_DIR 指定纯英文目录，例如："
        echo "         ZW3_BUILD_DIR=/tmp/zw3build bash build.sh"
        exit 1
        ;;
esac
rm -rf "$WORK"; mkdir -p "$WORK"
cp "$REPO"/src/overlay.c "$REPO"/src/d3d11_proxy.c "$REPO"/src/dxgi_proxy.c \
   "$REPO"/src/injector.c "$REPO"/src/fwd.S "$REPO"/src/fwd_dxgi.S \
   "$REPO"/src/d3d11.def "$REPO"/src/dxgi.def "$WORK/"
cd "$WORK"

echo "[1/4] overlay.c -> zy_overlay14.dll"
"$CC1" -quiet -m32 -Wall \
    -I"$GCCDIR/include" -I"$GCCDIR/include-fixed" -I"$MINGW/include" \
    overlay.c -o overlay.s
"$AS" --32 overlay.s -o overlay.o
"$LD" -m i386pe --shared -o zy_overlay14.dll \
    "$MINGW/lib/dllcrt2.o" \
    "$GCCDIR/crtbegin.o" \
    overlay.o \
    -L"$GCCDIR" -L"$MINGW/lib" \
    -lmingw32 -lgcc -lmingwex -lmsvcrt -lkernel32 -ld3d11 -ldxgi -luuid -luser32 -lgdi32 \
    "$GCCDIR/crtend.o"

echo "[2/4] d3d11_proxy.c + fwd.S -> d3d11.dll"
"$CC1" -quiet -m32 -Wall \
    -I"$GCCDIR/include" -I"$GCCDIR/include-fixed" -I"$MINGW/include" \
    d3d11_proxy.c -o d3d11_proxy.s
"$AS" --32 d3d11_proxy.s -o d3d11_proxy.o
"$AS" --32 fwd.S -o fwd.o
"$LD" -m i386pe --shared -o d3d11.dll \
    "$MINGW/lib/dllcrt2.o" \
    "$GCCDIR/crtbegin.o" \
    d3d11_proxy.o fwd.o d3d11.def \
    -L"$GCCDIR" -L"$MINGW/lib" \
    -lmingw32 -lgcc -lmingwex -lmsvcrt -ladvapi32 -lkernel32 -luser32 \
    "$GCCDIR/crtend.o"

echo "[3/4] dxgi_proxy.c + fwd_dxgi.S -> dxgi.dll"
"$CC1" -quiet -m32 -Wall \
    -I"$GCCDIR/include" -I"$GCCDIR/include-fixed" -I"$MINGW/include" \
    dxgi_proxy.c -o dxgi_proxy.s
"$AS" --32 dxgi_proxy.s -o dxgi_proxy.o
"$AS" --32 fwd_dxgi.S -o fwd_dxgi.o
"$LD" -m i386pe --shared -o dxgi.dll \
    "$MINGW/lib/dllcrt2.o" \
    "$GCCDIR/crtbegin.o" \
    dxgi_proxy.o fwd_dxgi.o dxgi.def \
    -L"$GCCDIR" -L"$MINGW/lib" \
    -lmingw32 -lgcc -lmingwex -lmsvcrt -lkernel32 -ladvapi32 -luser32 \
    "$GCCDIR/crtend.o"

echo "[4/4] injector.c -> inject.exe"
"$GCC" -m32 -O0 -municode -o inject.exe injector.c

mkdir -p "$OUT"
cp zy_overlay14.dll d3d11.dll dxgi.dll inject.exe "$OUT/"
echo
echo "=== 产物清单（build/） ==="
for f in zy_overlay14.dll d3d11.dll dxgi.dll inject.exe; do
    sz=$(stat -c %s "$OUT/$f")
    md5=$(md5sum "$OUT/$f" | cut -d' ' -f1)
    printf "  %-18s %8s B  md5 %s\n" "$f" "$sz" "$md5"
done

if [ -n "$LOADTEST" ] && [ -f "$LOADTEST" ]; then
    echo
    echo "=== 冒烟测试（loadtest32） ==="
    "$LOADTEST" "$OUT/zy_overlay14.dll" && echo "  zy_overlay14.dll: OK"
    "$LOADTEST" "$OUT/d3d11.dll"        && echo "  d3d11.dll: OK"
    "$LOADTEST" "$OUT/dxgi.dll"         && echo "  dxgi.dll: OK"
fi

echo
echo "[完成] 产物已输出到 $OUT"
