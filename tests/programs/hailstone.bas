10 REM Hailstone (Collatz) sequence - Rosetta Code
20 N# = 27
30 STEPS = 0
40 MAXVAL# = N#
50 WHILE N# <> 1
60   IF N# / 2 = INT(N# / 2) THEN N# = N# / 2 ELSE N# = 3 * N# + 1
70   STEPS = STEPS + 1
80   IF N# > MAXVAL# THEN MAXVAL# = N#
90 WEND
100 REM Hailstone(27) has 111 steps, max value 9232
110 IF STEPS <> 111 THEN PRINT "FAIL: expected 111 steps, got"; STEPS : END
120 IF MAXVAL# <> 9232 THEN PRINT "FAIL: expected max 9232, got"; MAXVAL# : END
130 PRINT "Hailstone OK: 27 ->"; STEPS; "steps, max"; MAXVAL#
