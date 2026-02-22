# Development

## Development History

| Phase | Commit | Description |
|-------|--------|-------------|
| 1 | `d8e8375` | Expression calculator with direct mode |
| 2 | `6162595` | Variables, arrays, program storage, control flow |
| 3 | `c2d73e9` | File I/O, PRINT USING, SAVE/LOAD, MID$ assignment, graphics stubs |
| 4 | `df5c308` | CHAIN, RUN "file", random-access I/O (FIELD/PUT/GET), CVI/CVS/CVD/MKI$/MKS$/MKD$ |
| 5 | `1f4c460` | CI, terminal I/O (raw mode), Sixel graphics, 13 classic programs, AND/OR precedence fix |
| 5+ | `169f16d` | SOUND/BEEP/PLAY with PulseAudio backend |
| 5+ | `691031a` | Fix RESTORE with line number, 8 Rosetta Code test programs |
| 6 | | Full-screen TUI editor, KEY statement, Ctrl+Break, DOSBox-X compat testing, project rename |

## Tests

50 test programs in `tests/programs/`. Run the full suite:

```bash
bash tests/run_tests.sh
```

Each test has a 5-second timeout. When `.expected` files are present
(generated from real GWBASIC.EXE), the runner also reports compatibility
match status.

### Compatibility Testing

Compare output against real GWBASIC.EXE running under DOSBox-X:

```bash
# Generate .expected files from GWBASIC.EXE (requires DOSBox-X Flatpak)
bash tests/run_compat.sh --generate

# Compare gwbasic output against .expected
bash tests/run_compat.sh
```

## CI

GitHub Actions runs on every push to `main` and on pull requests. The workflow
builds the project with PulseAudio support and runs all 50 test programs.

See [`.github/workflows/ci.yml`](https://github.com/evvaletov/gw-basic-2026/blob/main/.github/workflows/ci.yml).
