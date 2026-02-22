#!/bin/bash
# Compatibility test runner: compare gwbasic output against real GWBASIC.EXE
#
# Usage:
#   tests/run_compat.sh --generate   Generate .expected files from GWBASIC.EXE via DOSBox-X
#   tests/run_compat.sh              Compare gwbasic output against .expected files
set -u

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
GWBASIC="${PROJECT_DIR}/build/gwbasic"
EXPECTED_DIR="${SCRIPT_DIR}/expected"
TRANSFORM="${SCRIPT_DIR}/transform_for_capture.py"

DOSBOX_FLATPAK="com.dosbox_x.DOSBox-X"
DOSBOX_CONF="${SCRIPT_DIR}/dosbox-compat.conf"
GWBASIC_EXE="/home/evaletov/DOS/dosbox-x/C_DRIVE/GWBASIC/GWBASIC.EXE"
C_DRIVE="/home/evaletov/DOS/dosbox-x/C_DRIVE"

# Tests to skip (file I/O with host paths, interactive, sound, graphics, CHAIN)
SKIP_LIST=(
    chain_test.bas
    chain_target.bas
    file_io.bas
    random_access.bas
    save_load.bas
    run_file.bas
    text_adventure.bas
    number_guess.bas
    graphics_stubs.bas
    sound_test.bas
    play_music.bas
    play_scale.bas
    write_input.bas
    mkicvi.bas
    invoice.bas
)

should_skip() {
    local name="$1"
    for skip in "${SKIP_LIST[@]}"; do
        [ "$name" = "$skip" ] && return 0
    done
    return 1
}

normalize_output() {
    # Normalize line endings and trailing whitespace for comparison
    sed 's/\r//g; s/[[:space:]]*$//' "$1" | sed '/^$/d'
}

generate_expected() {
    echo "=== Generating expected output from GWBASIC.EXE via DOSBox-X ==="
    mkdir -p "$EXPECTED_DIR"

    local tmpdir
    tmpdir=$(mktemp -d)
    trap 'rm -rf "$tmpdir"' EXIT

    local count=0
    local errors=0

    for bas in "$SCRIPT_DIR"/programs/*.bas; do
        local name
        name="$(basename "$bas")"
        should_skip "$name" && continue

        local stem="${name%.bas}"
        local dos_bas="$tmpdir/${stem}.BAS"
        local dos_out="OUTPUT.TXT"

        # Transform the program for file-based output capture
        python3 "$TRANSFORM" "$bas" "$dos_bas" "$dos_out"

        # Copy to C drive
        cp "$dos_bas" "$C_DRIVE/${stem}.BAS"

        # Create autoexec batch to run the program
        local bat="$C_DRIVE/RUNTEST.BAT"
        printf '@ECHO OFF\r\nC:\\GWBASIC\\GWBASIC.EXE /F:9 C:\\%s.BAS\r\nEXIT\r\n' "$stem" > "$bat"

        # Remove any previous output
        rm -f "$C_DRIVE/$dos_out"

        # Run DOSBox-X headless
        SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
            flatpak run "$DOSBOX_FLATPAK" \
            -conf "$DOSBOX_CONF" \
            -c "MOUNT C $C_DRIVE" \
            -c "C:" \
            -c "CALL RUNTEST.BAT" \
            -c "EXIT" \
            >/dev/null 2>&1 &
        local pid=$!

        # Wait with timeout
        local timeout=15
        local elapsed=0
        while kill -0 "$pid" 2>/dev/null && [ "$elapsed" -lt "$timeout" ]; do
            sleep 1
            elapsed=$((elapsed + 1))
        done
        if kill -0 "$pid" 2>/dev/null; then
            kill "$pid" 2>/dev/null
            wait "$pid" 2>/dev/null
            printf "  TIMEOUT  %s\n" "$name"
            errors=$((errors + 1))
            continue
        fi
        wait "$pid" 2>/dev/null

        # Copy output
        if [ -f "$C_DRIVE/$dos_out" ]; then
            normalize_output "$C_DRIVE/$dos_out" > "$EXPECTED_DIR/${stem}.expected"
            printf "  OK       %s\n" "$name"
            count=$((count + 1))
        else
            printf "  NO-OUT   %s\n" "$name"
            errors=$((errors + 1))
        fi

        # Cleanup
        rm -f "$C_DRIVE/${stem}.BAS" "$C_DRIVE/$dos_out" "$bat"
    done

    echo ""
    echo "Generated $count expected files ($errors errors) in $EXPECTED_DIR"
}

compare_output() {
    if [ ! -x "$GWBASIC" ]; then
        echo "ERROR: gwbasic not found at $GWBASIC (run cmake/make first)"
        exit 1
    fi

    echo "=== Compatibility test: gwbasic vs .expected ==="

    local pass=0
    local fail=0
    local skip=0
    local missing=0

    for bas in "$SCRIPT_DIR"/programs/*.bas; do
        local name
        name="$(basename "$bas")"
        should_skip "$name" && { skip=$((skip + 1)); continue; }

        local stem="${name%.bas}"
        local expected="$EXPECTED_DIR/${stem}.expected"

        if [ ! -f "$expected" ]; then
            printf "  SKIP     %s (no .expected file)\n" "$name"
            missing=$((missing + 1))
            continue
        fi

        local actual
        actual=$(mktemp)
        if timeout 5 "$GWBASIC" "$bas" 2>/dev/null | sed 's/\r//g; s/[[:space:]]*$//' | sed '/^$/d' > "$actual"; then
            if diff -q "$expected" "$actual" >/dev/null 2>&1; then
                printf "  PASS     %s\n" "$name"
                pass=$((pass + 1))
            else
                printf "  FAIL     %s\n" "$name"
                diff -u "$expected" "$actual" | head -20
                fail=$((fail + 1))
            fi
        else
            printf "  ERROR    %s\n" "$name"
            fail=$((fail + 1))
        fi
        rm -f "$actual"
    done

    echo ""
    echo "$((pass + fail)) compared: $pass passed, $fail failed ($skip skipped, $missing no expected)"
    [ "$fail" -eq 0 ] || exit 1
}

case "${1:-}" in
    --generate)
        generate_expected
        ;;
    *)
        compare_output
        ;;
esac
