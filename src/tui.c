#include "tui.h"
#include "hal.h"
#include "gwbasic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

tui_state_t tui;

/* Saved original HAL pointers for passthrough in non-TUI mode */
static void (*orig_putch)(int);
static void (*orig_puts)(const char *);
static void (*orig_cls)(void);
static void (*orig_locate)(int, int);
static int  (*orig_get_cursor_row)(void);
static int  (*orig_get_cursor_col)(void);

/* Default F-key definitions matching GW-BASIC */
static const char *default_fkeys[10] = {
    "LIST ",          /* F1 */
    "RUN\r",          /* F2 */
    "LOAD\"",         /* F3 */
    "SAVE\"",         /* F4 */
    "CONT\r",         /* F5 */
    ",\"LPT1:\"\r",   /* F6 */
    "TRON\r",         /* F7 */
    "TROFF\r",        /* F8 */
    "KEY ",           /* F9 */
    "SCREEN 0,0,0\r", /* F10 */
};

static void scroll_up(void)
{
    int bottom = tui.view_bottom;
    memmove(&tui.screen[0], &tui.screen[1],
            bottom * sizeof(tui.screen[0]));
    /* Clear the bottom row */
    for (int c = 0; c < TUI_COLS; c++) {
        tui.screen[bottom][c].ch = ' ';
        tui.screen[bottom][c].attr = tui.current_attr;
    }
}

static void advance_cursor(void)
{
    tui.cursor_col++;
    if (tui.cursor_col >= TUI_COLS) {
        tui.cursor_col = 0;
        tui.cursor_row++;
        if (tui.cursor_row > tui.view_bottom) {
            scroll_up();
            tui.cursor_row = tui.view_bottom;
        }
    }
}

void tui_putch(int ch)
{
    if (ch == '\n') {
        tui.cursor_col = 0;
        tui.cursor_row++;
        if (tui.cursor_row > tui.view_bottom) {
            scroll_up();
            tui.cursor_row = tui.view_bottom;
            tui_refresh();
        }
        return;
    }
    if (ch == '\r') {
        tui.cursor_col = 0;
        return;
    }
    if (ch == '\b') {
        if (tui.cursor_col > 0) {
            tui.cursor_col--;
            tui.screen[tui.cursor_row][tui.cursor_col].ch = ' ';
            tui.screen[tui.cursor_row][tui.cursor_col].attr = tui.current_attr;
        }
        return;
    }
    if (ch == '\a') return;  /* bell */
    if (ch == '\t') {
        int target = (tui.cursor_col + 8) & ~7;
        while (tui.cursor_col < target && tui.cursor_col < TUI_COLS)
            tui_putch(' ');
        return;
    }

    tui.screen[tui.cursor_row][tui.cursor_col].ch = (uint8_t)ch;
    tui.screen[tui.cursor_row][tui.cursor_col].attr = tui.current_attr;
    advance_cursor();
}

void tui_puts(const char *s)
{
    while (*s)
        tui_putch((unsigned char)*s++);
    tui_refresh();
    tui_update_cursor();
}

void tui_cls(void)
{
    for (int r = 0; r < TUI_ROWS; r++)
        for (int c = 0; c < TUI_COLS; c++) {
            tui.screen[r][c].ch = ' ';
            tui.screen[r][c].attr = tui.current_attr;
        }
    tui.cursor_row = 0;
    tui.cursor_col = 0;
    tui_refresh();
    tui_update_cursor();
}

void tui_locate(int row, int col)
{
    /* GW-BASIC LOCATE is 1-based */
    tui.cursor_row = row - 1;
    tui.cursor_col = col - 1;
    if (tui.cursor_row < 0) tui.cursor_row = 0;
    if (tui.cursor_col < 0) tui.cursor_col = 0;
    if (tui.cursor_row >= TUI_ROWS) tui.cursor_row = TUI_ROWS - 1;
    if (tui.cursor_col >= TUI_COLS) tui.cursor_col = TUI_COLS - 1;
    tui_update_cursor();
}

