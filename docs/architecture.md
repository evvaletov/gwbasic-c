# Architecture

## Pipeline

```
Source text → Tokenizer (CRUNCH) → Token stream
                                      ↓
                              Expression evaluator (FRMEVL)
                                      ↓
                              Statement dispatcher (NEWSTT)
                                      ↓
                              HAL (platform I/O)
```

The interpreter follows the original GW-BASIC's internal structure. Source lines
are tokenized by CRUNCH into a compact token stream. The NEWSTT loop dispatches
each statement, calling FRMEVL for expression evaluation. All platform I/O goes
through a HAL vtable (`hal_ops_t`), keeping the core interpreter portable.

## Module Map

| Module | Source | Original Assembly |
|--------|--------|--------------------|
| Tokenizer (CRUNCH/LIST) | `tokenizer.c` | GWMAIN.ASM |
| Expression evaluator | `eval.c` | GWEVAL.ASM |
| Execution loop + control flow | `interp.c` | BINTRP.ASM |
| Graphics engine | `graphics.c` | — |
| Token/keyword tables | `tokens.c`, `tokens.h` | IBMRES.ASM |
| Error handling | `error.c` | GWDATA.ASM |
| Integer arithmetic | `math_int.c` | MATH1.ASM |
| Float ops + MBF conversion | `math_float.c` | MATH2.ASM |
| Transcendentals | `math_transcend.c` | MATH1.ASM |
| String functions | `strings.c` | BISTRS.ASM |
| PRINT statement | `print.c` | BINTRP.ASM |
| PRINT USING | `print_using.c` | BIPRTU.ASM |
| Variables + arrays | `vars.c`, `arrays.c` | GWMAIN.ASM |
| File I/O + random access | `fileio.c` | BIPTRG.ASM |
| Program I/O (SAVE/LOAD) | `program_io.c` | BIMISC.ASM |
| INPUT/LINE INPUT | `input.c` | BINTRP.ASM |
| Sound engine | `sound.c` | — |
| Platform abstraction | `hal_posix.c` | OEM*.ASM |

## Source Layout

```
src/         — core interpreter (19 files)
include/     — headers (11 files)
platform/    — HAL backends (1 file)
tests/       — test programs (50 .BAS files)
```

~8,600 lines of C11.

## Design Decisions

### Relation to Original Assembly

The original GW-BASIC source was
[released by Microsoft in 2020](https://github.com/microsoft/GW-BASIC) as 8088
assembly (43,771 lines across 43 `.ASM` files). This reimplementation uses that
assembly as a reference but is not a transpilation — it reimplements the
algorithms in idiomatic C with modern data structures.

### Key Differences from the Original

- **IEEE 754 floating point** — MBF (Microsoft Binary Format) conversion is only
  used for file I/O compatibility (CVI/CVS/CVD, MKI$/MKS$/MKD$)
- **Dynamic memory allocation** — `malloc`/`free` instead of a 64KB segment layout
- **malloc'd strings** — instead of a compacting garbage collector
- **`setjmp`/`longjmp`** — for error recovery, matching the original's stack reset
  behavior
