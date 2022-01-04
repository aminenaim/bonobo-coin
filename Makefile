CC=gcc
CFLAGS=-Wall -g
LDFLAGS=
EXEC=bc
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

mrproper:
	@rm -rf *.o

main.o: annexe.h