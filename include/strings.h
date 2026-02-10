#ifndef GW_STRINGS_H
#define GW_STRINGS_H

#include "types.h"
#include <stdint.h>

/* String creation/management */
gw_string_t gw_str_alloc(int len);
gw_string_t gw_str_from_cstr(const char *s);
gw_string_t gw_str_copy(gw_string_t *s);
void gw_str_free(gw_string_t *s);
char *gw_str_to_cstr(gw_string_t *s);  /* caller must free */

/* String functions */
gw_value_t gw_fn_len(gw_value_t *s);
gw_value_t gw_fn_left(gw_value_t *s, int n);
gw_value_t gw_fn_right(gw_value_t *s, int n);
gw_value_t gw_fn_mid(gw_value_t *s, int start, int len);
gw_value_t gw_fn_chr(int code);
gw_value_t gw_fn_asc(gw_value_t *s);
gw_value_t gw_fn_val(gw_value_t *s);
gw_value_t gw_fn_str(gw_value_t *v);
gw_value_t gw_fn_space(int n);
gw_value_t gw_fn_strings(int n, int code);
gw_value_t gw_fn_instr(int start, gw_value_t *haystack, gw_value_t *needle);
gw_value_t gw_fn_hex(int16_t n);
gw_value_t gw_fn_oct(int16_t n);

/* Concatenation */
gw_value_t gw_str_concat(gw_value_t *a, gw_value_t *b);

#endif
