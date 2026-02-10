#include "gwbasic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Global interpreter state */
interp_state_t gw;
hal_ops_t *gw_hal = NULL;

void gw_init(void)
{
    memset(&gw, 0, sizeof(gw));
    gw.cur_line_num = LINE_DIRECT;

    /* Default types: all single precision (like GW-BASIC) */
    for (int i = 0; i < 26; i++)
        gw.def_type[i] = VT_SNG;
}

/* CHRGET: advance text pointer, skip spaces, return current byte */
uint8_t gw_chrget(void)
{
    gw.text_ptr++;
    while (*gw.text_ptr == ' ')
        gw.text_ptr++;
    return *gw.text_ptr;
}

/* CHRGOT: return current byte without advancing */
uint8_t gw_chrgot(void)
{
    return *gw.text_ptr;
}

void gw_skip_spaces(void)
{
    while (*gw.text_ptr == ' ')
        gw.text_ptr++;
}

bool gw_is_letter(uint8_t ch)
{
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

bool gw_is_digit(uint8_t ch)
{
    return ch >= '0' && ch <= '9';
}

void gw_expect(uint8_t tok)
{
    gw_skip_spaces();
    if (gw_chrgot() != tok)
        gw_error(ERR_SN);
    gw_chrget();
}

/* Execute a tokenized line from the text pointer */
void gw_exec_line(uint8_t *tokens)
{
    gw.text_ptr = tokens;
    gw_skip_spaces();

    uint8_t tok = gw_chrgot();

    /* Empty line */
    if (tok == 0)
        return;

    /* PRINT / ? */
    if (tok == TOK_PRINT || tok == '?') {
        gw_chrget();
        gw_stmt_print();
        return;
    }

    /* LPRINT */
    if (tok == TOK_LPRINT) {
        gw_chrget();
        gw_stmt_print();
        return;
    }

    /* LET (explicit or implicit) */
    if (tok == TOK_LET) {
        gw_chrget();
        /* Fall through to implicit LET */
    }

    /* REM / single-quote - skip rest */
    if (tok == TOK_REM || tok == TOK_SQUOTE)
        return;

    /* CLS */
    if (tok == TOK_CLS) {
        gw_chrget();
        if (gw_hal) gw_hal->cls();
        return;
    }

    /* NEW */
    if (tok == TOK_NEW) {
        gw_init();
        return;
    }

    /* SYSTEM - exit */
    if (tok == TOK_PREFIX_FE) {
        uint8_t *save = gw.text_ptr;
        gw_chrget();
        if (gw_chrgot() == XSTMT_SYSTEM) {
            if (gw_hal) gw_hal->shutdown();
            exit(0);
        }
        gw.text_ptr = save;
    }

    /* END / STOP */
    if (tok == TOK_END || tok == TOK_STOP)
        return;

    /* For Phase 1, anything else is a syntax error */
    gw_error(ERR_SN);
}

/* Execute a raw text line (crunch + dispatch) */
void gw_exec_direct(const char *line)
{
    gw.cur_line_num = LINE_DIRECT;
    int len = gw_crunch(line, gw.kbuf, sizeof(gw.kbuf));
    (void)len;
    gw_exec_line(gw.kbuf);
}

/* Read a line from stdin (returns NULL on EOF) */
static char *read_line(void)
{
    static char buf[256];
    if (fgets(buf, sizeof(buf), stdin) == NULL)
        return NULL;
    /* Strip trailing newline */
    int len = strlen(buf);
    while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r'))
        buf[--len] = '\0';
    return buf;
}

static void print_banner(int interactive)
{
    if (!interactive)
        return;
    const char *banner =
        "GW-BASIC " GW_VERSION "\n"
        "(C) Eremey Valetov 2026. MIT License.\n"
        "Based on Microsoft GW-BASIC assembly source.\n";
    if (gw_hal)
        gw_hal->puts(banner);
    else
        fputs(banner, stdout);
}

int main(int argc, char **argv)
{
    gw_hal = hal_posix_create();
    gw_hal->init();
    gw_init();

    int interactive = isatty(fileno(stdin));

    /* Check for file argument */
    const char *filename = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: gwbasic [options] [file.bas]\n"
                   "Options:\n"
                   "  -h, --help     Show this help\n"
                   "  -v, --version  Show version\n");
            return 0;
        }
        if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            printf("GW-BASIC %s\n", GW_VERSION);
            return 0;
        }
        if (argv[i][0] != '-') {
            filename = argv[i];
        }
    }

    /* TODO: Load and run file if specified */
    (void)filename;

    print_banner(interactive);

    /* Direct mode loop */
    for (;;) {
        if (interactive) {
            fputs("Ok\n", stdout);
            fflush(stdout);
        }

        char *line = read_line();
        if (!line)
            break;

        /* Skip empty lines */
        if (line[0] == '\0')
            continue;

        /* Error recovery point */
        if (setjmp(gw_error_jmp) != 0) {
            /* Returned from error - continue loop */
            continue;
        }

        gw_exec_direct(line);
    }

    if (gw_hal)
        gw_hal->shutdown();
    return 0;
}
