10 REM Multiplication table - Rosetta Code
20 REM Header row
30 PRINT "  X";
40 FOR J = 1 TO 12
50   PRINT USING "####"; J;
60 NEXT J
70 PRINT
80 PRINT STRING$(51, "-");
90 PRINT
100 REM Table body
110 CHECKSUM = 0
120 FOR I = 1 TO 12
130   PRINT USING "###"; I;
140   FOR J = 1 TO 12
150     IF J < I THEN PRINT "    "; ELSE PRINT USING "####"; I * J;
160     IF J >= I THEN CHECKSUM = CHECKSUM + I * J
170   NEXT J
180   PRINT
190 NEXT I
200 REM Sum of i*j for j>=i, i=1..12: known value 3367
210 IF CHECKSUM <> 3367 THEN PRINT "FAIL: checksum"; CHECKSUM : END
220 PRINT "Multiplication table OK"
