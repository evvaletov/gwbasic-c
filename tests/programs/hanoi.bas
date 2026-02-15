10 REM Towers of Hanoi - iterative with explicit stack - Rosetta Code
20 NDISKS = 4
30 DIM SN(50), SF(50), ST(50), SV(50)
40 SP = 0
50 REM Push initial call: move NDISKS from A to C via B
60 SP = SP + 1 : SN(SP) = NDISKS : SF(SP) = 1 : ST(SP) = 3 : SV(SP) = 2
70 MOVES = 0
80 IF SP = 0 THEN 200
90 N = SN(SP) : F = SF(SP) : T = ST(SP) : V = SV(SP) : SP = SP - 1
100 IF N = 0 THEN 80
110 REM Push move(N-1, via, to, from) - this goes on stack FIRST (executed LAST)
120 SP = SP + 1 : SN(SP) = N-1 : SF(SP) = V : ST(SP) = T : SV(SP) = F
130 REM Push the actual move of disk N from F to T (sentinel N=0 above it)
140 SP = SP + 1 : SN(SP) = 0 : SF(SP) = F : ST(SP) = T : SV(SP) = 0
150 MOVES = MOVES + 1
160 REM Push move(N-1, from, via, to)
170 SP = SP + 1 : SN(SP) = N-1 : SF(SP) = F : ST(SP) = V : SV(SP) = T
180 GOTO 80
200 REM Should be 2^NDISKS - 1 = 15 moves
210 EXPECTED = 2^NDISKS - 1
220 IF MOVES <> EXPECTED THEN PRINT "FAIL: expected"; EXPECTED; "got"; MOVES : END
230 PRINT "Hanoi OK:"; MOVES; "moves"
