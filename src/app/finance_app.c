#include "finance_app.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../domain/finance_account.h"
#include "../ui/console_input.h"
#include "../ui/console_view.h"

typedef struct {
    FinanceAccount account;
    bool running;
} FinanceApplication;

static void handle_income(FinanceApplication *application)
{
    FinanceMoney amount_cents;
    char description[FINANCE_DESCRIPTION_MAX_LENGTH + 1U];
    ConsoleInputResult input_result;
    FinanceResult finance_result;

    input_result = console_read_money("\n本次收入金额：", &amount_cents);
    if (input_result == CONSOLE_INPUT_EOF) {
        application->running = false;
        return;
    }

    input_result = console_read_description(
        "本次收入说明：",
        description,
        sizeof(description)
    );
    if (input_result == CONSOLE_INPUT_EOF) {
        application->running = false;
        return;
    }

    finance_result = finance_account_record_income(
        &application->account,
        amount_cents,
        description
    );
    if (finance_result != FINANCE_RESULT_OK) {
        console_view_show_business_error(finance_result);
        return;
    }

    console_view_show_record_success(FINANCE_TRANSACTION_INCOME);
}

static void handle_expense(FinanceApplication *application)
{
    FinanceMoney amount_cents;
    char description[FINANCE_DESCRIPTION_MAX_LENGTH + 1U];
    ConsoleInputResult input_result;
    FinanceResult finance_result;

    input_result = console_read_money("\n本次支出金额：", &amount_cents);
    if (input_result == CONSOLE_INPUT_EOF) {
        application->running = false;
        return;
    }

    input_result = console_read_description(
        "本次支出说明：",
        description,
        sizeof(description)
    );
    if (input_result == CONSOLE_INPUT_EOF) {
        application->running = false;
        return;
    }

    finance_result = finance_account_record_expense(
        &application->account,
        amount_cents,
        description
    );
    if (finance_result != FINANCE_RESULT_OK) {
        console_view_show_business_error(finance_result);
        return;
    }

    console_view_show_record_success(FINANCE_TRANSACTION_EXPENSE);
}

static void handle_exit(FinanceApplication *application)
{
    bool confirmed;
    ConsoleInputResult input_result;

    input_result = console_read_confirmation(
        "\n你确定要退出吗(y/n)：",
        &confirmed
    );
    if (input_result == CONSOLE_INPUT_EOF || confirmed) {
        application->running = false;
    }
}

int finance_app_run(void)
{
    FinanceApplication application = {0};
    FinanceResult finance_result;

    finance_result = finance_account_initialize(
        &application.account,
        FINANCE_INITIAL_BALANCE_CENTS
    );
    if (finance_result != FINANCE_RESULT_OK) {
        console_view_show_business_error(finance_result);
        return EXIT_FAILURE;
    }

    application.running = true;
    while (application.running) {
        int menu_choice;
        ConsoleInputResult input_result;

        console_view_show_main_menu();
        input_result = console_read_menu_choice(
            "请选择(1-4)：",
            1,
            4,
            &menu_choice
        );
        if (input_result == CONSOLE_INPUT_EOF) {
            application.running = false;
            break;
        }

        switch (menu_choice) {
        case 1:
            console_view_show_details(&application.account);
            break;
        case 2:
            handle_income(&application);
            break;
        case 3:
            handle_expense(&application);
            break;
        case 4:
            handle_exit(&application);
            break;
        default:
            break;
        }
    }

    finance_account_destroy(&application.account);
    console_view_show_exit_message();
    return EXIT_SUCCESS;
}
