#ifndef GW_TOKENS_H
#define GW_TOKENS_H

#include <stdint.h>

/*
 * Token values derived from IBMRES.ASM.
 * Single-byte tokens: 0x80-0xF9
 * Two-byte tokens: 0xFD xx (extended functions), 0xFE xx (extended stmts),
 *                  0xFF xx (built-in functions)
 */

/* Statement tokens (STMDSP) - starting at 128 */
#define TOK_END       0x80  /* 128 */
#define TOK_FOR       0x81  /* 129 */
#define TOK_NEXT      0x82  /* 130 */
#define TOK_DATA      0x83  /* 131 */
#define TOK_INPUT     0x84  /* 132 */
#define TOK_DIM       0x85  /* 133 */
#define TOK_READ      0x86  /* 134 */
#define TOK_LET       0x87  /* 135 */
#define TOK_GOTO      0x88  /* 136 */
#define TOK_RUN       0x89  /* 137 */
#define TOK_IF        0x8A  /* 138 */
#define TOK_RESTORE   0x8B  /* 139 */
#define TOK_GOSUB     0x8C  /* 140 */
#define TOK_RETURN    0x8D  /* 141 */
#define TOK_REM       0x8E  /* 142 */
#define TOK_STOP      0x8F  /* 143 */
#define TOK_PRINT     0x90  /* 144 */
#define TOK_CLEAR     0x91  /* 145 */
#define TOK_LIST      0x92  /* 146 */
#define TOK_NEW       0x93  /* 147 */
#define TOK_ON        0x94  /* 148 */
#define TOK_WAIT      0x95  /* 149 */
#define TOK_DEF       0x96  /* 150 */
#define TOK_POKE      0x97  /* 151 */
#define TOK_CONT      0x98  /* 152 */
/* 153-154: padding (SNERR) */
#define TOK_OUT       0x9B  /* 155 */
#define TOK_LPRINT    0x9C  /* 156 */
#define TOK_LLIST     0x9D  /* 157 */
/* 158: padding */
#define TOK_WIDTH     0x9F  /* 159 */
#define TOK_ELSE      0xA0  /* 160 */
#define TOK_TRON      0xA1  /* 161 */
#define TOK_TROFF     0xA2  /* 162 */
#define TOK_SWAP      0xA3  /* 163 */
#define TOK_ERASE     0xA4  /* 164 */
#define TOK_EDIT      0xA5  /* 165 */
#define TOK_ERROR     0xA6  /* 166 */
#define TOK_RESUME    0xA7  /* 167 */
#define TOK_DELETE    0xA8  /* 168 */
#define TOK_AUTO      0xA9  /* 169 */
#define TOK_RENUM     0xAA  /* 170 */
#define TOK_DEFSTR    0xAB  /* 171 */
#define TOK_DEFINT    0xAC  /* 172 */
#define TOK_DEFSNG    0xAD  /* 173 */
#define TOK_DEFDBL    0xAE  /* 174 */
#define TOK_LINE      0xAF  /* 175 */
#define TOK_WHILE     0xB0  /* 176 */
#define TOK_WEND      0xB1  /* 177 */
#define TOK_CALL      0xB2  /* 178 */
/* 179-181: padding */
#define TOK_WRITE     0xB6  /* 182 */
#define TOK_OPTION    0xB7  /* 183 */
#define TOK_RANDOMIZE 0xB8  /* 184 */
#define TOK_OPEN      0xB9  /* 185 */
#define TOK_CLOSE     0xBA  /* 186 */
#define TOK_LOAD      0xBB  /* 187 */
#define TOK_MERGE     0xBC  /* 188 */
#define TOK_SAVE      0xBD  /* 189 */
#define TOK_COLOR     0xBE  /* 190 */
#define TOK_CLS       0xBF  /* 191 */
#define TOK_MOTOR     0xC0  /* 192 */
#define TOK_BSAVE     0xC1  /* 193 */
#define TOK_BLOAD     0xC2  /* 194 */
#define TOK_SOUND     0xC3  /* 195 */
#define TOK_BEEP      0xC4  /* 196 */
#define TOK_PSET      0xC5  /* 197 */
#define TOK_PRESET    0xC6  /* 198 */
#define TOK_SCREEN    0xC7  /* 199 */
#define TOK_KEY       0xC8  /* 200 */
#define TOK_LOCATE    0xC9  /* 201 */

