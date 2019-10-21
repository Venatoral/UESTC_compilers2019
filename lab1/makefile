man_lex: man_lex.c
	gcc man_lex.c -g -o test

auto_lex: auto_lex.l
	flex auto_lex.l
	gcc lex.yy.c -g -o test -lfl
