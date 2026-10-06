#include "money.h"

#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>

bool finance_money_parse(const char *text, FinanceMoney *money)
{
    const char *cursor;
    uint64_t whole_part = 0;
    uint64_t fraction_part = 0;
    int fraction_digits = 0;

    if (text == NULL || money == NULL) {
        return false;
    }

    cursor = text;
    while (isspace((unsigned char)*cursor)) {
        cursor++;
    }

    if (*cursor == '+') {
        cursor++;
    }

    if (*cursor == '-' || !isdigit((unsigned char)*cursor)) {
        return false;
    }

    while (isdigit((unsigned char)*cursor)) {
        const unsigned int digit = (unsigned int)(*cursor - '0');

        if (whole_part > (UINT64_MAX - digit) / 10U) {
            return false;
        }

        whole_part = whole_part * 10U + digit;
        cursor++;
    }

    if (*cursor == '.') {
        cursor++;

        while (isdigit((unsigned char)*cursor)) {
            if (fraction_digits >= 2) {
                return false;
            }

            fraction_part = fraction_part * 10U
                + (uint64_t)(*cursor - '0');
            fraction_digits++;
            cursor++;
        }

        if (fraction_digits == 0) {
            return false;
        }
    }

    while (isspace((unsigned char)*cursor)) {
        cursor++;
    }

    if (*cursor != '\0') {
        return false;
    }

    if (fraction_digits == 1) {
        fraction_part *= 10U;
    }

    if (whole_part > ((uint64_t)INT64_MAX - fraction_part) / 100U) {
        return false;
    }

    *money = (FinanceMoney)(whole_part * 100U + fraction_part);
    return true;
}

bool finance_money_format(
    FinanceMoney money,
    char *buffer,
    size_t buffer_size
)
{
    uint64_t magnitude;
    uint64_t whole_part;
    uint64_t fraction_part;
    const char *sign = "";
    int written;

    if (buffer == NULL || buffer_size == 0) {
        return false;
    }

    if (money < 0) {
        sign = "-";
        magnitude = (uint64_t)(-(money + 1));
        magnitude += 1U;
    } else {
        magnitude = (uint64_t)money;
    }

    whole_part = magnitude / 100U;
    fraction_part = magnitude % 100U;

    written = snprintf(
        buffer,
        buffer_size,
        "%s%" PRIu64 ".%02" PRIu64,
        sign,
        whole_part,
        fraction_part
    );

    return written >= 0 && (size_t)written < buffer_size;
}
