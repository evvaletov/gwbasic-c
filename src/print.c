#include "gwbasic.h"
#include <string.h>
#include <stdio.h>

static int print_col = 0;

static void print_char(int ch)
{
    if (gw_hal) {
        gw_hal->putch(ch);
    } else {
        putchar(ch);
    }
    if (ch == '\n')
        print_col = 0;
    else
        print_col++;
}

static void print_str(const char *s)
{
    while (*s)
        print_char(*s++);
}

void gw_print_newline(void)
{
    print_char('\n');
}

void gw_print_value(gw_value_t *v)
{
    if (v->type == VT_STR) {
        for (int i = 0; i < v->sval.len; i++)
            print_char(v->sval.data[i]);
        gw_str_free(&v->sval);
    } else {
        char buf[64];
        gw_format_number(v, buf, sizeof(buf));
        print_str(buf);
        /* Trailing space after number */
        print_char(' ');
    }
}

/*
 * PRINT statement - reimplements the PRINT code from GW-BASIC.
 * Handles: PRINT expr, expr; expr, TAB(n), SPC(n)
 */
void gw_stmt_print(void)
{
    int need_newline = 1;
    int screen_width = 80;
    if (gw_hal)
        screen_width = gw_hal->screen_width;

    for (;;) {
        gw_skip_spaces();
        uint8_t tok = gw_chrgot();

        /* End of statement */
        if (tok == 0 || tok == ':' || tok == TOK_ELSE)
            break;

        /* Semicolon - no spacing */
        if (tok == ';') {
            gw_chrget();
            need_newline = 0;
            continue;
        }

        /* Comma - advance to next print zone (14-char zones) */
        if (tok == ',') {
            gw_chrget();
            int zone_width = 14;
            int target = ((print_col / zone_width) + 1) * zone_width;
            if (target >= screen_width) {
                gw_print_newline();
            } else {
                while (print_col < target)
                    print_char(' ');
            }
            need_newline = 0;
            continue;
        }

        /* TAB(n) */
        if (tok == TOK_TAB) {
            gw_chrget();
            /* The '(' is part of the token name */
            int n = gw_eval_int();
            gw_skip_spaces();
            gw_expect_rparen();
            if (n < 1) n = 1;
            n--;  /* convert to 0-based */
            if (n > print_col) {
                while (print_col < n)
                    print_char(' ');
            }
            need_newline = 0;
            continue;
        }

        /* SPC(n) */
        if (tok == TOK_SPC) {
            gw_chrget();
            int n = gw_eval_int();
            gw_skip_spaces();
            gw_expect_rparen();
            if (n < 0) n = 0;
            for (int i = 0; i < n; i++)
                print_char(' ');
            need_newline = 0;
            continue;
        }

        /* Expression */
        gw_value_t v = gw_eval();
        gw_print_value(&v);
        need_newline = 1;
    }

    if (need_newline)
        gw_print_newline();
}
