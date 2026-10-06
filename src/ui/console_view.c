#include "console_view.h"

#include <stdio.h>

#include "../domain/finance_account.h"
#include "../shared/money.h"

static const char *transaction_type_name(
    FinanceTransactionType transaction_type
)
{
    switch (transaction_type) {
    case FINANCE_TRANSACTION_INCOME:
        return "收入";
    case FINANCE_TRANSACTION_EXPENSE:
        return "支出";
    default:
        return "未知";
    }
}

void console_view_show_main_menu(void)
{
    puts("\n--------- 家庭收支记账软件 ---------");
    puts("1. 收支明细");
    puts("2. 登记收入");
    puts("3. 登记支出");
    puts("4. 退出");
}

void console_view_show_details(const FinanceAccount *account)
{
    size_t index;
    size_t transaction_count;

    if (account == NULL) {
        puts("账户信息不可用。");
        return;
    }

    transaction_count = finance_account_get_transaction_count(account);
    if (transaction_count == 0U) {
        puts("\n当前没有收支明细... 来一笔吧!");
        return;
    }

    puts("\n---------------- 当前收支明细 ----------------");
    printf("%-8s %-14s %-14s %s\n", "收支", "收支金额", "账户余额", "说明");

    for (index = 0U; index < transaction_count; index++) {
        char amount_text[64];
        char balance_text[64];
        const FinanceTransaction *transaction =
            finance_account_get_transaction(account, index);

        if (transaction == NULL
            || !finance_money_format(
                transaction->amount_cents,
                amount_text,
                sizeof(amount_text)
            )
            || !finance_money_format(
                transaction->balance_after_cents,
                balance_text,
                sizeof(balance_text)
            )) {
            puts("收支明细格式化失败。");
            return;
        }

        printf(
            "%-8s %-14s %-14s %s\n",
            transaction_type_name(transaction->type),
            amount_text,
            balance_text,
            transaction->description
        );
    }
}

void console_view_show_business_error(FinanceResult result)
{
    printf("\n%s\n", finance_result_message(result));
}

void console_view_show_record_success(FinanceTransactionType transaction_type)
{
    printf("\n%s登记成功。\n", transaction_type_name(transaction_type));
}

void console_view_show_exit_message(void)
{
    puts("\n你退出了家庭财务软件~~");
}
