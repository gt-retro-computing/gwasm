SRCS = main.c lexer.c parser.c helpers.c
CC = gcc

parser: $(SRCS) helpers.h
	$(CC) -std=c99 -o parser $(SRCS)

parser.c: parser.y lexer.l helpers.h
	bison parser.y --debug

lexer.c: lexer.l helpers.h
	flex lexer.l

clean:
	rm -rf *.o lexer.c lexer.h parser.c parser.h parser
