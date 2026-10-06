# 家庭收支记账软件

[中文](README.md) | [English](README-English.md)

一个基于 C11 的控制台家庭收支记账软件示例。项目根据 `docs/家庭收支记账软件.docx` 的需求实现，支持登记收入、登记支出、查看收支明细和安全退出。

项目重点不只是完成基础功能，还将原始的单文件示例拆分为应用层、领域层、用户界面层和共享工具层，演示如何使用结构体、模块化函数、输入校验、动态内存和错误结果提升 C 语言项目的可维护性。

## 功能特性

- 初始账户余额为 `1000.00`。
- 登记收入并记录收入说明。
- 登记支出并记录支出说明。
- 支出金额超过当前余额时拒绝操作并提示余额不足。
- 查看每笔收支金额、交易后的账户余额和说明。
- 没有交易记录时显示空记录提示。
- 退出前要求输入 `y` 或 `n` 确认。
- 菜单、金额、说明和确认输入均进行格式校验。
- 金额在内部使用“分”作为整数保存，避免浮点数计算带来的货币精度问题。
- 交易记录使用动态数组管理，支持运行期间持续追加记录。

## 目录结构

```text
personal-finance-manager/
├── README.md
├── README-English.md
├── docs/
│   └── 家庭收支记账软件.docx
└── src/
    ├── main.c
    ├── app/
    │   ├── finance_app.c
    │   └── finance_app.h
    ├── domain/
    │   ├── finance_account.c
    │   ├── finance_account.h
    │   └── finance_types.h
    ├── shared/
    │   ├── money.c
    │   └── money.h
    └── ui/
        ├── console_input.c
        ├── console_input.h
        ├── console_view.c
        └── console_view.h
```

## 模块说明

| 模块 | 职责 |
| --- | --- |
| `src/main.c` | 程序入口，只负责启动应用 |
| `src/app/` | 应用流程编排，连接账户业务、输入和展示 |
| `src/domain/` | 账户、交易记录和收支业务规则 |
| `src/shared/` | 金额解析与格式化等通用能力 |
| `src/ui/` | 控制台输入读取、校验和输出 |
| `docs/` | 原始需求文档和功能说明 |

## 编译运行

项目没有依赖第三方库，可以使用 GCC 或 Clang 编译。

### GCC

在项目根目录执行：

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Isrc \
    src/main.c \
    src/app/finance_app.c \
    src/domain/finance_account.c \
    src/shared/money.c \
    src/ui/console_input.c \
    src/ui/console_view.c \
    -o finance-manager
```

运行程序：

```bash
./finance-manager
```

在 Windows PowerShell 中运行生成的可执行文件：

```powershell
.\finance-manager.exe
```

### Clang

```bash
clang -std=c11 -Wall -Wextra -Wpedantic -Isrc \
    src/main.c \
    src/app/finance_app.c \
    src/domain/finance_account.c \
    src/shared/money.c \
    src/ui/console_input.c \
    src/ui/console_view.c \
    -o finance-manager
```

## 使用流程

启动程序后，主菜单提供以下操作：

```text
1. 收支明细
2. 登记收入
3. 登记支出
4. 退出
```

### 登记收入

选择 `2`，输入金额和说明。系统会增加账户余额，并保存一条收入记录。

### 登记支出

选择 `3`，输入金额和说明。系统会先检查余额是否充足。余额不足时，本次操作不会写入交易记录。

### 查看明细

选择 `1`，程序会按照交易顺序展示：

- 收支类型
- 收支金额
- 交易完成后的账户余额
- 收支说明

### 退出程序

选择 `4` 后输入 `y` 确认退出，输入 `n` 返回主菜单。其他输入会继续要求重新输入。

## 工程化设计

### 分层职责

- 应用层只负责组织用户操作流程。
- 领域层负责账户余额和交易记录的一致性。
- UI 层负责控制台输入输出，不直接修改账户内部状态。
- 共享层负责金额解析和格式化。

### 金额安全

程序没有使用 `double` 保存余额，而是使用 `int64_t` 保存分。用户输入的 `100.50` 会转换为 `10050` 分，所有加减运算完成后再格式化为两位小数。

### 输入校验

程序会校验菜单范围、金额格式、金额正数约束、金额小数位数、说明长度以及退出确认输入，避免直接使用不安全的 `scanf` 读取用户输入。

### 资源管理

交易记录使用动态数组存储，容量不足时自动扩容；应用退出前释放账户持有的动态内存。

## 当前边界

- 数据只保存在程序运行期间，退出程序后交易记录会丢失。
- 当前版本是单账户、单进程的控制台程序。
- 当前没有数据库、文件持久化、用户登录、图形界面或网络接口。
- 金额输入最多支持两位小数。
- 收支说明最大长度为 `127` 字节。

## 后续扩展方向

如果将项目继续用于真实场景，可以在现有模块边界上扩展：

- 增加文件或数据库持久化。
- 增加按日期、类型和说明查询交易记录。
- 增加月度收入、支出和余额统计。
- 增加预算和分类管理。
- 增加单元测试和集成测试。
- 使用 CMake 或 Makefile 统一构建流程。

## 参考资料

- [家庭收支记账软件需求文档](docs/家庭收支记账软件.docx)
- [C 源代码](src/)