int tui_get_cursor_row(void) { return tui.cursor_row; }
int tui_get_cursor_col(void) { return tui.cursor_col; }

/* Render the entire screen buffer to the terminal */
void tui_refresh(void)
{
    /* Move to top-left */
    printf("\033[H");

    int bottom = tui.key_bar_visible ? TUI_ROWS : tui.view_bottom + 1;

    for (int r = 0; r < bottom; r++) {
        printf("\033[%d;1H", r + 1);
        for (int c = 0; c < TUI_COLS; c++) {
            uint8_t ch = tui.screen[r][c].ch;
            putchar(ch ? ch : ' ');
        }
    }

    /* Draw the function key bar if visible */
    if (tui.key_bar_visible)
        tui_refresh_row(TUI_ROWS - 1);

    fflush(stdout);
}

void tui_refresh_row(int row)
{
    if (row < 0 || row >= TUI_ROWS) return;
    printf("\033[%d;1H", row + 1);
    for (int c = 0; c < TUI_COLS; c++) {
        uint8_t ch = tui.screen[row][c].ch;
        putchar(ch ? ch : ' ');
    }
    fflush(stdout);
}

void tui_update_cursor(void)
{
    printf("\033[%d;%dH", tui.cursor_row + 1, tui.cursor_col + 1);
    fflush(stdout);
}

/* Read a key, parsing escape sequences for special keys */
int tui_read_key(void)
{
    gw_hal->enable_raw();

    /* Use HAL's getch for the first byte */
    int ch = gw_hal->getch();
    if (ch < 0) return -1;

    /* Check for Ctrl+C / Ctrl+Break */
    if (ch == 3) return TK_CTRL_C;

    /* Not an escape sequence */
    if (ch != 27) return ch;

    /* Escape sequence: read next byte with timeout */
    /* Check if there's more data */
    if (!gw_hal->kbhit()) {
        /* Just bare Escape key */
        return TK_ESCAPE;
    }

    int seq1 = gw_hal->getch();
    if (seq1 < 0) return TK_ESCAPE;

    if (seq1 == '[') {
        /* CSI sequence */
        int seq2 = gw_hal->getch();
        if (seq2 < 0) return TK_ESCAPE;

        switch (seq2) {
        case 'A': return TK_UP;
        case 'B': return TK_DOWN;
        case 'C': return TK_RIGHT;
        case 'D': return TK_LEFT;
        case 'H': return TK_HOME;
        case 'F': return TK_END;
        default:
            if (seq2 >= '0' && seq2 <= '9') {
                /* Extended sequence like \033[2~ (Insert) */
                int seq3 = gw_hal->getch();
                if (seq3 == '~') {
                    switch (seq2) {
                    case '2': return TK_INSERT;
                    case '3': return TK_DELETE;
                    case '5': return TK_PGUP;
                    case '6': return TK_PGDN;
                    }
                } else if (seq2 == '1' && seq3 >= '0' && seq3 <= '9') {
                    /* F5-F12: \033[15~ through \033[24~ */
                    int seq4 = gw_hal->getch();
                    if (seq4 == '~') {
                        int code = (seq2 - '0') * 10 + (seq3 - '0');
                        switch (code) {
                        case 15: return TK_F5;
                        case 17: return TK_F6;
                        case 18: return TK_F7;
                        case 19: return TK_F8;
                        case 20: return TK_F9;
                        case 21: return TK_F10;
                        }
                    }
                } else if (seq2 == '2' && seq3 >= '0' && seq3 <= '9') {
                    int seq4 = gw_hal->getch();
                    (void)seq4;
                    /* F10+, ignore for now */
                }
            }
            break;
        }
    } else if (seq1 == 'O') {
        /* SS3 sequences for F1-F4 */
        int seq2 = gw_hal->getch();
        switch (seq2) {
        case 'P': return TK_F1;
        case 'Q': return TK_F2;
        case 'R': return TK_F3;
        case 'S': return TK_F4;
        case 'H': return TK_HOME;
        case 'F': return TK_END;
        }
    }

    return TK_ESCAPE;
}

