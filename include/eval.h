#ifndef GW_EVAL_H
#define GW_EVAL_H

#include "types.h"

/* Expression evaluator - recursive descent with precedence climbing */
gw_value_t gw_eval(void);       /* evaluate any expression */
gw_value_t gw_eval_num(void);   /* evaluate, require numeric */
gw_value_t gw_eval_str(void);   /* evaluate, require string */
int16_t    gw_eval_int(void);   /* evaluate, require integer result */
uint16_t   gw_eval_uint16(void);

/* Force type conversions */
int16_t  gw_to_int(gw_value_t *v);
float    gw_to_sng(gw_value_t *v);
double   gw_to_dbl(gw_value_t *v);

/* Check for closing paren */
void gw_expect_rparen(void);

#endif
