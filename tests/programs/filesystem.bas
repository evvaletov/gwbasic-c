10 REM FILES, MKDIR, CHDIR, RMDIR test
20 MKDIR "gwb_test_dir"
30 OPEN "gwb_test_dir/test.txt" FOR OUTPUT AS #1
40 PRINT #1, "hello"
50 CLOSE #1
60 CHDIR "gwb_test_dir"
70 SHELL "pwd > /dev/null"
80 CHDIR ".."
90 KILL "gwb_test_dir/test.txt"
100 RMDIR "gwb_test_dir"
110 PRINT "All filesystem tests passed"
