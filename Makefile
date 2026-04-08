
CC = cc

FLAGS = -g

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: libpfonts.a

.c.o:
	$(CC) $(FLAGS) -c $<

libpfonts.a: $(OBJ)
	ar rcs libpfonts.a $(OBJ)

clean:
	rm -f libpfonts.a $(OBJ) 

install: all
	cp -f libpfonts.a /usr/local/lib


.PHONY: all clean install
