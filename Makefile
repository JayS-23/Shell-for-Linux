shell: main.c
	gcc main.c -o shell
run: shell
	./shell
clean:
	rm -f shell