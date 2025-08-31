all: shell

shell:
	gcc	shell.c	-o	shell
	./shell

clean:
	rm -f shell *.out