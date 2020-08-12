
OBJS=y.tab.o lex.yy.o ast.o vector.o function.o
CC=gcc -std=gnu99 -g

calc: parser.y lexer.l main.o ast.o vector.o function.o astprint.o parser
	$(CC) -c y.tab.c lex.yy.c
	$(CC) $(OBJS) main.o -o calc
	$(CC) vector.o test.c -o test 
	$(CC) $(OBJS) astprint.o -o astprint


clean:
	rm -f calc
	rm -f *.o
	rm -f lex.yy.c y.tab.c
	rm -f y.tab.h

parser:
	bison --debug -y -d parser.y
	flex lexer.l

%.o: %.c %.h parser
	$(CC) -c $< -o $@