#ifndef PERSONAL_FINANCE_CONSOLE_VIEW_H
#define PERSONAL_FINANCE_CONSOLE_VIEW_H

#include "../domain/finance_types.h"

/**
 * @brief 展示家庭收支软件主菜单。
 */
void console_view_show_main_menu(void);

/**
 * @brief 展示账户收支明细。
 *
 * @param account 待展示的账户。
 */
void console_view_show_details(const FinanceAccount *account);

/**
 * @brief 展示业务操作错误。
 *
 * @param result 业务处理结果。
 */
void console_view_show_business_error(FinanceResult result);

/**
 * @brief 展示一笔收支登记成功的提示。
 *
 * @param transaction_type 收支类型。
 */
void console_view_show_record_success(FinanceTransactionType transaction_type);

/**
 * @brief 展示程序退出提示。
 */
void console_view_show_exit_message(void);

#endif
