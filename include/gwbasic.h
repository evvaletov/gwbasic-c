#ifndef GWBASIC_H
#define GWBASIC_H

#include "types.h"
#include "tokens.h"
#include "error.h"
#include "hal.h"
#include "interp.h"
#include "eval.h"
#include "gw_math.h"
#include "strings.h"

#define GW_VERSION "0.1.0"
#define GW_BANNER "GW-BASIC " GW_VERSION

/* Tokenizer */
int  gw_crunch(const char *text, uint8_t *out, int outsize);
void gw_list_line(uint8_t *tokens, int len, char *out, int outsize);

/* PRINT statement */
void gw_stmt_print(void);
void gw_print_value(gw_value_t *v);
void gw_print_newline(void);

#endif
