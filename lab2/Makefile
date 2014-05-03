SOURCES=minishell.c util.c command.c

all:
	gcc -std=c99 -D_POSIX_C_SOURCE=1 -Wall -o minishell $(SOURCES) -lreadline

clean:
	@-rm -f minishell
