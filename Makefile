all: src/basiccodegen.l src/basiccodegen.y src/main.c
	yacc -d src/basiccodegen.y
	lex src/basiccodegen.l
	gcc src/main.c -o compiler -I.
run: all
	./compiler tests/test1.txt
clean:
	rm -f compiler lex.yy.c y.tab.c y.tab.h
