#include "finance_account.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static FinanceResult ensure_transaction_capacity(FinanceAccount *account)
{
    size_t new_capacity;
    FinanceTransaction *new_transactions;

    if (account->transaction_count < account->transaction_capacity) {
        return FINANCE_RESULT_OK;
    }

    if (account->transaction_capacity == 0U) {
        new_capacity = FINANCE_INITIAL_TRANSACTION_CAPACITY;
    } else {
        if (account->transaction_capacity > SIZE_MAX / 2U) {
            return FINANCE_RESULT_MEMORY_ALLOCATION;
        }
        new_capacity = account->transaction_capacity * 2U;
    }

    if (new_capacity > SIZE_MAX / sizeof(FinanceTransaction)) {
        return FINANCE_RESULT_MEMORY_ALLOCATION;
    }

    new_transactions = realloc(
        account->transactions,
        new_capacity * sizeof(FinanceTransaction)
    );
    if (new_transactions == NULL) {
        return FINANCE_RESULT_MEMORY_ALLOCATION;
    }

    account->transactions = new_transactions;
    account->transaction_capacity = new_capacity;
    return FINANCE_RESULT_OK;
}

static FinanceResult validate_transaction_input(
    const FinanceAccount *account,
    FinanceMoney amount_cents,
    const char *description
)
{
    if (account == NULL || description == NULL) {
        return FINANCE_RESULT_INVALID_ARGUMENT;
    }

    if (amount_cents <= 0) {
        return FINANCE_RESULT_INVALID_AMOUNT;
    }

    if (description[0] == '\0'
        || strlen(description) > FINANCE_DESCRIPTION_MAX_LENGTH) {
        return FINANCE_RESULT_INVALID_DESCRIPTION;
    }

    return FINANCE_RESULT_OK;
}

static FinanceResult append_transaction(
    FinanceAccount *account,
    FinanceTransactionType type,
    FinanceMoney amount_cents,
    FinanceMoney balance_after_cents,
    const char *description
)
{
    FinanceResult result;
    FinanceTransaction *transaction;

    result = ensure_transaction_capacity(account);
    if (result != FINANCE_RESULT_OK) {
        return result;
    }

    transaction = &account->transactions[account->transaction_count];
    transaction->type = type;
    transaction->amount_cents = amount_cents;
    transaction->balance_after_cents = balance_after_cents;
    strcpy(transaction->description, description);
    account->transaction_count++;

    return FINANCE_RESULT_OK;
}

FinanceResult finance_account_initialize(
    FinanceAccount *account,
    FinanceMoney initial_balance_cents
)
{
    if (account == NULL || initial_balance_cents < 0) {
        return FINANCE_RESULT_INVALID_ARGUMENT;
    }

    account->balance_cents = initial_balance_cents;
    account->transactions = NULL;
    account->transaction_count = 0U;
    account->transaction_capacity = 0U;
    return FINANCE_RESULT_OK;
}

void finance_account_destroy(FinanceAccount *account)
{
    if (account == NULL) {
        return;
    }

    free(account->transactions);
    account->transactions = NULL;
    account->transaction_count = 0U;
    account->transaction_capacity = 0U;
    account->balance_cents = 0;
}

FinanceResult finance_account_record_income(
    FinanceAccount *account,
    FinanceMoney amount_cents,
    const char *description
)
{
    FinanceResult result;
    FinanceMoney new_balance_cents;

    result = validate_transaction_input(account, amount_cents, description);
    if (result != FINANCE_RESULT_OK) {
        return result;
    }

    if (account->balance_cents > INT64_MAX - amount_cents) {
        return FINANCE_RESULT_MONEY_OVERFLOW;
    }

    new_balance_cents = account->balance_cents + amount_cents;
    result = append_transaction(
        account,
        FINANCE_TRANSACTION_INCOME,
        amount_cents,
        new_balance_cents,
        description
    );
    if (result != FINANCE_RESULT_OK) {
        return result;
    }

    account->balance_cents = new_balance_cents;
    return FINANCE_RESULT_OK;
}

FinanceResult finance_account_record_expense(
    FinanceAccount *account,
    FinanceMoney amount_cents,
    const char *description
)
{
    FinanceResult result;
    FinanceMoney new_balance_cents;

    result = validate_transaction_input(account, amount_cents, description);
    if (result != FINANCE_RESULT_OK) {
        return result;
    }

    if (amount_cents > account->balance_cents) {
        return FINANCE_RESULT_INSUFFICIENT_BALANCE;
    }

    new_balance_cents = account->balance_cents - amount_cents;
    result = append_transaction(
        account,
        FINANCE_TRANSACTION_EXPENSE,
        amount_cents,
        new_balance_cents,
        description
    );
    if (result != FINANCE_RESULT_OK) {
        return result;
    }

    account->balance_cents = new_balance_cents;
    return FINANCE_RESULT_OK;
}

FinanceMoney finance_account_get_balance(const FinanceAccount *account)
{
    return account == NULL ? 0 : account->balance_cents;
}

size_t finance_account_get_transaction_count(const FinanceAccount *account)
{
    return account == NULL ? 0U : account->transaction_count;
}

const FinanceTransaction *finance_account_get_transaction(
    const FinanceAccount *account,
    size_t index
)
{
    if (account == NULL || index >= account->transaction_count) {
        return NULL;
    }

    return &account->transactions[index];
}

const char *finance_result_message(FinanceResult result)
{
    switch (result) {
    case FINANCE_RESULT_OK:
        return "操作成功。";
    case FINANCE_RESULT_INVALID_ARGUMENT:
        return "操作参数无效。";
    case FINANCE_RESULT_INVALID_AMOUNT:
        return "金额必须大于 0。";
    case FINANCE_RESULT_INVALID_DESCRIPTION:
        return "说明不能为空或超出长度限制。";
    case FINANCE_RESULT_INSUFFICIENT_BALANCE:
        return "余额不足，无法完成本次支出。";
    case FINANCE_RESULT_MEMORY_ALLOCATION:
        return "系统内存不足，无法保存本次记录。";
    case FINANCE_RESULT_MONEY_OVERFLOW:
        return "金额超出系统可处理范围。";
    default:
        return "发生未知业务错误。";
    }
}
