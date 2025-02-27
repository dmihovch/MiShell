CC=gcc

sh:
	$(CC) -o sh src/exec.c src/builtins.c  src/main.c src/tokenizer.c src/utils.c

run: sh
	./sh
clean: 
	rm -rf *.o sh