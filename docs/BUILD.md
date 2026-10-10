# 构建指南（BUILD）

## 1. 工具链

- Windows + Git Bash（Git for Windows 自带）
- msys64 + mingw-w64 **32 位** GCC：
  ```
  # 安装 msys64 后，在 MSYS2 shell 里执行：
  pacman -S mingw-w64-i686-gcc
  ```
  默认安装路径 `C:\msys64`（脚本按 `/c/msys64` 查找，可用环境变量 `MSYS_ROOT` 覆盖）。

## 2. 一键构建

```bash
cd zw3_pk_mod
bash build.sh
```

环境变量：

| 变量 | 默认 | 说明 |
|---|---|---|
| `MSYS_ROOT` | `/c/msys64` | msys64 安装根目录（**唯一必须绝对的路径**——外部工具链位置，无法相对） |
| `ZW3_BUILD_DIR` | 自动 | 编译暂存目录。默认自动择优：仓库路径纯 ASCII → 用仓库内 `.build-tmp/`（相对、已 gitignore）；仓库在中文目录 → 回退 `/tmp/zw3build`。**必须纯 ASCII**（mingw 读不了中文路径，含非 ASCII 会直接报错退出）。⚠ **每次构建会 `rm -rf` 清空重建该目录**，⛔勿指向含其他数据的目录 |
| `LOADTEST` | （空） | 设为 `loadtest32.exe` 路径则构建后自动加载冒烟测试 |

产物（`build/`）：

| 产物 | 说明 |
|---|---|
| `zy_overlay14.dll` | 面板主模块 |
| `d3d11.dll` | d3d11 垫片（Wine 检测 + 转发 + 面板加载） |
| `dxgi.dll` | dxgi 垫片（安全转发） |
| `inject.exe` | 32 位注入器（开发调试用） |

## 3. 编译链路说明（为什么这样编）

采用**三段链**：`cc1 → as --32 → ld`（`-O0` 无优化链），保持与现役二进制一致的行为特性与体积特征。完整命令参数见 `build.sh` 内注释。

### 关键纪律（都是踩过的坑）

1. **cc1 必须全路径调起**，且 **`mingw32/bin` 必须在 PATH 中**——cc1 依赖的运行时 DLL（libisl/libmpc/libwinpthread 等）在 bin 目录，缺 PATH 时 cc1 会以 `exit=127` **静默失败**（无任何报错）；
2. **全链路 ASCII 路径**：mingw 工具链不接受中文路径。`build.sh` 自动把源码复制到 ASCII 暂存目录再编译（仓库路径含中文也安全）；
3. **overlay 链接 ⛔ 不加 `d3d11.def`**（那是垫片专用导出表，加了会报 `cannot export D3D11*/D3DKMT*`）；
4. **两个垫片链接必须把 `fwd*.S` 汇编的转发桩一并输入**，否则 ld 报 `cannot export X: symbol not defined` ×51（d3d11）/ ×20（dxgi）；
5. **对象 / 库顺序已按"复现留档最优"固定**（顺序只影响 .idata / 符号表等元数据的排列，不影响功能）：
   - d3d11：输入序 `d3d11_proxy.o fwd.o`，库序 `-ladvapi32` 须在 `-lkernel32` 之前；
   - dxgi：输入序 `dxgi_proxy.o fwd_dxgi.o`，库序 `-lkernel32 -ladvapi32 -luser32`；
6. **已知告警**（源码既有、无害，勿改）：`overlay.c`（paint 内）、`d3d11_proxy.c`、`dxgi_proxy.c`（各自 DllMain 内）各一处 unused 变量（`nm`/`sys`/`sys`）。行号会随改动漂移，以变量名为准。

## 4. 复现核验（黄金标准）

### 构建链复现精度（历史实测备案）

将源码回退到历史版本（撤销下方"开源整理记录"的改动后），同名编译与工程留档逐字节对比：

