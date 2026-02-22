#!/usr/bin/env python3
"""Transform a .bas file so PRINT output goes to a file instead of screen.

Real GWBASIC.EXE uses BIOS INT 10h for PRINT, so DOS > redirection won't
capture it. This script rewrites PRINT to PRINT #9, with file #9 opened
for output, so the results land in a file we can diff against.

Usage: transform_for_capture.py input.bas output.bas outfile.txt
"""

import re
import sys


def transform(lines, outfile):
    result = []
    # Add file open as the very first line (line 0)
    result.append(f'0 OPEN "{outfile}" FOR OUTPUT AS #9\n')

    for line in lines:
        line = line.rstrip('\r\n')
        if not line.strip():
            continue

        # Parse line number
        m = re.match(r'^(\s*\d+)(.*)', line)
        if not m:
            result.append(line + '\n')
            continue

        linenum = m.group(1)
        rest = m.group(2)

        # Replace PRINT with PRINT #9, (but not PRINT# which is file I/O)
        # Handle multi-statement lines with :
        rest = transform_statements(rest)

        result.append(linenum + rest + '\n')

    # Add cleanup: close file and exit
    result.append('63999 CLOSE #9:SYSTEM\n')
    return result


def transform_statements(text):
    """Transform PRINT statements in a line to PRINT #9,"""
    parts = split_statements(text)
    out = []
    for part in parts:
        stripped = part.lstrip()
        upper = stripped.upper()
        if upper.startswith('PRINT') and not upper.startswith('PRINT#') and not upper.startswith('PRINT #'):
            # Check it's not PRINT USING with a file
            after = stripped[5:]
            # Replace PRINT with PRINT #9,
            if after and after[0] not in (' ', '\t', ';', ',', '"'):
                # PRINT immediately followed by letter - might be variable like PRINTER
                out.append(part)
            else:
                indent = part[:len(part) - len(stripped)]
                out.append(indent + 'PRINT #9,' + after)
        elif upper.startswith('END'):
            # Replace END with CLOSE #9:SYSTEM
            out.append(' CLOSE #9:SYSTEM')
        elif upper.startswith('STOP'):
            out.append(' CLOSE #9:SYSTEM')
        elif upper.startswith('SYSTEM'):
            out.append(' CLOSE #9:SYSTEM')
        else:
            out.append(part)
    return ':'.join(out)


def split_statements(text):
    """Split a BASIC line on : but not inside strings."""
    parts = []
    current = []
    in_string = False
    for ch in text:
        if ch == '"':
            in_string = not in_string
            current.append(ch)
        elif ch == ':' and not in_string:
            parts.append(''.join(current))
            current = []
        else:
            current.append(ch)
    parts.append(''.join(current))
    return parts


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print(f'Usage: {sys.argv[0]} input.bas output.bas outfile.txt', file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[1]) as f:
        lines = f.readlines()

    result = transform(lines, sys.argv[3])

    with open(sys.argv[2], 'w') as f:
        f.writelines(result)
