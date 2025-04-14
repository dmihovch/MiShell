CC=gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Werror -g
MACFLAGS = -fsanitize=address

mishell:
	@$(CC) $(CFLAGS) -o mishell src/shloop.c src/exec.c src/builtins.c  src/main.c src/tokenizer.c src/utils.c

debug-mac:
	@$(CC) $(CFLAGS) $(MACFLAGS) -o mishell src/shloop.c src/exec.c src/builtins.c  src/main.c src/tokenizer.c src/utils.c

run: mishell
	@./mishell

valgrind: mishell
	@valgrind --leak-check=full --show-leak-kinds=all ./mishell
clean:
	@rm -rf *.o mishell
