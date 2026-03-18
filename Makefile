all: basiccodegen.l basiccodegen.y main.c
	yacc -d basiccodegen.y
	lex basiccodegen.l
	gcc main.c
run: all
	./a.out decl.txt
clean:
	rm -f a.out lex.yy.c y.tab.c y.tab.h
