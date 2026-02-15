10 REM 100 Doors problem - Rosetta Code
20 DIM D(100)
30 FOR PASS = 1 TO 100
40   FOR DOOR = PASS TO 100 STEP PASS
50     D(DOOR) = 1 - D(DOOR)
60   NEXT DOOR
70 NEXT PASS
80 REM Only perfect squares should be open
90 C = 0
100 FOR I = 1 TO 100
110   IF D(I) = 0 THEN 150
120   S = SQR(I)
130   IF INT(S) <> S THEN PRINT "FAIL: door"; I; "open but not perfect square" : END
140   C = C + 1
150 NEXT I
160 IF C <> 10 THEN PRINT "FAIL: expected 10 open doors, got"; C : END
170 PRINT "100 Doors OK:"; C; "open"