/* Extract a logical line from the screen buffer at the given row */
static int extract_screen_line(int row, char *buf, int bufsz)
{
    int len = 0;
    /* Scan from col 0 to TUI_COLS-1, trimming trailing spaces */
    for (int c = 0; c < TUI_COLS && len < bufsz - 1; c++) {
        buf[len++] = tui.screen[row][c].ch ? tui.screen[row][c].ch : ' ';
    }
    /* Trim trailing spaces */
    while (len > 0 && buf[len - 1] == ' ')
        len--;
    buf[len] = '\0';
    return len;
}

/* The main line editor — implements GW-BASIC's screen editing behavior */
char *tui_read_line(void)
{
    static char line_buf[TUI_MAX_LINE + 1];
    int enter_row = tui.cursor_row;

    tui_update_cursor();

    for (;;) {
        int key = tui_read_key();
        if (key < 0) return NULL;

        /* Check for Ctrl+Break */
        if (key == TK_CTRL_C || tui.break_flag) {
            tui.break_flag = false;
            line_buf[0] = '\0';
            return NULL;
        }

        /* Function keys: inject definition string */
        if (key >= TK_F1 && key <= TK_F10) {
            int fk = key - TK_F1;
            const char *def = tui.fkey_defs[fk];
            for (const char *p = def; *p; p++) {
                if (*p == '\r') {
                    /* Auto-enter: extract line from screen and return */
                    tui_putch('\n');
                    tui_refresh();
                    extract_screen_line(enter_row, line_buf, sizeof(line_buf));
                    tui_update_cursor();
                    return line_buf;
                }
                tui.screen[tui.cursor_row][tui.cursor_col].ch = (uint8_t)*p;
                tui.screen[tui.cursor_row][tui.cursor_col].attr = tui.current_attr;
                advance_cursor();
            }
            tui_refresh_row(tui.cursor_row);
            tui_update_cursor();
            continue;
        }

        switch (key) {
        case TK_ENTER:
            /* Extract the line content from the screen at enter_row */
            extract_screen_line(enter_row, line_buf, sizeof(line_buf));
            tui.cursor_col = 0;
            tui.cursor_row++;
            if (tui.cursor_row > tui.view_bottom) {
                scroll_up();
                tui.cursor_row = tui.view_bottom;
            }
            tui_refresh();
            tui_update_cursor();
            return line_buf;

        case TK_BACKSPACE:
        case 127:  /* DEL key on some terminals */
            if (tui.cursor_col > 0) {
                tui.cursor_col--;
                /* Shift characters left */
                for (int c = tui.cursor_col; c < TUI_COLS - 1; c++)
                    tui.screen[tui.cursor_row][c] = tui.screen[tui.cursor_row][c + 1];
                tui.screen[tui.cursor_row][TUI_COLS - 1].ch = ' ';
                tui.screen[tui.cursor_row][TUI_COLS - 1].attr = tui.current_attr;
                tui_refresh_row(tui.cursor_row);
            }
            tui_update_cursor();
            break;

        case TK_DELETE:
            /* Shift characters left from current position */
            for (int c = tui.cursor_col; c < TUI_COLS - 1; c++)
                tui.screen[tui.cursor_row][c] = tui.screen[tui.cursor_row][c + 1];
            tui.screen[tui.cursor_row][TUI_COLS - 1].ch = ' ';
            tui.screen[tui.cursor_row][TUI_COLS - 1].attr = tui.current_attr;
            tui_refresh_row(tui.cursor_row);
            tui_update_cursor();
            break;

        case TK_LEFT:
            if (tui.cursor_col > 0)
                tui.cursor_col--;
            else if (tui.cursor_row > 0) {
                tui.cursor_row--;
                tui.cursor_col = TUI_COLS - 1;
            }
            tui_update_cursor();
            break;

        case TK_RIGHT:
            if (tui.cursor_col < TUI_COLS - 1)
                tui.cursor_col++;
            else if (tui.cursor_row < tui.view_bottom) {
                tui.cursor_row++;
                tui.cursor_col = 0;
            }
            tui_update_cursor();
            break;

        case TK_UP:
            if (tui.cursor_row > 0)
                tui.cursor_row--;
            tui_update_cursor();
            break;

        case TK_DOWN:
            if (tui.cursor_row < tui.view_bottom)
                tui.cursor_row++;
            tui_update_cursor();
            break;

        case TK_HOME:
            tui.cursor_col = 0;
            tui_update_cursor();
            break;

        case TK_END: {
            /* Move to end of content on this row */
            int c = TUI_COLS - 1;
            while (c > 0 && tui.screen[tui.cursor_row][c].ch == ' ')
                c--;
            if (tui.screen[tui.cursor_row][c].ch != ' ')
                c++;
            if (c >= TUI_COLS) c = TUI_COLS - 1;
            tui.cursor_col = c;
            tui_update_cursor();
            break;
        }

        case TK_CTRL_HOME:
            tui_cls();
            enter_row = 0;
            break;

        case TK_INSERT:
            tui.insert_mode = !tui.insert_mode;
            if (tui.insert_mode)
                tui_set_cursor_block();
            else
                tui_set_cursor_line();
            break;

        case TK_ESCAPE:
            /* Clear current line content from cursor to end */
            for (int c = tui.cursor_col; c < TUI_COLS; c++) {
                tui.screen[tui.cursor_row][c].ch = ' ';
                tui.screen[tui.cursor_row][c].attr = tui.current_attr;
            }
            tui_refresh_row(tui.cursor_row);
            tui_update_cursor();
            break;

        default:
            /* Printable character */
            if (key >= 32 && key < 127) {
                if (tui.insert_mode && tui.cursor_col < TUI_COLS - 1) {
                    /* Shift right */
                    for (int c = TUI_COLS - 1; c > tui.cursor_col; c--)
                        tui.screen[tui.cursor_row][c] = tui.screen[tui.cursor_row][c - 1];
                }
                tui.screen[tui.cursor_row][tui.cursor_col].ch = (uint8_t)key;
                tui.screen[tui.cursor_row][tui.cursor_col].attr = tui.current_attr;
                /* Track which row the line started on */
                if (tui.cursor_row != enter_row && tui.cursor_col == 0) {
                    enter_row = tui.cursor_row;
                }
                advance_cursor();
                tui_refresh_row(tui.cursor_row);
                tui_update_cursor();
            }
            break;
        }
    }
}

