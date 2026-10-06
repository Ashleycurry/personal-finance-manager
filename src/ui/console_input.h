#ifndef PERSONAL_FINANCE_CONSOLE_INPUT_H
#define PERSONAL_FINANCE_CONSOLE_INPUT_H

#include <stdbool.h>
#include <stddef.h>

#include "../shared/money.h"

typedef enum {
    CONSOLE_INPUT_OK = 0,
    CONSOLE_INPUT_EOF,
    CONSOLE_INPUT_INVALID,
    CONSOLE_INPUT_TOO_LONG
} ConsoleInputResult;

/**
 * @brief 读取并校验菜单编号。
 *
 * 函数会持续读取，直到用户输入范围内的整数或遇到输入结束。
 *
 * @param prompt 提示文本。
 * @param minimum 允许的最小值。
 * @param maximum 允许的最大值。
 * @param choice 输出菜单编号。
 * @return 输入处理结果。
 */
ConsoleInputResult console_read_menu_choice(
    const char *prompt,
    int minimum,
    int maximum,
    int *choice
);

/**
 * @brief 读取正数金额。
 *
 * @param prompt 提示文本。
 * @param amount_cents 输出金额，单位为分。
 * @return 输入处理结果。
 */
ConsoleInputResult console_read_money(
    const char *prompt,
    FinanceMoney *amount_cents
);

/**
 * @brief 读取必填文本。
 *
 * @param prompt 提示文本。
 * @param buffer 输出文本缓冲区。
 * @param buffer_size 缓冲区容量。
 * @return 输入处理结果。
 */
ConsoleInputResult console_read_description(
    const char *prompt,
    char *buffer,
    size_t buffer_size
);

/**
 * @brief 读取退出确认。
 *
 * 只接受 y/Y 或 n/N，其他输入会继续提示。
 *
 * @param prompt 提示文本。
 * @param confirmed 输出是否确认。
 * @return 输入处理结果。
 */
ConsoleInputResult console_read_confirmation(
    const char *prompt,
    bool *confirmed
);

#endif
