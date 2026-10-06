# Personal Finance Manager

[中文](README.md) | [English](README-English.md)

A C11 console application for managing household income and expenses. The project is implemented from the requirements in `docs/家庭收支记账软件.docx` and supports recording income, recording expenses, viewing transaction details, and confirming application exit.

The project goes beyond the original single-file example by separating application orchestration, domain rules, console I/O, and shared utilities. It demonstrates how structures, modular functions, input validation, dynamic memory, and explicit result codes can improve the maintainability of a C project.

## Features

- Starts with an account balance of `1000.00`.
- Records income with a description.
- Records expenses with a description.
- Rejects an expense when the current balance is insufficient.
- Displays each transaction amount, the balance after the transaction, and its description.
- Shows an empty-state message when there are no transactions.
- Requires `y` or `n` confirmation before exit.
- Validates menu choices, amounts, descriptions, and confirmation input.
- Stores money internally as integer cents to avoid floating-point currency errors.
- Uses a dynamically growing array for transaction records.

## Directory Structure

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

## Module Overview

| Module | Responsibility |
| --- | --- |
| `src/main.c` | Application entry point |
| `src/app/` | Orchestrates the application workflow |
| `src/domain/` | Account state, transactions, and business rules |
| `src/shared/` | Shared utilities such as money parsing and formatting |
| `src/ui/` | Console input validation and output |
| `docs/` | Original requirements and feature description |

## Build and Run

The project has no third-party dependencies. It can be built with GCC or Clang.

### GCC

Run the following command from the project root:

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

Run the application:

```bash
./finance-manager
```

On Windows PowerShell, run the generated executable with:

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

## User Flow

After startup, the main menu provides the following operations:

```text
1. View transaction details
2. Record income
3. Record expense
4. Exit
```

### Record Income

Choose `2`, then enter an amount and a description. The account balance is increased and a new income transaction is stored.

### Record Expense

Choose `3`, then enter an amount and a description. The application checks the available balance first. When the balance is insufficient, the transaction is rejected and no record is created.

### View Details

Choose `1` to display transactions in insertion order:

- Transaction type
- Transaction amount
- Balance after the transaction
- Description

### Exit

Choose `4`, then enter `y` to confirm exit or `n` to return to the main menu. Any other input is rejected and requested again.

## Engineering Design

### Layered Responsibilities

- The application layer coordinates user actions.
- The domain layer protects account balance and transaction consistency.
- The UI layer handles console input and output without directly modifying account state.
- The shared layer handles money parsing and formatting.

### Safe Money Representation

The application does not store balances as `double`. It uses `int64_t` cents instead. For example, user input `100.50` is converted to `10050` cents, arithmetic is performed on integers, and the result is formatted back to two decimal places for display.

### Input Validation

The program validates menu ranges, amount syntax, positive amounts, decimal precision, description length, and exit confirmation. It avoids using unchecked `scanf` calls for user input.

### Resource Management

Transactions are stored in a dynamically allocated array that grows when necessary. The application releases the account's allocated memory before exiting.

## Current Scope

- Data is kept in memory only and is lost when the process exits.
- The current version is a single-account, single-process console application.
- There is currently no database, file persistence, authentication, graphical interface, or network API.
- Amount input supports up to two decimal places.
- Transaction descriptions are limited to `127` bytes.

## Possible Extensions

The existing module boundaries provide a foundation for future improvements:

- Add file or database persistence.
- Add queries by date, transaction type, and description.
- Add monthly income, expense, and balance summaries.
- Add budgets and category management.
- Add unit and integration tests.
- Add CMake or Makefile-based build automation.

## References

- [Personal finance manager requirements](docs/家庭收支记账软件.docx)
- [C source code](src/)
