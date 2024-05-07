#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>


#define MAX_LINE_LENGTH 1000
#define MAX_WORD_LENGTH 1042
#define SHARED_FILE "shared_file.txt"
#define MAX_MESSAGES 30
#define MAX_USER_DEFINED 20
#define FIFO_NAME "/tmp/my_fifo"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 610

// Structure for the message
struct message {
    long mtype;
    char mtext[MAX_WORD_LENGTH];
};


typedef struct {
    char content[BUFSIZ];
} Memory;


union semun {
   int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
};


