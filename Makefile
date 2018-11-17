PROGRAM   = galaga
CC        = gcc
CFLAGS    = -g -Wall -Wextra
LDFLAGS   = -lGL -lGLU -lglut
OBJ = main.o		\
	  init.o		\
	  callbacks.o	\
	  spacecraft.o

VPATH = src

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROGRAM): $(OBJ) 
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

.PHONY: clean dist

clean:
	-rm -f *.o $(PROGRAM) *.swp *.swo src/*.swp src/*.swo

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
