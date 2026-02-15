10 REM Pascal's triangle - Rosetta Code
20 NROWS = 8
30 DIM R(20)
40 FOR ROW = 0 TO NROWS - 1
50   REM Build row right-to-left to avoid overwriting
60   R(ROW) = 1
65   IF ROW < 2 THEN 100
70   FOR J = ROW - 1 TO 1 STEP -1
80     R(J) = R(J) + R(J-1)
90   NEXT J
100  R(0) = 1
110  REM Print with spacing
120  PRINT SPACE$(2 * (NROWS - ROW));
130  FOR J = 0 TO ROW
140    PRINT USING "####"; R(J);
150  NEXT J
160  PRINT
170 NEXT ROW
180 REM Verify a known value: row 7, col 3 = C(7,3) = 35
190 IF R(3) <> 35 THEN PRINT "FAIL: C(7,3) should be 35, got"; R(3) : END
200 PRINT "Pascal's triangle OK"
