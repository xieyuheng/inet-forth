1 1 eq ok
1 2 eq not ok
1 -1 eq not ok
-1 -1 eq ok

1 1 iadd 2 eq ok
2 2 iadd 4 eq ok
1 -1 iadd 0 eq ok

1 1 imul 1 eq ok
2 2 imul 4 eq ok
2 -1 imul -2 eq ok

1 1 idiv 1 eq ok
5 2 idiv 2 eq ok
5 2 imod 1 eq ok

1 int-to-float 1.0 eq ok
