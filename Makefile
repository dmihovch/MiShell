CC=gcc

sh:
	@$(CC) -o sh src/shloop.c src/exec.c src/builtins.c  src/main.c src/tokenizer.c src/utils.c

debug-mac:
	@$(CC) -g -fsanitize=address -o sh src/shloop.c src/exec.c src/builtins.c  src/main.c src/tokenizer.c src/utils.c

run: sh
	@exec ./sh
clean: 
	@rm -rf *.o sh

runv: sh
	@exec valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./sh

runv-test: sh test
	@exec valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./sh test
