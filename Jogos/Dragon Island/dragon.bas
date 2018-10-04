#!/usr/local/bin/yabasic
20 clear screen
21 print "Voce esta sozinho e os dragoes comem as 8!"
22 print
23 h = 5
24 m = 0
25 d = 1 + int(ran(11))
26 rem print d
27 gosub 57
28 print "Em qual caverna voce atira a sua lanca";
29 input t
30 if (d = t)  goto 51
31 if ((d-t)*(t-d) >= -1) print "S*N*O*R*T!"
32 print "Voce errou! ";
33 print "O dragao ";
34 s = int(ran(4))- 1
35 if (s = 0) goto 41
36 d = d + s
37 if (d < 1) d = 10 + d
38 if (d > 10) d = 10 + d
39 print "se move!"
40 goto 42
41 print "aguarda."
42 print "Voce se arrisca a pegar a lanca (S ou N)";
43 input a$
44 if (a$="S" or a$="s") goto 48
45 if (a$<>"N" and a$<>"n") goto 42
46 gosub 57
47 goto 33
48 if (d = t) goto 55
49 print "Voce conseguiu!"
50 goto 27
51 print "A*A*R*R*R*G*G*H*H! V*O*C*E M*E* P*E*G*O*U!"
52 print ""
53 print "PARABENS!!!"
54 end
55 print "T*E* P*E*G*U*E*I*!!! N*H*A*C! N*H*A*C!"
56 end
57 print ""
58 print "Sao ",h;
59 if (m=0) goto 62
60 print ":",m
61 goto 64
62 print " horas"
63 if (h=8) goto 55
64 m = m + 15
65 if m <> 60 goto 68
66 h = h + 1
67 m = 0
68 return