/* Non-statement keyword tokens */
#define TOK_TO        0xCA  /* 202 */
#define TOK_THEN      0xCB  /* 203 */
#define TOK_TAB       0xCC  /* 204 */
#define TOK_STEP      0xCD  /* 205 */
#define TOK_USR       0xCE  /* 206 */
#define TOK_FN        0xCF  /* 207 */
#define TOK_SPC       0xD0  /* 208 */
#define TOK_NOT       0xD1  /* 209 */
#define TOK_ERL       0xD2  /* 210 */
#define TOK_ERR       0xD3  /* 211 */
#define TOK_STRINGS   0xD4  /* 212 - STRING$ */
#define TOK_USING     0xD5  /* 213 */
#define TOK_INSTR     0xD6  /* 214 */
#define TOK_SQUOTE    0xD7  /* 215 - ' (single quote = REM) */
#define TOK_VARPTR    0xD8  /* 216 */
#define TOK_CSRLIN    0xD9  /* 217 */
#define TOK_POINT     0xDA  /* 218 */
#define TOK_OFF       0xDB  /* 219 */
#define TOK_INKEYS    0xDC  /* 220 - INKEY$ */

/* Operator tokens (after 7-byte gap at 221-227) */
#define TOK_GT        0xE4  /* 228 - > */
#define TOK_EQ        0xE5  /* 229 - = */
#define TOK_LT        0xE6  /* 230 - < */
#define TOK_PLUS      0xE7  /* 231 - + */
#define TOK_MINUS     0xE8  /* 232 - - */
#define TOK_MUL       0xE9  /* 233 - * */
#define TOK_DIV       0xEA  /* 234 - / */
#define TOK_POW       0xEB  /* 235 - ^ */
#define TOK_AND       0xEC  /* 236 */
#define TOK_OR        0xED  /* 237 */
#define TOK_XOR       0xEE  /* 238 */
#define TOK_EQV       0xEF  /* 239 */
#define TOK_IMP       0xF0  /* 240 */
#define TOK_MOD       0xF1  /* 241 */
#define TOK_IDIV      0xF2  /* 242 - \ (integer division) */

/* Multi-byte token prefixes */
#define TOK_PREFIX_FD 0xFD  /* extended functions */
#define TOK_PREFIX_FE 0xFE  /* extended statements */
#define TOK_PREFIX_FF 0xFF  /* built-in functions */

