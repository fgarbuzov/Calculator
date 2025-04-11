CC = gcc
CFLAGS = -Wall -O2

SRC_DIR = src

SRC = $(wildcard $(SRC_DIR)/*.c)

EXEC = calc.out

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(SRC) -o $@ -lm

clean:
	rm -f $(EXEC)
