#
#       Super Star Trek Classic
#
#	This program is free software; you can redistribute it
#	and/or  modify in any way you want.
#

#CC	= mcc
#CC	= c89
CC	= gcc
#CC	= acc
#CC	= cc
#CC     = purify acc

#CFLAGS	= -g 
#CFLAGS	= -O 
CFLAGS	= -O -Wall -g
#CFLAGS	= -O -g
OBJS	= startrek.o
LIBS	= -lm
PROGS	= startrek
FILES	= startrek.doc
DEST	= /usr/local/games

all:	$(PROGS) 

startrek:	$(OBJS)
	$(CC) $(LDFLAGS) -o startrek $(OBJS) $(LIBS)

clean:
	rm -f $(PROGS) *.o

install:
	sudo cp $(PROGS) $(DEST)
	sudo cp $(FILES) $(DEST)
	sudo strip $(DEST)/$(PROGS)

