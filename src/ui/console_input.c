#include "console_input.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONSOLE_LINE_BUFFER_SIZE 256U

static void discard_remaining_line(void)
{
    int character;

    do {
        character = getchar();
    } while (character != '\n' && character != EOF);
}

static ConsoleInputResult read_line(
    const char *prompt,
    char *buffer,
    size_t buffer_size
)
{
    char *line_end;

    if (prompt == NULL || buffer == NULL || buffer_size < 2U) {
        return CONSOLE_INPUT_INVALID;
    }

    fputs(prompt, stdout);
    fflush(stdout);

    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return CONSOLE_INPUT_EOF;
    }

    line_end = strpbrk(buffer, "\r\n");
    if (line_end == NULL) {
        discard_remaining_line();
        return CONSOLE_INPUT_TOO_LONG;
    }

    *line_end = '\0';
    return CONSOLE_INPUT_OK;
}

static char *trim_whitespace(char *text)
{
    char *start;
    char *end;

    while (isspace((unsigned char)*text)) {
        text++;
    }

    start = text;
    end = start + strlen(start);
    while (end > start && isspace((unsigned char)end[-1])) {
        end--;
    }
    *end = '\0';

    return start;
}

static bool parse_integer(const char *text, int *value)
{
    char *end;
    long parsed_value;

    if (text == NULL || value == NULL || text[0] == '\0') {
        return false;
    }

    errno = 0;
    parsed_value = strtol(text, &end, 10);
    if (errno == ERANGE
        || end == text
        || *end != '\0'
        || parsed_value < INT_MIN
        || parsed_value > INT_MAX) {
        return false;
    }

    *value = (int)parsed_value;
    return true;
}

ConsoleInputResult console_read_menu_choice(
    const char *prompt,
    int minimum,
    int maximum,
    int *choice
)
{
    char line[CONSOLE_LINE_BUFFER_SIZE];
    char *normalized_line;
    int parsed_choice;
    ConsoleInputResult result;

    if (minimum > maximum || choice == NULL) {
        return CONSOLE_INPUT_INVALID;
    }

    for (;;) {
        result = read_line(prompt, line, sizeof(line));
        if (result == CONSOLE_INPUT_EOF) {
            return result;
        }
        if (result == CONSOLE_INPUT_TOO_LONG) {
            puts("输入内容过长，请重新输入。");
            continue;
        }

        normalized_line = trim_whitespace(line);
        if (parse_integer(normalized_line, &parsed_choice)
            && parsed_choice >= minimum
            && parsed_choice <= maximum) {
            *choice = parsed_choice;
            return CONSOLE_INPUT_OK;
        }

        printf("请输入 %d-%d 之间的整数。\n", minimum, maximum);
    }
}

ConsoleInputResult console_read_money(
    const char *prompt,
    FinanceMoney *amount_cents
)
{
    char line[CONSOLE_LINE_BUFFER_SIZE];
    char *normalized_line;
    ConsoleInputResult result;

    if (amount_cents == NULL) {
        return CONSOLE_INPUT_INVALID;
    }

    for (;;) {
        result = read_line(prompt, line, sizeof(line));
        if (result == CONSOLE_INPUT_EOF) {
            return result;
        }
        if (result == CONSOLE_INPUT_TOO_LONG) {
            puts("金额输入过长，请重新输入。");
            continue;
        }

        normalized_line = trim_whitespace(line);
        if (finance_money_parse(normalized_line, amount_cents)
            && *amount_cents > 0) {
            return CONSOLE_INPUT_OK;
        }

        puts("请输入大于 0 且最多包含两位小数的金额。");
    }
}

ConsoleInputResult console_read_description(
    const char *prompt,
    char *buffer,
    size_t buffer_size
)
{
    char line[CONSOLE_LINE_BUFFER_SIZE];
    char *normalized_line;
    ConsoleInputResult result;

    if (buffer == NULL || buffer_size < 2U) {
        return CONSOLE_INPUT_INVALID;
    }

    for (;;) {
        result = read_line(prompt, line, sizeof(line));
        if (result == CONSOLE_INPUT_EOF) {
            return result;
        }
        if (result == CONSOLE_INPUT_TOO_LONG) {
            puts("说明内容过长，请重新输入。");
            continue;
        }

        normalized_line = trim_whitespace(line);
        if (normalized_line[0] == '\0') {
            puts("说明不能为空，请重新输入。");
            continue;
        }
        if (strlen(normalized_line) >= buffer_size) {
            puts("说明内容超出长度限制，请重新输入。");
            continue;
        }

        strcpy(buffer, normalized_line);
        return CONSOLE_INPUT_OK;
    }
}

ConsoleInputResult console_read_confirmation(
    const char *prompt,
    bool *confirmed
)
{
    char line[CONSOLE_LINE_BUFFER_SIZE];
    char *normalized_line;
    ConsoleInputResult result;

    if (confirmed == NULL) {
        return CONSOLE_INPUT_INVALID;
    }

    for (;;) {
        result = read_line(prompt, line, sizeof(line));
        if (result == CONSOLE_INPUT_EOF) {
            return result;
        }
        if (result == CONSOLE_INPUT_TOO_LONG) {
            puts("输入内容过长，请输入 y 或 n。");
            continue;
        }

        normalized_line = trim_whitespace(line);
        if (strlen(normalized_line) == 1U) {
            const char choice = (char)tolower(
                (unsigned char)normalized_line[0]
            );
            if (choice == 'y' || choice == 'n') {
                *confirmed = choice == 'y';
                return CONSOLE_INPUT_OK;
            }
        }

        puts("输入有误，请输入 y 或 n。");
    }
}
