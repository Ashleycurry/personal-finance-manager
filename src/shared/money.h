#ifndef PERSONAL_FINANCE_MONEY_H
#define PERSONAL_FINANCE_MONEY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief 统一的货币内部表示，单位为分。
 *
 * 使用整数保存金额，避免浮点数参与货币运算时产生精度误差。
 */
typedef int64_t FinanceMoney;

/**
 * @brief 将用户输入的金额文本解析为分。
 *
 * 支持整数金额和最多两位小数，例如 "100"、"100.50"。
 *
 * @param text 待解析的金额文本。
 * @param money 输出解析后的金额，单位为分。
 * @return 解析成功返回 true，否则返回 false。
 */
bool finance_money_parse(const char *text, FinanceMoney *money);

/**
 * @brief 将分格式化为带两位小数的金额文本。
 *
 * @param money 待格式化的金额，单位为分。
 * @param buffer 输出缓冲区。
 * @param buffer_size 输出缓冲区容量。
 * @return 格式化成功返回 true，否则返回 false。
 */
bool finance_money_format(
    FinanceMoney money,
    char *buffer,
    size_t buffer_size
);

#endif
