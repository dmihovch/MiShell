CC=gcc

sh:
	$(CC) -o sh src/shell.c src/exec.c src/builtins.c  src/lexer.c src/parser.c src/utils.c

run: sh
	./sh
clean: 
	rm -rf *.o sh