# 代码架构导览（外置说明书）

> **本文件是代码的"外置注释"。** 代码本身**刻意保持零注释**——注释与实现一旦分家就会互相误导，
> 不如把解释集中到唯一一份文档里。所有结构说明、机制原理、改动指引都在本文件。
>
> **维护规则（重要）**：
> 1. 改代码 → **同步改本文件**（改代码的人负责，不接受"以后再补"）；
> 2. 本文件以**符号名**（函数名/宏名/数组名）为锚点——它们可以用 grep 秒定位，**行号会漂移，尽量别用**；
> 3. 主要读者：**接手开发的工程师与 AI 助手**。写作优先保证"信息密度"与"可检索性"，不求文采；
> 4. **全部代码零注释**（2026-10-10 起执行）：解释性注释已剥离进本文件与 [INTERNALS.md](INTERNALS.md)——新知识一律写入文档，⛔不要加回代码里。

---

## 1. 一页速览

```
游戏 CHENUEN_c.exe 启动
  │  加载同目录 d3d11.dll（=我们的垫片 d3d11_proxy.c）
  ├─► 垫片检测环境（env_is_wine：ntdll wine_get_version + 注册表双判据）
  │     · 真 Windows → 转发 System32\d3d11.dll（无视同目录 DXVK 文件）
  │     · Wine（安卓） → 转发同目录 d3d11_real.dll（DXVK）
  ├─► 垫片按候选表加载面板：zy_overlay14.dll（数字大者优先，加载第一个存在的）
  │     · 另有一个独立 boot 线程给系统 dxgi"钉钉子"（PC 防 DXVK 混搭，v6+pin）
  │
  ├─► zy_overlay14.dll（=overlay.c）：DllMain → init_thread → install_hooks
  │     · hook 交换链 Present/Present1（on_frame 里做全部渲染与逻辑）
  │     · 键盘钩子线程 kb_thread + 鼠标滚轮钩子 ms_proc
  │     · 面板绘制到 DIB → 上传纹理 → 在 Present 里混合叠加
  │
  └─► 同目录 dxgi.dll（=dxgi_proxy.c 垫片）：
        · PC → System32\dxgi.dll ／ Wine → dxgi_real.dll（DXVK）
        · 作用：切断"微软组件 × DXVK dxgi 混搭"崩溃链
```

**运行环境两形态**：PC（Windows 11 实测）与安卓（盖世游戏 / Winlator 的 Wine 容器）。
同一套文件两端通用——垫片自己识别环境（`env_is_wine`）。

## 2. 文件清单

| 文件 | 行数 | 职责 |
|---|---|---|
| `src/overlay.c` | ~4550 | **全部面板功能**：渲染 / 输入 / 内存修改 / 委任 / 作弊 / 召集 / 设置 / 诊断 |
| `src/d3d11_proxy.c` + `src/fwd.S` + `src/d3d11.def` | ~300 | d3d11 垫片（环境感知转发 + 面板自动加载），fwd.S 是 51 个转发桩 |
| `src/dxgi_proxy.c` + `src/fwd_dxgi.S` + `src/dxgi.def` | ~170 | dxgi 垫片（安全转发器），20 个转发桩 |
| `src/injector.c` | ~70 | 32 位注入器（`find_pid` / `wmain`），开发调试用 |
| `tools/zw3_launcher.py` | ~170 | 注入式启动器（status / inject / start 三命令） |
| `docs/addresses.json` | — | **内存地址表**：地址、定位过程、坑注（overlay 硬编码值的来源档案） |

> 全部源码为**零注释纯实现**（2026-10-10 剥离）。两个垫片原有的头部设计注释
> （版本谱系、实验证据链）与其他全部注释一并迁入 [INTERNALS.md](INTERNALS.md)——查机制细节看它。

## 3. overlay.c 分区地图（按源码顺序）

