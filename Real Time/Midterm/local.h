#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <pthread.h>


typedef struct {
    int id;
    char name[50];
} Animal;


typedef struct {
    int id;
    int gender;
} Chick;


typedef struct {
    int id;
    int incubation_days;
} Egg;






















