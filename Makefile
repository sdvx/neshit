CC = gcc
CFLAGS = -c -O3 -fomit-frame-pointer

all: neshit

neshit: main.o cpu.o rom.o
	$(CC) main.o cpu.o rom.o -o neshit

main.o: main.c
	$(CC) $(CFLAGS) main.c

cpu.o: cpu.c
	$(CC) $(CFLAGS) cpu.c

rom.o: rom.c
	$(CC) $(CFLAGS) rom.c

clean:
	rm -rf *.o

