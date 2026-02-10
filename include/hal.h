#ifndef GW_HAL_H
#define GW_HAL_H

#include <stdint.h>
#include <stdbool.h>

/* Hardware Abstraction Layer vtable */
typedef struct hal_ops {
    /* Terminal I/O */
    void (*putch)(int ch);
    void (*puts)(const char *s);
    int  (*getch)(void);           /* blocking read */
    bool (*kbhit)(void);           /* key available? */
    void (*locate)(int row, int col);
    int  (*get_cursor_row)(void);
    int  (*get_cursor_col)(void);
    void (*cls)(void);
    void (*set_width)(int cols);

    /* Terminal properties */
    int  screen_width;
    int  screen_height;

    /* Lifecycle */
    void (*init)(void);
    void (*shutdown)(void);
} hal_ops_t;

extern hal_ops_t *gw_hal;

/* Platform implementations */
hal_ops_t *hal_posix_create(void);

#endif
