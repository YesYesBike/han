TARGET = han
CC = cc
CFLAGS = -g
OBJ = han.o

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

han.o: han.c han.h
	$(CC) $(CFLAGS) -c $<
