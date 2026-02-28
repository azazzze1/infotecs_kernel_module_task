CC = gcc
CFLAGS = -Wall -Wextra

all: logModuleController

logModuleController: main.c logModuleController.c
	$(CC) $(CFLAGS) -o logModuleController main.c logModuleController.c

clean:
	rm -f logModuleController

.PHONY: all clean