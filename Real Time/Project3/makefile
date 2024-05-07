CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lGLU -lglut -lm -lpthread

all: ant

ant: ant.c
	$(CC) $(CFLAGS) -o ant ant.c $(LIBS)

clean:
	rm -f ant