| 产物 | 复现残差 | 残差性质 |
|---|---|---|
| `d3d11.dll` | ≤ 8 字节 | PE 编译时间戳（3 处副本：0x88 / 0xd8 / 0xb004） |
| `dxgi.dll` | ≤ 9 字节 | PE 编译时间戳（3 处副本：0x88 / 0xd8 / 0xaa04） |
| `zy_overlay14.dll` | ≤ 21 字节 | 时间戳 8 + 等价布局差 4（局部变量栈槽次序，语义等价）+ 尾部元数据 9 |

> **残差全部为编译时间戳与文件名字段，代码段零差异**——构建链与原构建流程等价。
> 注：`build.sh` 输出名为最终部署名（`d3d11.dll` 等），与工程留档名不同时会额外引入 ≤11 字节的"文件名池"元数据差异（同样无功能影响）。

### 开源整理记录（相对历史版的源码差异，全部逐条可审计）

| # | 文件 | 改动 | 原因 |
|---|---|---|---|
| 1 | `src/overlay.c` | 日志路径：`D:/zw3_overlay.log` → exe 同目录动态构造（`mod_dir()`，与 zy_proxy.log 同模式） | 去除绝对路径，PC/安卓通用 |
| 2 | `src/overlay.c` | 刷新行动防御提示文本：去除对内部工具的引用 | 开源版自洽 |
| 3 | 全部源码（4 个 C + py + sh） | **注释剥离**：解释性注释全部移除（66 处 + 脚本注释），迁入 `docs/INTERNALS.md`；代码进入零注释策略 | 消除"注释与实现脱节"风险；已重编验证对二进制零影响（对比残差仅时间戳 6 字节） |

以上为全部源码级差异；其余文件为原样迁移。

## 5. 验证方法

1. **加载冒烟**（推荐）：
   ```bash
   LOADTEST=/path/to/loadtest32.exe bash build.sh
   # 每个 DLL 应输出 "OK err=0"（LoadLibrary + DllMain 正常返回）
   ```
2. **游戏内实测**：产物复制到游戏目录 → 启动游戏 → 左上角面板出现 → 核对菜单与热键。

## 6. 部署

- **玩家形态**：`zy_overlay14.dll` + `d3d11.dll` + `dxgi.dll` 放入游戏根目录。游戏加载 d3d11/dxgi 时经垫片链自动加载面板（PC/安卓共用同一份文件，垫片自动区分环境）。
- **⚠ 完整依赖链（PC）**：游戏是 DirectDraw 时代的产物，垫片链之前还需要 dgVoodoo2 的
  wrapper 文件：`DDraw.dll`、`D3DImm.dll`、`dgVoodoo.conf`（把 DDraw 调用转到 D3D11，垫片才拿得到
  Present 可挂）。dgVoodoo2 从官方（dege.freeweb.hu）获取——不在本仓库内。
- **⚠ 完整依赖链（安卓 Wine）**：需要 DXVK 的 `d3d11_real.dll`、`dxgi_real.dll`（垫片在 Wine 环境
  下转发给它们）；通常随盖世游戏 / Winlator 容器提供。
- **发布包的完整根目录形态**（v1.0，可作部署对照）：
  ```
  CHENUEN_c.exe          游戏主程序（自备，版本见 docs/GAME-VERSION.md）
  DDraw.dll D3DImm.dll   dgVoodoo2 wrapper（PC 侧）
  dgVoodoo.conf dgVoodooCpl.exe
  d3d11.dll dxgi.dll     本工程垫片（build.sh 产物）
  d3d11_real.dll dxgi_real.dll   DXVK 真身（安卓侧；PC 上垫片会无视它们）
  zy_overlay14.dll       本工程面板（build.sh 产物）
  cd-rom/                游戏资源（自备）
  ```
- **开发调试**：`tools/zw3_launcher.py`（注入式，见 `python tools/zw3_launcher.py` 用法说明；依赖 `pip install -r tools/requirements.txt`）。
