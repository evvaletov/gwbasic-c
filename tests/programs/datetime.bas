10 REM DATE$, TIME$, TIMER test
20 D$ = DATE$
30 T$ = TIME$
40 R = TIMER
50 IF LEN(D$) <> 10 THEN PRINT "FAIL date len": END
60 IF MID$(D$,3,1) <> "-" THEN PRINT "FAIL date sep1": END
70 IF MID$(D$,6,1) <> "-" THEN PRINT "FAIL date sep2": END
80 IF LEN(T$) <> 8 THEN PRINT "FAIL time len": END
90 IF MID$(T$,3,1) <> ":" THEN PRINT "FAIL time sep1": END
100 IF MID$(T$,6,1) <> ":" THEN PRINT "FAIL time sep2": END
110 IF R < 0 OR R > 86400 THEN PRINT "FAIL timer range": END
120 PRINT "DATE$ = "; D$
130 PRINT "TIME$ = "; T$
140 PRINT "TIMER ="; INT(R)
150 PRINT "All date/time tests passed"
