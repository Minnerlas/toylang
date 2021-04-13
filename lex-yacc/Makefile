# DEBUG=-DYYDEBUG
DEBUG=

a.out:	y.tab.c lex.yy.c ast.c util.c
	gcc -g $^ -I.. $(DEBUG)

run: a.out
	./$<

y.tab.c: parser.y
	./oyacc -d $<

y.tab.h: y.tab.c

lex.yy.c: lexer.l y.tab.h
	lex $<

clean:
	rm -rvf y.tab.h y.tab.c lex.yy.c a.out
