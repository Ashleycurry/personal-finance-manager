#ifndef PERSONAL_FINANCE_ACCOUNT_H
#define PERSONAL_FINANCE_ACCOUNT_H

#include <stddef.h>

#include "finance_types.h"

/**
 * @brief 初始化家庭账户。
 *
 * @param account 待初始化的账户对象。
 * @param initial_balance_cents 初始余额，单位为分。
 * @return 初始化结果。
 */
FinanceResult finance_account_initialize(
    FinanceAccount *account,
    FinanceMoney initial_balance_cents
);

/**
 * @brief 释放账户持有的动态资源。
 *
 * @param account 待销毁的账户对象。
 */
void finance_account_destroy(FinanceAccount *account);

/**
 * @brief 登记一笔收入。
 *
 * @param account 目标账户。
 * @param amount_cents 收入金额，单位为分。
 * @param description 收入说明。
 * @return 业务处理结果。
 */
FinanceResult finance_account_record_income(
    FinanceAccount *account,
    FinanceMoney amount_cents,
    const char *description
);

/**
 * @brief 登记一笔支出。
 *
 * @param account 目标账户。
 * @param amount_cents 支出金额，单位为分。
 * @param description 支出说明。
 * @return 业务处理结果。
 */
FinanceResult finance_account_record_expense(
    FinanceAccount *account,
    FinanceMoney amount_cents,
    const char *description
);

/**
 * @brief 获取账户当前余额。
 *
 * @param account 目标账户。
 * @return 当前余额，单位为分；传入空指针时返回 0。
 */
FinanceMoney finance_account_get_balance(const FinanceAccount *account);

/**
 * @brief 获取账户交易数量。
 *
 * @param account 目标账户。
 * @return 交易数量；传入空指针时返回 0。
 */
size_t finance_account_get_transaction_count(const FinanceAccount *account);

/**
 * @brief 按索引读取一笔交易记录。
 *
 * @param account 目标账户。
 * @param index 交易索引。
 * @return 只读交易记录指针；索引无效时返回 NULL。
 */
const FinanceTransaction *finance_account_get_transaction(
    const FinanceAccount *account,
    size_t index
);

/**
 * @brief 返回业务结果对应的中文提示。
 *
 * @param result 业务处理结果。
 * @return 稳定的中文提示文本。
 */
const char *finance_result_message(FinanceResult result);

#endif