| 区 | 符号锚点（grep 这些） | 职责 |
|---|---|---|
| A. 常量与地址 | `RUNTIME_BASE` `CITY_ARR` `ACTED_FLAG` `PANEL_*` `SCENE_*` `ACT_*` `UI_COL_*` `FNAME`/`FOFF` `CNAME`/`CSTEP` | 游戏内存地址、面板状态机枚举、UI 色板（改配色就改这里） |
| B. 基础设施 | `cap_for` `text_w` `wrap_text` `mod_dir` `logf_` `l10` `ctl_ok` `alt_ok` | 文本测量/换行、取 exe 目录、日志（写 exe 同目录）、修饰键检测 |
| C. 内存读写层 | `rd32` `wr32` `wr32g` `addr_writable` `ctx_writable` `char_idx_valid` | **所有**写内存的必经关口——三重校验（语境/边界/合法值域），被拦截会记日志 |
| D. 游戏互操作 | `detect_mode` `player_fac` `game_cache_refresh` `rebuild_syms_ok` `train_syms_ok` `office_demote_arm` `death_policy_apply` | 读游戏状态、定位游戏函数地址（符号重建）、机制字节写入 |
| E. 委任机制 | `deleg_*` 全家（`deleg_tier_of` `deleg_round_end` `deleg_on_load` `deleg_toggle` `deleg_open` …） | 太守委任：档位/进度/预算/轮转结算/读档恢复。**轮转核心 = `deleg_round_end`（每季落一切）** |
| F. 设置持久化 | `settings_path` `settings_save` `settings_load` | 写入 `cd-rom\zyconf\settings.dat`（magic 头 + 结构体）；委任档位/战死/作弊/覆盖开关随档保存 |
| G. 业务功能 | 召集 `pull_*`；训练 `game_train_one`/`batch_train_city`/`train_list_build`/`train_pick`；作弊 `cheat_full_army`/`cheat_*_fire`；刷新 `do_refresh_action`；改值 `apply_field` | 面板里的每个"功能"实现 |
| H. 菜单/机制数据 | `MECHS_MECH` `MECHS_CHEAT` `DELEG_ITEMS` `DIAGS` `KeyDef`/`KEYS` | **表驱动**：机制项/作弊项/诊断项/热键都是表——加功能先加表项 |
| I. 规则框架 | `GameRule` 结构 + `g_rules[]`（`rule_death_policy` `rule_delegation`） | 机制的统一注册：轮末回调 `on_round_end` / 场景切换 `on_scene_change` / 读档 `on_game_load` |
| J. 面板状态机 | `panel_step` `rules_round_check` `menu_nav` `panel_rows`/`PANEL_ROWS` `nav_*` 全家 `do_action` | 场景+当前状态 → 显示哪块面板；`nav_*` 是各面板的按键导航逻辑；`do_action` 是动作分发终点 |
| K. 渲染 | `paint`（主渲染，~660 行）`recompute_layout` `ensure_dib` `mode_bg/edge/title` | 画整个面板到 DIB（标题/行/高亮/提示/进度条） |
| L. 输入 | `kb_thread`（钩子线程）`kb_proc` `ms_proc`（滚轮）`handle_hotkeys` `handle_wheel` `poll_hotkeys` `act_of`/`altact_of` `keys_lookup` | 热键表 `KEYS[]` 查找 → ACT_* 动作；轮询兜底 `poll_hotkeys` |
| M. 覆盖画面通道 | `ovimg_*`（`init`/`load`/`poll`/`ensure_tex`/`upload`/`draw`） | 读 `ovimg\SCREEN.BMP` 由 overlay 覆盖绘制（Ctrl+P 开关）；PC/手机推图工具配套 |
| N. D3D 资源 | `g_dxbc_vs`/`g_dxbc_ps`（字节码）`gb_ensure` `gb_blend` | 预编译着色器 + 渲染状态，实现面板与游戏画面的混合（含专属混合模式） |
| O. 帧回调 | `on_frame`（hook 主回调）`myPresent`/`myPresent1` `make_dummy_window` `install_hooks` | Present 劫持链：每帧 → 面板上传/混合/绘制；`install_hooks` 在 init 时挂上 |
| P. 入口 | `DllMain` → `init_thread` | 起线程做全部初始化（避免 loader lock 里干活） |

## 4. 核心机制速查

### 4.1 渲染链（每帧）
`myPresent` → `on_frame` → 若面板脏 `paint()` 画到 DIB → `gb_blend()` 用 DXBC 着色器把 DIB 混合到后备缓冲 → 交还原 Present。缩放通过 `recompute_layout` 全局几何。

### 4.2 输入系统
三条通道：① 键盘钩子线程 `kb_thread`（`WH_KEYBOARD_LL`，主通道）② 鼠标滚轮钩子 `ms_proc` ③ 轮询兜底 `poll_hotkeys`（对钩子环境的保险）。
**热键全表**（`KEYS[]`，15 项）：`Ctrl+↑/↓` 选行、`Ctrl+←/→` 减/加值（`PANEL_MAP` 下做翻页）、`Ctrl+A` 当前项拉满（`ACT_MAXFIELD`；地图/委任页例外走 `ACT_ALL`，见 `act_of` 的特判）、`Ctrl+S` 兵力调满、`Ctrl+T` 训练度调满、`Ctrl+R` 刷新行动、`Ctrl+E` 返回/关闭、`Ctrl+P` 覆盖画面开关、`Ctrl+D` 导出诊断、`Ctrl+Enter` 呼出菜单/确认、`Alt+←/→` 面板缩放、`Alt+Enter` 显示切换。

### 4.3 内存安全闸（改动数据的必经逻辑）
所有功能改内存都走 `wr32g`：先 `ctx_writable`（当前语境合法）→ `addr_writable`（地址在数据区范围）→ 值域上限 `cap_for`。**拒绝写会记日志（[guard] 前缀）**——诊断面板写入 bug 先看这里的日志。

### 4.4 面板状态机
`panel_step` 根据（场景 `SCENE_*`，检测目标 `detect_mode`）推出当前该显示的面板（`PANEL_*`）与行集；`nav_*` 处理该面板上的按键；`do_action` 执行。**加界面 = 加 PANEL_* 常量 + PANEL_ROWS 项 + nav_ 函数 + paint 某分支。**

