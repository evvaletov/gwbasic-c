10 REM File I/O test
20 OPEN "/tmp/gwbasic_fio_test.txt" FOR OUTPUT AS #1
30 PRINT #1, "Hello from GW-BASIC"
40 PRINT #1, "Second line"
50 CLOSE #1
60 OPEN "/tmp/gwbasic_fio_test.txt" FOR INPUT AS #1
70 LINE INPUT #1, A$
80 LINE INPUT #1, B$
90 CLOSE #1
100 PRINT A$
110 PRINT B$
