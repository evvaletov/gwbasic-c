#include "hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

static struct termios orig_termios;
static int raw_mode = 0;
static int cursor_row = 0;
static int cursor_col = 0;

static void posix_putch(int ch)
{
    putchar(ch);
    if (ch == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else if (ch == '\r') {
        cursor_col = 0;
    } else {
        cursor_col++;
    }
}

static void posix_puts(const char *s)
{
    while (*s)
        posix_putch(*s++);
}

static int posix_getch(void)
{
    fflush(stdout);
    return getchar();
}

static bool posix_kbhit(void)
{
    /* In cooked mode, always return false */
    if (!raw_mode)
        return false;

    int ch = getchar();
    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }
    return false;
}

static void posix_locate(int row, int col)
{
    printf("\033[%d;%dH", row, col);
    fflush(stdout);
    cursor_row = row - 1;
    cursor_col = col - 1;
}

static int posix_get_cursor_row(void) { return cursor_row; }
static int posix_get_cursor_col(void) { return cursor_col; }

static void posix_cls(void)
{
    printf("\033[2J\033[H");
    fflush(stdout);
    cursor_row = 0;
    cursor_col = 0;
}

static void posix_set_width(int cols)
{
    /* Not much we can do on POSIX without ncurses */
    (void)cols;
}

static void posix_init(void)
{
    /* Disable output buffering for interactive use */
    setbuf(stdout, NULL);
}

static void posix_shutdown(void)
{
    if (raw_mode) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        raw_mode = 0;
    }
}

static hal_ops_t posix_hal = {
    .putch = posix_putch,
    .puts = posix_puts,
    .getch = posix_getch,
    .kbhit = posix_kbhit,
    .locate = posix_locate,
    .get_cursor_row = posix_get_cursor_row,
    .get_cursor_col = posix_get_cursor_col,
    .cls = posix_cls,
    .set_width = posix_set_width,
    .screen_width = 80,
    .screen_height = 25,
    .init = posix_init,
    .shutdown = posix_shutdown,
};

hal_ops_t *hal_posix_create(void)
{
    /* Try to detect terminal size */
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        if (ws.ws_col > 0) posix_hal.screen_width = ws.ws_col;
        if (ws.ws_row > 0) posix_hal.screen_height = ws.ws_row;
    }
    return &posix_hal;
}
