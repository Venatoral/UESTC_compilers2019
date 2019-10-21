rdparser: rdparser.o lex.yy.o
	gcc rdparser.c lex.yy.o -o test -lfl

rdcheck: rdcheck.o lex.yy.o
	gcc rdcheck.c lex.yy.o -o test -lfl

rdparser.o: rdparser.c
	gcc rdparser.c -g -c

rdcheck.o: rdcheck.c
	gcc rdcheck.c -g -c

lex.yy.o: rdlex.l
	flex rdlex.l
	gcc lex.yy.c -g -c
