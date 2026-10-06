#ifndef PERSONAL_FINANCE_TYPES_H
#define PERSONAL_FINANCE_TYPES_H

#include <stddef.h>

#include "../shared/money.h"

#define FINANCE_INITIAL_BALANCE_CENTS ((FinanceMoney)100000)
#define FINANCE_DESCRIPTION_MAX_LENGTH 127U
#define FINANCE_INITIAL_TRANSACTION_CAPACITY 8U

/**
 * @brief 收支记录的业务类型。
 */
typedef enum {
    FINANCE_TRANSACTION_INCOME = 1,
    FINANCE_TRANSACTION_EXPENSE = 2
} FinanceTransactionType;

/**
 * @brief 账户操作结果。
 */
typedef enum {
    FINANCE_RESULT_OK = 0,
    FINANCE_RESULT_INVALID_ARGUMENT,
    FINANCE_RESULT_INVALID_AMOUNT,
    FINANCE_RESULT_INVALID_DESCRIPTION,
    FINANCE_RESULT_INSUFFICIENT_BALANCE,
    FINANCE_RESULT_MEMORY_ALLOCATION,
    FINANCE_RESULT_MONEY_OVERFLOW
} FinanceResult;

/**
 * @brief 单笔收支记录。
 */
typedef struct {
    FinanceTransactionType type;
    FinanceMoney amount_cents;
    FinanceMoney balance_after_cents;
    char description[FINANCE_DESCRIPTION_MAX_LENGTH + 1U];
} FinanceTransaction;

/**
 * @brief 家庭账户聚合根。
 *
 * 账户余额和交易记录由同一个对象维护，确保每次业务变更后数据保持一致。
 */
typedef struct {
    FinanceMoney balance_cents;
    FinanceTransaction *transactions;
    size_t transaction_count;
    size_t transaction_capacity;
} FinanceAccount;

#endif
