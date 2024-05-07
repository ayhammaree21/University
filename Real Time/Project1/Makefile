CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lGLU -lglut

all: parent child

parent: parent.c
	$(CC) $(CFLAGS) -o parent parent.c $(LIBS)

child: child.c
	$(CC) $(CFLAGS) -o child child.c

clean:
	rm -f parent child

