#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE 1024

int min, max;
FILE *file;
float num;
int fd;
char buffer_receive[BUFFER_SIZE];
char buffer_send[BUFFER_SIZE];

float randomFloat(int, int);
void signal_parent(pid_t) ;
void signal_catcher(int);
void signal_cathcher_coProcessor(int);


int main(int argc, char *argv[ ]) {

      if (argc < 3) {
        printf("Usage: %s <seed> <mode>\n", argv[0]);
        printf("  <mode> can be either \"child\" or \"co-processor\"\n");
        exit(1);
      }

    int seed = atoi(argv[1]);
    srand(seed);
    
     if (strcmp(argv[2], "player") == 0) {

        if (sigset(SIGINT, signal_catcher) == SIG_ERR) {
            perror("Sigset can not set SIGINT");
            exit(SIGINT);
        }

        while (1);

    } else if (strcmp(argv[2], "co-processor") == 0) {

       if (sigset(SIGUSR1, signal_cathcher_coProcessor) == SIG_ERR) {
            perror("Sigset can not set SIGUSR1");
            exit(SIGUSR1);
        }

       while(1);
    }

}

float randomFloat(int min, int max) {
    float range = (float)(max - min) + 1.0f;
    float random = (float)rand() / (float)RAND_MAX;
    return (float)(random * range) + (float)min;
}

void signal_parent(pid_t parent_pid) {

   
    char filename[20];

    // Read range from file
    file = fopen("range.txt", "r");
    fscanf(file, "%d,%d", &min, &max);
    fclose(file);
 
    num = randomFloat(min, max);

    // Write random number to file
    parent_pid = getppid();
    sprintf(filename, "%d.txt",getpid());
    file = fopen(filename, "w");
    fprintf(file, "%.1f", num);
    fclose(file);

    kill(parent_pid, SIGUSR1);
}

void signal_catcher(int p){
    // Signal parent process
    signal_parent(getppid());
}


void signal_cathcher_coProcessor(int p){

    // Open the FIFO for reading
       if ((fd = open(FIFO_NAME, O_RDWR)) == -1){
           perror("open");
           exit(EXIT_FAILURE);
          }

      // Read the message from the FIFOS
     if( read(fd, buffer_receive, sizeof(buffer_receive)) <0){
      perror("read");
      exit(1);
     }

      
      float num1, num2, num3, num4;
      sscanf(buffer_receive, "%f,%f,%f,%f", &num1, &num2, &num3, &num4);

       // Calculate sum1 and sum2
        float sum1 = num1 + num2;
        float sum2 = num3 + num4;
         
       sprintf(buffer_send, "%.1f,%.1f", sum1, sum2);

       if( write(fd, buffer_send, sizeof(buffer_send)) <0){
        perror("write");
        exit(1);
      }

     close(fd);
      
}


