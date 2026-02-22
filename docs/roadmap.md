# Roadmap

## Planned Features

- **PRINT USING edge cases** — Thousands separator (`,`), `**` asterisk fill,
  `**$` combined, `^^^^` scientific notation, `&` full-string format
- **Binary SAVE/LOAD** — Protected (,P) and tokenized binary formats
- **DEF SEG / PEEK / POKE** — Memory-mapped I/O emulation for common BIOS/screen
  addresses
- **GET/PUT** — Sprite capture and blit for graphics mode
- **TUI color support** — Map GW-BASIC COLOR attributes to ANSI 16-color output
- **INKEY$ extended keys** — Return CHR$(0) + scan code for arrow keys and
  function keys, matching the original two-byte encoding

## IDE and Notebook Integration

- **Jupyter kernel for GW-BASIC** — A Jupyter Notebook kernel that runs
  GW-BASIC programs cell-by-cell, with rich output for `PRINT`, inline graphics
  rendering for drawing commands, and interactive `INPUT` via notebook widgets.
  Similar in spirit to [foxkernel](https://github.com/evvaletov/foxkernel).
- **JetBrains plugin (IntelliJ/CLion)** — Full-featured language plugin with
  syntax highlighting, code completion, line number navigation, run
  configurations, debugger integration (breakpoints via `STOP`, variable
  inspection), structure view (line number outline), and error annotations.
- **VS Code extension** — Language extension providing syntax highlighting
  (TextMate grammar), snippets, run/debug tasks, integrated terminal runner,
  and Language Server Protocol support for diagnostics and hover info.

## Known Limitations

- No binary/protected file format support (ASCII only)
- `PEEK`/`POKE` are no-ops
- String garbage collection not implemented (uses `malloc`/`free` instead)
- Maximum 256 variables, 64 arrays, 16 FOR nesting, 24 GOSUB nesting
