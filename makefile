PROG = AES
CC = gcc
CFLAGS = 

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

$(PROG) : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm $(PROG) 
	rm $(OBJ)