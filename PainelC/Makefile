# Makefile para gerar e instalar o software de controle do painel

CC = gcc
CFLAGS = -O3 -Wall -pthread
LL = -lpigpio -lrt

all:	painelc

painelc:	painelc.o display.o leds.o util.o
	$(CC) -o painelc painelc.o leds.o display.o util.o $(LL)

painelc.o:	painelc.c painelc.h retroserver.h tape1.h tape2.h tape3.h
	$(CC) $(CFLAGS) -c -o painelc.o painelc.c

display.o:	display.c painelc.h fonte8x8.h
	$(CC) $(CFLAGS) -c -o display.o display.c

leds.o:	leds.c painelc.h
	$(CC) $(CFLAGS) -c -o leds.o leds.c

util.o:	util.c painelc.h
	$(CC) $(CFLAGS) -c -o util.o util.c

clean:
	rm -f *.o painelc

install:
	sudo cp painelc.service /etc/systemd/system/
	sudo systemctl enable painelc

remove:
	sudo systemctl disable painelc
