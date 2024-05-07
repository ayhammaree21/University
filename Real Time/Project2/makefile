CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lGLU -lglut

all: parent child sender spy masterSpy helper receiver

parent: parent.c
	$(CC) $(CFLAGS) -o parent parent.c $(LIBS)

child: child.c
	$(CC) $(CFLAGS) -o child child.c

sender: sender.c
	$(CC) $(CFLAGS) -o sender sender.c $(LIBS)

spy: spy.c
	$(CC) $(CFLAGS) -o spy spy.c $(LIBS)

masterSpy: masterSpy.c
	$(CC) $(CFLAGS) -o masterSpy masterSpy.c $(LIBS)

helper: helper.c
	$(CC) $(CFLAGS) -o helper helper.c $(LIBS)

receiver: receiver.c
	$(CC) $(CFLAGS) -o receiver receiver.c $(LIBS)

clean:
	rm -f parent child sender spy masterSpy helper receiver