### 4.5 委任机制
档位（0/¼/½/¾/满）存于内存镜像 + `settings.dat`；`deleg_round_end`（注册在 `GameRule.on_round_end`）在**每季轮末**按档位执行开发；`deleg_on_load`（`on_game_load`）处理读档后状态恢复。**该机制的三条铁律**：⛔不再"凡 0 档一律拉满"（v210 修复）、改档即落盘、首都命令菜单不跳修改器。

### 4.6 覆盖画面通道（ovimg）——新机制画面的官方通道
`ovimg_load` 读 `<exe目录>\ovimg\SCREEN.BMP` → 作为纹理整屏不透明绘制（250ms 轮询指纹触发）。
`OFF` 哨兵文件在场 = 暂停；删文件 = 恢复游戏画面。`Ctrl+P` 显/隐（`g_ovOn`，**默认关闭**，随 settings 持久化）。

**战略定位**（与 GDI 面板的双通道分工）：
- **ovimg** = 美术资产级画面（事件画幅、决策界面、地图册）——**新机制的新界面走这里，不必逆向/替换游戏渲染管线**；
- **GDI 面板**（`paint()` 链）= 实时读数（改内存值同帧反映）；面板永远绘制在覆盖画面之上，两者可同时用。

> 与"立绘素材"**无关**：立绘是游戏资源文件（BCV 系列），走独立的素材流水线（外部增量，不在本仓库）。
> ovimg 是 overlay 的**运行时呈现能力**，属 overlay 本体功能。
> PC/手机侧的"推图工具"（把图片送进 `ovimg/` 目录）才是外部增量，不在本仓库内。

### 4.7 设置持久化
`settings.dat`（在 `cd-rom\zyconf\`）带 magic 校验；字段：委任档位/战死策略/作弊模式/覆盖开关/格式版本。**加持久化字段 = 改结构体 + 升格式版本 + 处理旧档兼容。**

### 4.8 诊断
`Ctrl+D` → `diag_dump_all`：按 `DIAGS[]` 表 dump 各内存区到 `<exe目录>\zydiag\`。**排查"功能不对"的第一入口**。

## 5. 常见开发任务 → 代码位置索引

| 想做什么 | 动哪里 |
|---|---|
| 加作弊菜单项 | `MECHS_CHEAT[]` 加表项 + 写 `cheat_*_fire` 函数 + （如有面板行）`nav_cheat` |
| 加机制菜单项 | `MECHS_MECH[]` + `GameRule` 回调 |
| 改配色/字号 | `UI_COL_*` 宏 + `paint` 里的字号参数 |
| 加热键 | `KEYS[]` 表 + （新动作时）`ACT_*` 常量 + `do_action` 分支 |
| 改面板布局/行列数 | `PANEL_ROWS[]` + `panel_rows` + `recompute_layout`（几何） |
| 加一个值域上限 | `cap_for` |
| 改委任规则 | `deleg_*`（档位表 `DELEG_ITEMS`、轮转 `deleg_round_end`） |
| 定位新内存地址 | 按 `docs/addresses.json` 的方法论；新地址加进常量区 A 并更新此文档 |
| 调渲染混合效果 | `gb_blend` + `g_dxbc_vs/ps`（预编译着色器字节码） |
| 处理"某功能没反应" | 先 `Ctrl+D` 诊断 + 看 exe 目录 `zw3_overlay.log` |

## 6. 垫片链要点（细节见 INTERNALS.md §6）

- **d3d11_proxy.c**：候选表 `CAND[]` 按数字从高到低找面板 DLL（`zy_overlay14.dll` 为现役名）；`env_is_wine` 双判据；boot 线程做"钉钉子"（`g_pinDesc` 记录结果，看 `zy_proxy.log` 判读）。
- **dxgi_proxy.c**：同架构；导出 20 个与系统对齐，保证"谁加载都是安全转发"。
- **fwd.S / fwd_dxgi.S**：每个导出一个 `jmp *g_tab+N` 桩——垫片导出表与真身函数表的绑定在 `DllMain`/`boot` 里填充。
- **为什么对象顺序/库顺序在 build.sh 里写死**：影响 .idata 元数据排列（复现基准需要），详见 BUILD.md。

## 7. 已知边界与约定

- **零注释策略**：overlay.c 不再添加行内注释——解释一律进本文件（分家即误导）；
- **-O0 纪律**：编译禁优化（行为与体积特征需与历史构建一致）；
- **地址硬编码**：一切内存常量针对既定版本 exe（见 docs/GAME-VERSION.md），**换版本必须重新定位**；
- **表驱动优先**：加功能先找表（`MECHS_*` / `KEYS` / `PANEL_ROWS` / `DIAGS` / `g_rules`），加表项优于加分支；
- **`nm`/`sys` 三处编译告警**为历史遗留，无害，勿"顺手清理"（会破坏复现基准）。
