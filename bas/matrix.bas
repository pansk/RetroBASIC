4 PRINT"testing read"
5 READ A$:printA$
6 read B$:print B$
9 DATA "1","2"
10 DIM A(5,5)
20 A(3,3)=10
30 PRINT"tight-printing 5x5 matrix containing one non-zero entry in 3,3"
40 MAT PRINT A;
50 PRINT"wide-printing 5x5 containing one non-zero entry in 3,3"
60 MAT PRINT A
70 PRINT"printing a 1-D array containing one non-zero entry in 2"
80 DIM Z(3)
85 Z(2)=5
90 MAT PRINT Z
95 PRINT"printing a (0,3) array"
96 DIM Y(0,3)
97 Y(0,2)=2
98 MAT PRINT Y
100 DIM B(3,3)
110 REM PRINT"input a 3x3"
120 REM MAT INPUT B
130 REM PRINT"printing input"
140 REM MAT PRINT B
200 DIM C(2,5)
210 PRINT"READing and PRINTing 2x5"
220 MAT READ C
240 MAT PRINT C
300 PRINT"IDNing 3x3"
310 MAT B=IDN
320 MAT PRINT B
400 print"ZERing 3x3"
410 MAT B=ZER
420 MAT PRINT B
500 PRINT"CONing 3x3"
510 MAT B=CON
520 MAT PRINT B
600 PRINT"CONing a (1x)6"
610 DIM E(6)
620 MAT E=CON
630 MAT PRINT E
700 PRINT "DIMD(6,6), IDN(3,3)"
710 DIM D(6,6)
720 MAT D=IDN(3,3)
730 MAT PRINT D
800 PRINT "Copy that to F(2,2)"
810 DIM F(10,10)
820 MAT F=D(2,2)
830 MAT PRINT F
900 PRINT "B is currently"
905 MAT PRINT B
910 PRINT "add B to B and put into D"
915 MAT D=B+B
920 MAT PRINT D
925 PRINT "sub B back out of D again"
930 MAT D=D-B
935 MAT PRINT D
1000 PRINT"multiply B by 5 into D"
1010 MAT D=(5)*B
1020 MAT PRINT D
2000 REM
2010 PRINT"multiply B by 2"
2020 MAT B=(2)*B
2030 MAT PRINT B
2040 PRINT"multiply D by B"
2050 MAT D=D*B
2060 MAT PRINT D
3000 DIM M(3,4),N(4,2),R(3,2)
3010 PRINT"reading M"
3020 MAT READ M
3030 MAT PRINT M
3040 PRINT"reading N"
3050 MAT READ N
3060 MAT PRINT N
3070 PRINT"multiply M*N into R"
3080 MAT R=M*N
3090 MAT PRINT R
4000 PRINT"read S"
4005 DIM S(3,2),J(5,5)
4010 MAT READ S
4015 MAT PRINT S
4017 PRINT"transposing S"
4020 MAT J=TRN(S)
4030 MAT PRINT J
4100 PRINT"read S$"
4110 DIM S$(2,2),T$(2,2)
4120 MAT READ S$
4130 MAT PRINT S$
4140 PRINT"transposing S$"
4150 MAT T$=TRN(S$)
4160 MAT PRINT T$
5000 PRINT "fill R with 7.5"
5010 MAT R=(7.5)
5020 MAT PRINT R
5030 PRINT "fill S$ with 'hello'"
5040 MAT S$=("hello")
5050 MAT PRINT S$
5100 PRINT"zer S$"
5110 MAT S$=ZER
5120 MAT PRINT S$
5500 PRINT "read IN"
5510 DIM IN(3,3), OU(3,3)
5520 MAT READ IN
5530 MAT PRINT IN
5540 PRINT "invert IN to OU"
5550 MAT OU=INV(IN)
5560 MAT PRINT OU
10000 DATA 1.5,2.5,3,4,5,6,7,8,9,10
10010 DATA 5,2,0,10
10030 DATA 3,5,2,5
10040 DATA 20,0,0,0
10050 DATA 1.50, 0.20
10060 DATA 2.80, 0.40
10070 DATA 5.00,1.00
10080 DATA 2.00,0.50
10090 DATA 4.2,-3.8,15.6,8.7,0,-4
10100 DATA "one",two2:DATA three,"four"
10110 DATA 15,10,5,12,24,8,6,0,36
19999 END
