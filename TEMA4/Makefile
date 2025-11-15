CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
OBJS = main.o usuarios.o productos.o
TARGET = tienda

.PHONY: all clean asm objdump

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c usuarios.h productos.h
	$(CC) $(CFLAGS) -c main.c -o main.o

usuarios.o: usuarios.c usuarios.h
	$(CC) $(CFLAGS) -c usuarios.c -o usuarios.o

productos.o: productos.c productos.h
	$(CC) $(CFLAGS) -c productos.c -o productos.o

asm: 
	$(CC) $(CFLAGS) -S main.c -o main.s
	$(CC) $(CFLAGS) -S usuarios.c -o usuarios.s
	$(CC) $(CFLAGS) -S productos.c -o productos.s

obj:
	$(CC) $(CFLAGS) -c main.c -o main.o
	$(CC) $(CFLAGS) -c usuarios.c -o usuarios.o
	$(CC) $(CFLAGS) -c productos.c -o productos.o

objdump:
ifndef FILE
	$(error FILE variable not set. Ej: make objdump FILE=main.o OUT=main_from_objdump.s)
endif
ifndef OUT
	$(error OUT variable not set. Ej: make objdump FILE=main.o OUT=main_from_objdump.s)
endif
	objdump -d $(FILE) > $(OUT)

clean:
	rm -f $(OBJS) $(TARGET) *.s *.o *_from_objdump.s