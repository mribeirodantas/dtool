SRC=dtool.c
OBJ=$(SRC:.c=.o)
OUT=dtool
CC=gcc ${CFLAGS}
RM=rm -f

default: all

.c.o:
	$(CC) -c $<

dtool: $(OBJ)
	$(CC) $(OBJ) -o $(OUT)

all: $(OUT)
clean: 
	$(RM) $(OBJ) $(OUT)
