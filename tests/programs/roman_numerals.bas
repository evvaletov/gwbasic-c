10 REM Roman numeral encoder - Rosetta Code
20 DIM V(12), R$(12)
30 DATA 1000,"M",900,"CM",500,"D",400,"CD"
40 DATA 100,"C",90,"XC",50,"L",40,"XL"
50 DATA 10,"X",9,"IX",5,"V",4,"IV",1,"I"
60 FOR I = 0 TO 12
70 READ V(I), R$(I)
80 NEXT I
90 DATA 1990, 2008, 1666, 3999, 42, 14
100 FOR T = 1 TO 6
110 READ N
120 ORIG = N
130 S$ = ""
140 I = 0
150 WHILE N > 0
160   WHILE N >= V(I)
170     S$ = S$ + R$(I)
180     N = N - V(I)
190   WEND
200   I = I + 1
210 WEND
220 PRINT ORIG; "="; S$
230 NEXT T
240 PRINT "Roman numerals OK"