/* Function key bar */
static void render_key_bar(void)
{
    int row = TUI_ROWS - 1;
    /* Clear the bar row */
    for (int c = 0; c < TUI_COLS; c++) {
        tui.screen[row][c].ch = ' ';
        tui.screen[row][c].attr = 0x70;  /* reverse video: black on white */
    }

    int col = 0;
    for (int i = 0; i < 10 && col < TUI_COLS; i++) {
        /* Key number */
        char label[4];
        snprintf(label, sizeof(label), "%d", i + 1);
        for (const char *p = label; *p && col < TUI_COLS; p++) {
            tui.screen[row][col].ch = *p;
            tui.screen[row][col].attr = 0x07;  /* normal: white on black */
            col++;
        }
        /* Key definition (truncated to fit) */
        const char *def = tui.fkey_defs[i];
        int maxw = 6;  /* each key slot is ~8 chars total */
        for (int j = 0; j < maxw && def[j] && def[j] != '\r' && col < TUI_COLS; j++) {
            tui.screen[row][col].ch = (uint8_t)def[j];
            tui.screen[row][col].attr = 0x70;  /* reverse */
            col++;
        }
        /* Padding */
        if (col < TUI_COLS) {
            tui.screen[row][col].ch = ' ';
            tui.screen[row][col].attr = 0x07;
            col++;
        }
    }

    tui_refresh_row(row);
}

void tui_key_on(void)
{
    tui.key_bar_visible = true;
    tui.view_bottom = TUI_ROWS - 2;  /* row 23 (0-based) */
    render_key_bar();
    tui_update_cursor();
}

