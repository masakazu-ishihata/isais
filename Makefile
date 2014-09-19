# Makefile
TAR = test
CC = gcc -O6 -Wall
L = -lm
OBJS = main.o isais.o
SHAD = -fPIC -shared
LIB = libisais.so

all : $(TAR) lib
$(TAR): $(OBJS)
	  $(CC) -o $@ $(OBJS) $(L)
clean:
	  rm -f $(OBJS) $(LIB) $(TAR) *~ *.bak

.c.o:	  $(CC) -c $<om

lib:
	$(CC) $(SHAD) -o $(LIB) isais.c $(L)

main.o: main.h isais.o
isais.o: isais.h