/* Built-in function tokens (prefix 0xFF) */
#define FUNC_LEFT     0x80  /* 128 */
#define FUNC_RIGHT    0x81  /* 129 */
#define FUNC_MID      0x82  /* 130 */
#define FUNC_SGN      0x83  /* 131 */
#define FUNC_INT      0x84  /* 132 */
#define FUNC_ABS      0x85  /* 133 */
#define FUNC_SQR      0x86  /* 134 */
#define FUNC_RND      0x87  /* 135 */
#define FUNC_SIN      0x88  /* 136 */
#define FUNC_LOG      0x89  /* 137 */
#define FUNC_EXP      0x8A  /* 138 */
#define FUNC_COS      0x8B  /* 139 */
#define FUNC_TAN      0x8C  /* 140 */
#define FUNC_ATN      0x8D  /* 141 */
#define FUNC_FRE      0x8E  /* 142 */
#define FUNC_INP      0x8F  /* 143 */
#define FUNC_POS      0x90  /* 144 */
#define FUNC_LEN      0x91  /* 145 */
#define FUNC_STR      0x92  /* 146 */
#define FUNC_VAL      0x93  /* 147 */
#define FUNC_ASC      0x94  /* 148 */
#define FUNC_CHR      0x95  /* 149 */
#define FUNC_PEEK     0x96  /* 150 */
#define FUNC_SPACE    0x97  /* 151 */
#define FUNC_OCT      0x98  /* 152 */
#define FUNC_HEX      0x99  /* 153 */
#define FUNC_LPOS     0x9A  /* 154 */
#define FUNC_CINT     0x9B  /* 155 */
#define FUNC_CSNG     0x9C  /* 156 */
#define FUNC_CDBL     0x9D  /* 157 */
#define FUNC_FIX      0x9E  /* 158 */
#define FUNC_PEN      0x9F  /* 159 */
#define FUNC_STICK    0xA0  /* 160 */
#define FUNC_STRIG    0xA1  /* 161 */
#define FUNC_EOF      0xA2  /* 162 */
#define FUNC_LOC      0xA3  /* 163 */
#define FUNC_LOF      0xA4  /* 164 */

/* Extended statement tokens (prefix 0xFE) */
#define XSTMT_FILES   0x80
#define XSTMT_FIELD   0x81
#define XSTMT_SYSTEM  0x82
#define XSTMT_NAME    0x83
#define XSTMT_LSET    0x84
#define XSTMT_RSET    0x85
#define XSTMT_KILL    0x86
#define XSTMT_PUT     0x87
#define XSTMT_GET     0x88
#define XSTMT_RESET   0x89
#define XSTMT_COMMON  0x8A
#define XSTMT_CHAIN   0x8B
#define XSTMT_DATE    0x8C
#define XSTMT_TIME    0x8D
#define XSTMT_PAINT   0x8E
#define XSTMT_COM     0x8F
#define XSTMT_CIRCLE  0x90
#define XSTMT_DRAW    0x91
#define XSTMT_PLAY    0x92
#define XSTMT_TIMER   0x93
#define XSTMT_ERDEV   0x94
#define XSTMT_IOCTL   0x95
#define XSTMT_CHDIR   0x96
#define XSTMT_MKDIR   0x97
#define XSTMT_RMDIR   0x98
#define XSTMT_SHELL   0x99
#define XSTMT_ENVIRON 0x9A
#define XSTMT_VIEW    0x9B
#define XSTMT_WINDOW  0x9C
#define XSTMT_PMAP    0x9D
#define XSTMT_PALETTE 0x9E
#define XSTMT_LCOPY   0x9F
#define XSTMT_CALLS   0xA0

/* Extended function tokens (prefix 0xFD) */
#define XFUNC_CVI     0x80
#define XFUNC_CVS     0x81
#define XFUNC_CVD     0x82
#define XFUNC_MKI     0x83
#define XFUNC_MKS     0x84
#define XFUNC_MKD     0x85

/* Special token constants - embedded constant prefixes */
#define TOK_COLON      ':'   /* statement separator (not tokenized) */
#define TOK_INT2       0x0E  /* 2-byte integer (int16_t follows) */
#define TOK_INT1       0x0F  /* 1-byte integer 0-255 (uint8_t follows) */
/* 0x11-0x1A: literal integers 0-9 (no data bytes) */
#define TOK_CONST_SNG  0x1C  /* single-precision (4 bytes IEEE follow) */
#define TOK_CONST_DBL  0x1F  /* double-precision (8 bytes IEEE follow) */

/* Keyword table entry */
typedef struct {
    const char *name;
    uint8_t token;       /* token value (or second byte for multi-byte) */
    uint8_t prefix;      /* 0 for single-byte, 0xFD/0xFE/0xFF for multi */
} keyword_entry_t;

extern const keyword_entry_t gw_keywords[];
extern int gw_keyword_count;

const char *gw_token_name(uint8_t prefix, uint8_t token);

#endif
