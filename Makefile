PROGRAM = compile
OBJS   	= Parser.o Token.o CodeGen.o

CC     	= gcc 
CFLAGS 	= -g -o

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(PROGRAM) $(OBJS)


Parser.o: Parser.c
	$(CC) -c Parser.c

Parser.o: Parser.h Token.h CodeGen.h


Token.o: Token.c
	$(CC) -c Token.c

Token.o: Token.h


CodeGen.o: CodeGen.c
	$(CC) -c CodeGen.c

CodeGen.o: CodeGen.h Token.h


.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS)
