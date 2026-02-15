# gwbasic-c

A portable C reimplementation of Microsoft GW-BASIC, using the
[original 8088 assembly source](https://github.com/microsoft/GW-BASIC)
(released by Microsoft in 2020) as the authoritative reference.

This is not a transpilation — it reimplements the algorithms in clean C11
with modern data structures while targeting bug-compatible behavior.
Unlike the original assembly (43,771 lines across 43 `.ASM` files), this
version is structured as modular C suitable for new feature development.

## Highlights

- **~8,600 lines of C11** with 50 test programs
- **Sixel graphics** — `SCREEN 1`/`SCREEN 2` rendering in compatible terminals
- **Sound** — `SOUND`, `BEEP`, `PLAY` (MML) via PulseAudio
- **Full file I/O** — sequential, random-access, SAVE/LOAD/MERGE/CHAIN
- **Terminal I/O** — raw mode for `INKEY$` and `INPUT$`
- **MIT License**

```{toctree}
:maxdepth: 2

getting-started
language-reference
architecture
development
roadmap
```
