#ifndef GW_INTERP_H
#define GW_INTERP_H

#include "types.h"
#include <stdbool.h>

/* Interpreter state - the global context */
typedef struct {
    /* Text pointer (TXTPTR equivalent) */
    uint8_t *text_ptr;

    /* Current accumulator (FAC equivalent) */
    gw_value_t fac;

    /* Program state */
    uint16_t cur_line_num;      /* CURLIN - 65535 = direct mode */
    program_line_t *prog_head;  /* first line of program */
    bool trace_on;              /* TRON/TROFF */

    /* Tokenizer buffers */
    uint8_t kbuf[300];          /* crunch buffer */
    char buf[256];              /* input line buffer */

    /* Default variable types for A-Z (DEFTBL) */
    gw_valtype_t def_type[26];

    /* DATA pointer */
    uint8_t *data_ptr;
    uint16_t data_line;
} interp_state_t;

extern interp_state_t gw;

/* Direct mode line number sentinel */
#define LINE_DIRECT 65535

void gw_init(void);
void gw_exec_line(uint8_t *tokens);
void gw_exec_direct(const char *line);

/* CHRGET/CHRGOT - advance/peek the text pointer */
uint8_t gw_chrget(void);
uint8_t gw_chrgot(void);
void gw_skip_spaces(void);
bool gw_is_letter(uint8_t ch);
bool gw_is_digit(uint8_t ch);

/* Expect a specific token, else syntax error */
void gw_expect(uint8_t tok);

#endif
