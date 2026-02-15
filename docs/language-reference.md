# Language Reference

## Data Types

| Type | Suffix | Description |
|------|--------|-------------|
| INTEGER | `%` | 16-bit signed |
| SINGLE | `!` | 32-bit float |
| DOUBLE | `#` | 64-bit float |
| STRING | `$` | Up to 255 bytes |

## Operators

`+`, `-`, `*`, `/`, `^`, `\` (integer div), `MOD`, `AND`, `OR`, `XOR`, `EQV`,
`IMP`, `NOT`, `<`, `=`, `>`, `<=`, `>=`, `<>`

## Numeric Functions

`SGN`, `INT`, `ABS`, `SQR`, `SIN`, `COS`, `TAN`, `ATN`, `LOG`, `EXP`, `RND`,
`FIX`, `CINT`, `CSNG`, `CDBL`

## String Functions

`LEN`, `ASC`, `CHR$`, `VAL`, `STR$`, `LEFT$`, `RIGHT$`, `MID$`, `SPACE$`,
`STRING$`, `HEX$`, `OCT$`, `INSTR`, `INPUT$`

## File Functions

`EOF`, `LOC`, `LOF`

## Pseudo-variables

`ERL`, `ERR`, `CSRLIN`, `INKEY$`, `DATE$`, `TIME$`

## Literals

Decimal, `&H` hex, `&O` octal, `D` exponent (double), `E` exponent (single),
type suffixes (`%`, `!`, `#`)

## Statements

| Category | Statements |
|----------|------------|
| Output | `PRINT`, `LPRINT`, `PRINT USING`, `WRITE`, `CLS` |
| Variables | `LET`, `DIM`, `ERASE`, `SWAP`, `DEFINT`, `DEFSNG`, `DEFDBL`, `DEFSTR` |
| Control flow | `GOTO`, `GOSUB`/`RETURN`, `FOR`/`NEXT`, `IF`/`THEN`/`ELSE`, `WHILE`/`WEND`, `ON...GOTO`, `ON...GOSUB` |
| Input | `INPUT`, `LINE INPUT`, `DATA`/`READ`/`RESTORE`, `INKEY$` |
| Program control | `RUN`, `RUN "file"`, `CONT`, `STOP`, `END`, `NEW`, `LIST`, `CLEAR` |
| Sequential I/O | `OPEN`, `CLOSE`, `PRINT#`, `WRITE#`, `INPUT#`, `LINE INPUT#` |
| Random-access I/O | `FIELD`, `LSET`, `RSET`, `PUT`, `GET`, `CVI`/`CVS`/`CVD`, `MKI$`/`MKS$`/`MKD$` |
| Program I/O | `SAVE`, `LOAD`, `MERGE`, `CHAIN` |
| Error handling | `ON ERROR GOTO`, `RESUME`, `RESUME NEXT`, `RESUME n`, `ERROR`, `ERR`, `ERL` |
| User functions | `DEF FN`, `RANDOMIZE` |
| File management | `KILL`, `NAME` |
| Screen | `LOCATE`, `COLOR`, `WIDTH`, `SCREEN` |
| Graphics | `PSET`, `PRESET`, `LINE`, `CIRCLE`, `DRAW`, `PAINT` |
| Sound | `SOUND`, `BEEP`, `PLAY` (MML parser, PulseAudio backend) |
| Misc | `POKE`, `KEY`, `TRON`/`TROFF`, `OPTION BASE`, `MID$` assignment, `COMMON` |
| System | `SYSTEM` |

## Graphics

Graphics mode is activated with `SCREEN 1` (320x200, 4 colors) or
`SCREEN 2` (640x200, monochrome). Drawing commands render to an internal
framebuffer and output via [Sixel graphics](https://en.wikipedia.org/wiki/Sixel),
which works in terminals like xterm, mlterm, foot, and WezTerm.

### Drawing Commands

- `PSET (x,y), color` / `PRESET (x,y)` — set/reset individual pixels
- `LINE (x1,y1)-(x2,y2), color [,B|BF]` — lines, boxes, filled boxes
- `CIRCLE (cx,cy), r [,color [,start, end [,aspect]]]` — circles and arcs
- `PAINT (x,y), fill, border` — flood fill
- `DRAW string` — turtle graphics mini-language (U/D/L/R/E/F/G/H, M, C, S, B, N)
- `POINT (x,y)` — read pixel color
- `COLOR fg, bg` — set foreground/background colors

### Example

```
SCREEN 1
LINE (0,0)-(319,199), 1
CIRCLE (160,100), 80, 2
PAINT (160,100), 3, 2
```

## Sound

- `SOUND frequency, duration` — play a tone (frequency in Hz, duration in clock ticks)
- `BEEP` — play the default beep
- `PLAY string` — Music Macro Language (MML) string for melodies

Sound output uses PulseAudio when available; commands are silently ignored otherwise.

## Terminal I/O

When stdin is a terminal, the interpreter enters POSIX raw mode for real-time
keyboard polling with `INKEY$` and character-at-a-time input with `INPUT$(n)`.
Piped input is handled normally without raw mode. `INPUT` and `LINE INPUT`
temporarily exit raw mode for cooked-mode line editing.
