#ifndef GW_TYPES_H
#define GW_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/* Value types matching GW-BASIC's VALTYP */
typedef enum {
    VT_INT    = 2,   /* 16-bit integer (%) */
    VT_SNG    = 4,   /* single precision (!) */
    VT_DBL    = 8,   /* double precision (#) */
    VT_STR    = 3,   /* string ($) */
} gw_valtype_t;

/* String descriptor - matches original's 3-byte layout concept */
typedef struct {
    uint8_t len;
    char *data;       /* malloc'd, NOT null-terminated */
} gw_string_t;

/* Unified value - the FAC (floating accumulator) equivalent */
typedef struct {
    gw_valtype_t type;
    union {
        int16_t ival;
        float fval;
        double dval;
        gw_string_t sval;
    };
} gw_value_t;

/* Program line stored in memory */
typedef struct program_line {
    struct program_line *next;
    uint16_t num;        /* line number 0-65529 */
    uint16_t len;        /* token data length */
    uint8_t *tokens;     /* tokenized line data */
} program_line_t;

/* Variable entry */
typedef struct {
    char name[2];        /* 2-char name (original GW-BASIC limit) */
    gw_valtype_t type;
    gw_value_t val;
} var_entry_t;

/* MBF (Microsoft Binary Format) types for file compatibility */
typedef struct {
    uint8_t mantissa[3];
    uint8_t exponent;
} mbf_single_t;

typedef struct {
    uint8_t mantissa[7];
    uint8_t exponent;
} mbf_double_t;

#endif
