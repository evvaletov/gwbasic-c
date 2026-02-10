#include "gwbasic.h"
#include <limits.h>

int16_t gw_int_add(int16_t a, int16_t b)
{
    int32_t r = (int32_t)a + (int32_t)b;
    if (r < -32768 || r > 32767)
        gw_error(ERR_OV);
    return (int16_t)r;
}

int16_t gw_int_sub(int16_t a, int16_t b)
{
    int32_t r = (int32_t)a - (int32_t)b;
    if (r < -32768 || r > 32767)
        gw_error(ERR_OV);
    return (int16_t)r;
}

int16_t gw_int_mul(int16_t a, int16_t b)
{
    int32_t r = (int32_t)a * (int32_t)b;
    if (r < -32768 || r > 32767)
        gw_error(ERR_OV);
    return (int16_t)r;
}

int16_t gw_int_div(int16_t a, int16_t b)
{
    if (b == 0) gw_error(ERR_DZ);
    if (a == -32768 && b == -1) gw_error(ERR_OV);
    return a / b;
}

int16_t gw_int_mod(int16_t a, int16_t b)
{
    if (b == 0) gw_error(ERR_DZ);
    return a % b;
}

int16_t gw_int_idiv(int16_t a, int16_t b)
{
    return gw_int_div(a, b);
}

int16_t gw_int_neg(int16_t a)
{
    if (a == -32768) gw_error(ERR_OV);
    return -a;
}
