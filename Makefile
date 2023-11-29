CC = gcc
DIR = /usr/local/bin

default: enote.c
	$(CC) $^ -o enote

install: enote.c
	$(CC) $^ -o enote
	cp enote $(DIR)/enote
	chmod 555 $(DIR)/enote

uninstall: $(DIR)/enote
	rm $^

debug: enote.c
	@$(CC) -g $^ -o enote
	@gdb enote
	@rm enote

run: enote.c
	@$(CC) $^ -o enote
	@./enote
	@rm enote