void tui_key_off(void)
{
    tui.key_bar_visible = false;
    tui.view_bottom = TUI_ROWS - 1;  /* row 24 */
    /* Clear the bar row */
    for (int c = 0; c < TUI_COLS; c++) {
        tui.screen[TUI_ROWS - 1][c].ch = ' ';
        tui.screen[TUI_ROWS - 1][c].attr = tui.current_attr;
    }
    tui_refresh_row(TUI_ROWS - 1);
    tui_update_cursor();
}

void tui_key_list(void)
{
    for (int i = 0; i < 10; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "F%d ", i + 1);
        tui_puts(buf);
        /* Print the definition, showing \r as visible marker */
        for (const char *p = tui.fkey_defs[i]; *p; p++) {
            if (*p == '\r')
                tui_putch(0x0D);  /* show as a special char, or just skip */
            else
                tui_putch(*p);
        }
        tui_putch('\n');
    }
    tui_refresh();
    tui_update_cursor();
}

/* Cursor shape via ANSI escape sequences */
void tui_set_cursor_block(void)
{
    printf("\033[1 q");
    fflush(stdout);
}

void tui_set_cursor_line(void)
{
    printf("\033[5 q");
    fflush(stdout);
}

/* Ctrl+Break handler */
static void sigint_handler(int sig)
{
    (void)sig;
    tui.break_flag = true;
}

void tui_install_break_handler(void)
{
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
}

void tui_check_break(void)
{
    if (tui.break_flag) {
        tui.break_flag = false;
        tui_puts("\nBreak\n");
        gw.running = false;
        longjmp(gw_error_jmp, -1);
    }
}

/* Initialize the TUI subsystem */
void tui_init(void)
{
    memset(&tui, 0, sizeof(tui));
    tui.current_attr = 0x07;  /* white on black */
    tui.view_bottom = TUI_ROWS - 1;
    tui.insert_mode = false;
    tui.active = true;

    /* Set default F-key definitions */
    for (int i = 0; i < 10; i++)
        strncpy(tui.fkey_defs[i], default_fkeys[i], sizeof(tui.fkey_defs[i]) - 1);

    /* Clear screen buffer */
    for (int r = 0; r < TUI_ROWS; r++)
        for (int c = 0; c < TUI_COLS; c++) {
            tui.screen[r][c].ch = ' ';
            tui.screen[r][c].attr = tui.current_attr;
        }

    /* Save original HAL pointers */
    orig_putch = gw_hal->putch;
    orig_puts = gw_hal->puts;
    orig_cls = gw_hal->cls;
    orig_locate = gw_hal->locate;
    orig_get_cursor_row = gw_hal->get_cursor_row;
    orig_get_cursor_col = gw_hal->get_cursor_col;

    /* Swap in TUI handlers */
    gw_hal->putch = tui_putch;
    gw_hal->puts = tui_puts;
    gw_hal->cls = tui_cls;
    gw_hal->locate = tui_locate;
    gw_hal->get_cursor_row = tui_get_cursor_row;
    gw_hal->get_cursor_col = tui_get_cursor_col;

    /* Install break handler */
    tui_install_break_handler();

    /* Enter alternate screen buffer, clear, hide cursor initially */
    printf("\033[?1049h");  /* alternate screen buffer */
    printf("\033[2J\033[H"); /* clear */
    tui_set_cursor_line();
    fflush(stdout);

    /* Show KEY bar by default (matches real GW-BASIC) */
    tui_key_on();
}

void tui_shutdown(void)
{
    if (!tui.active) return;
    tui.active = false;

    /* Restore original HAL pointers */
    gw_hal->putch = orig_putch;
    gw_hal->puts = orig_puts;
    gw_hal->cls = orig_cls;
    gw_hal->locate = orig_locate;
    gw_hal->get_cursor_row = orig_get_cursor_row;
    gw_hal->get_cursor_col = orig_get_cursor_col;

    /* Leave alternate screen buffer, restore cursor */
    printf("\033[?1049l");
    printf("\033[0 q");  /* reset cursor shape */
    fflush(stdout);

    /* Restore default SIGINT */
    signal(SIGINT, SIG_DFL);
}
