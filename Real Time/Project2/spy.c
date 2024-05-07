#include "local.h"
void semaphore_lock(int);
void semaphore_unlock(int);
void setUp_Shmem_Sem_queue();

    key_t key;
    int shmid, semid;
    Memory *shmaddr;
    int msgqid;
    int msgNum;
    int childRunning = 0;

void signal_start(){
	childRunning=1;
	printf("Start Spy\n");

}


void signal_stop(){
	printf("Stop Spy\n");
	childRunning=0;
}


int main(int argc, char* argv[]) {

	if (argc != 2) {
        fprintf(stderr, "Usage: %s <msgNum>\n", argv[0]);
        exit(1);
    }

  
     msgNum = atoi(argv[1]);
    if (msgNum ==-1) {
        perror("Error: Failed to get arguments");
        exit(1);
    }

   setUp_Shmem_Sem_queue();

    struct sigaction stop_action, start_action;
    stop_action.sa_handler = signal_stop;
    start_action.sa_handler = signal_start;

	    if (sigaction(SIGUSR1, &stop_action, NULL) == -1) {
        perror("sigaction for SIGINT failed");
        exit(1);
    }

    if (sigaction( SIGINT, &start_action, NULL) == -1) {
        perror("sigaction for SIGUSR1 failed");
        exit(1);
    }

     while(1){
 
               if(childRunning ==1){

   		struct timeval tv;
   		gettimeofday(&tv, NULL);
  	        srand(tv.tv_sec * 1000000 + tv.tv_usec ^ getpid()<<16);
   
  		int  location = rand() % msgNum ;

        	semaphore_lock(semid);

      		struct message msg;
      		msg.mtype = 1;
      		strcpy(msg.mtext, shmaddr[location].content);
		int result;
    		do {
       		 result = msgsnd(msgqid, &msg, MAX_WORD_LENGTH, 0);
    		} while (result == -1 && errno == EINTR);

	   	 if (result == -1) {
		perror("msgsnd");
		exit(1);
	   	 }
            	

  		semaphore_unlock(semid);

	        sleep(5);
            }else{
                   pause();
            }


       }

 return 0;

 

   
}


void setUp_Shmem_Sem_queue(){

    
    key = ftok(SHARED_FILE, 'S');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    shmid = shmget(key, BUFSIZ* msgNum, 0);
    if (shmid == -1) {
        perror("shmget---spy");
        exit(1);
    }

    shmaddr = (Memory*)shmat(shmid, NULL, 0);
    if (shmaddr == (Memory*)-1) {
        perror("shmat");
        exit(1);
    }

    if ( (semid = semget(key, 1, 0)) == -1 ) {
    perror("semget -- helper -- access");
    exit(3);
    }

    key = ftok(SHARED_FILE, 'y');

    msgqid = msgget(key, IPC_CREAT | 0666);
    if (msgqid == -1) {
        perror("msgget");
        exit(1);
    }

}

void semaphore_lock(int semaphore_id) {
    struct sembuf semaphore_op;
    semaphore_op.sem_num = 0;
    semaphore_op.sem_op = -1;
    semaphore_op.sem_flg = 0;

       while (semop(semaphore_id, &semaphore_op, 1) == -1) {
        if (errno != EINTR) {
            perror("semop");
            exit(1);
        }
    }
}

void semaphore_unlock(int semaphore_id) {
    struct sembuf semaphore_op;
    semaphore_op.sem_num = 0;
    semaphore_op.sem_op = 1;
    semaphore_op.sem_flg = 0;

      while (semop(semaphore_id, &semaphore_op, 1) == -1) {
        if (errno != EINTR) {
            perror("semop");
            exit(1);
        }
    }
}
