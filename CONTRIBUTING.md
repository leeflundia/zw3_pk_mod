# 贡献指南（Contributing Guide）

欢迎为《郑问之三国志》MOD 工程贡献代码。提交 PR 前请完整阅读本文件——**不符合红线的 PR 会直接关闭**。

> English summary: contributions are welcome, but **never** commit game binaries, original game assets, or third-party wrapper binaries (see "红线" below). Code is C (32-bit, MinGW, no -O2). Build with `bash build.sh`, then open a PR with real-device test evidence.

## 一、三条红线（违反即关闭）

1. **⛔ 版权物料零容忍**
   - 不得提交：游戏本体任何文件（exe / BCV 素材 / 音乐 / 剧本）、原版立绘与素材、dgVoodoo2 / DXVK 的官方二进制；
   - 重绘立绘等衍生素材**不在本仓库管理范围**（素材线独立于代码仓库），请勿随代码 PR 附带图片资源；
   - 本仓库只收**你自己原创的代码**。引用他人逆向成果须在 PR 里注明来源。
2. **⛔ 构建产物不入库**
   - `build/`、`*.dll`、`*.exe` 一律不提交（`.gitignore` 已排除，勿强行 `git add -f`）。
3. **⛔ 个人信息不入仓**
   - 代码、注释、文档、测试数据中不得出现网盘链接、社交账号、真实姓名/邮箱、内网 IP、本机绝对路径（示例路径请用占位符如 `/d/Games/ZW3/`）。

## 二、开发环境

| 项 | 要求 |
|---|---|
| 编译器 | msys64 + `mingw-w64-i686-gcc`（32 位，唯一支持的工具链） |
| 构建 | 仓库根目录执行 `bash build.sh`，产物在 `build/` |
| 行尾 | **LF**（本仓库 `core.autocrlf=false`；`build.sh` 为 shell 脚本，CRLF 会直接坏） |
| 目标游戏 | 版本必须匹配，核对方法见 [docs/GAME-VERSION.md](docs/GAME-VERSION.md) |

## 三、代码规范（本工程特有）

1. **⛔ 禁止 `-O2` 及任何优化等级改动**——构建链是实测定案，优化会改变指令时序导致运行时失效（历史事故已证）；
2. **`src/overlay.c` 保持零注释**——机制说明一律写入 `docs/ARCHITECTURE.md`（外置说明书）或 `docs/INTERNALS.md`，改行为请同步改文档；
3. **内存地址改动必须实证**——新增/修改地址需附目标 exe 的 MD5 核对过程，并同步更新 `docs/addresses.json`；
4. 面板文案用书面语、术语统一（命令点 / 训练度 / 我方 / 已开启·已禁用），参照现有菜单风格；
5. 新增功能默认**不影响既有功能**：不做大范围重构型 PR，一个 PR 只做一件事。

## 四、贡献流程

```bash
# 1. Fork 本仓库，克隆到本地
git clone https://github.com/<你的用户名>/zw3_pk_mod.git
cd zw3_pk_mod

# 2. 从 main 开一个主题分支
git switch -c feat/简短主题名

# 3. 开发 + 本地构建验证
bash build.sh          # 必须通过

# 4. 提交（commit message 写"为什么"，不只写"改了什么"）
git add -A && git commit -m "fix: 委任季结漏算兵粮（原因：CITY_ARR 偏移在 v205 后位移）"

# 5. 推送到你的 fork，在 GitHub 上发起 PR（base=main）
git push -u origin feat/简短主题名
```

## 五、PR 验收标准（maintainer 按此审查）

- [ ] `bash build.sh` 干净通过；
- [ ] **PC 端实测**：dgVoodoo2 环境下游戏启动、面板可用、改动功能符合预期（附截图或日志）;
- [ ] **安卓端实测**：Winlator 环境验证（若改动涉及双端共用逻辑）；
- [ ] 未触碰三条红线；
- [ ] 涉及文档的行为变化已同步 `docs/`；
- [ ] 行尾为 LF、无多余空白改动（`git diff` 自查）。

> 实测证据可以贴图。无法在双端验证的改动，maintainer 可能要求补充或代为验证后合入。

## 六、版本与发布

- 合入 main 的改动由 maintainer 统一打 tag（`vNNN`）发布，贡献者无需自行版本号；
- 破坏存档兼容性的改动（内存布局、存档字段）必须在 PR 描述中**显著标注**，这类改动审查周期会更长。

## 七、问题反馈

Bug 与新功能请使用 [Issue 模板](../../issues/new/choose)；环境信息（游戏版本 MD5、PC/安卓、dgVoodoo/Winlator 版本）请尽量附全，缺复现信息的 issue 处理优先级会很低。

## 八、许可证约束

提交即表示同意：你的贡献以本仓库的 **MIT 许可证**发布。
