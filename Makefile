TARGET = han
CC = cc
CFLAGS = -g

$(TARGET): han.c han.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm $(TARGET)
