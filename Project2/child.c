#include "local.h"
void semaphore_lock(int);
void semaphore_unlock(int);
char* encodeMessage(const char*, int );
void extractNumber(char*, int*);

    key_t key;
    int shmid, semid;
    Memory *shmaddr;

int main(int argc, char* argv[]) {

	if (argc != 3) {
        fprintf(stderr, "Usage: %s <MsgNum>\n", argv[0]);
        exit(1);
    }


    int msgNum = atoi(argv[1]);
    int columnsNum = atoi(argv[2]);
    if (msgNum == -1 || columnsNum ==-1) {
        perror("Error: Failed to get arguments");
        exit(1);
    }
    

   key_t key = ftok(SHARED_FILE, 'q');
   int queueId = msgget(key, IPC_CREAT | 0666);
   if (queueId == -1) {
        perror("msgget");
        exit(1);
    }

            struct message msg;
            if (msgrcv(queueId, &msg, BUFSIZ, 1, 0) == -1) {
                perror("msgrcv");
                exit(1);
            }


    printf("\n Child %d received message: %s\n\n", getpid(), msg.mtext);   

    char* encodedMessage = encodeMessage(msg.mtext, msgNum);


    char* encodedMessagePrefix = malloc(strlen(encodedMessage) + 10 + 1);
    sprintf(encodedMessagePrefix, "%d %s", msgNum, encodedMessage);
     
    printf("------ > EncodedMessagePrefix: %s\n\n",encodedMessagePrefix); 

    key = ftok(SHARED_FILE, 'S');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    shmid = shmget(key, BUFSIZ* columnsNum, 0);
    if (shmid == -1) {
        perror("shmget---child");
        exit(1);
    }

    shmaddr = (Memory*)shmat(shmid, NULL, 0);
    if (shmaddr == (Memory*)-1) {
        perror("shmat");
        exit(1);
    }

    if ( (semid = semget(key, 1, 0)) == -1 ) {
    perror("semget -- child -- access");
    exit(3);
  }
  
       semaphore_lock(semid);
      strcpy(shmaddr[msgNum - 1].content, encodedMessagePrefix);
      semaphore_unlock(semid);

    free(encodedMessage); 
    free(encodedMessagePrefix);

    return 0;
}



char* encodeMessage(const char* msg, int msgNum) {
    int k = 0;
    int j = 0;
    int shift = 0;
    int number = 0;
    char* encodedMsg = malloc(MAX_WORD_LENGTH * sizeof(char));
    while (msg[k] != '\0') {
        if (isdigit(msg[k])) {
            char tempStr[2] = { msg[k], '\0' };
            number = atoi(tempStr);
            number = 1000000 - number; 

            char replacement[8];
            snprintf(replacement, sizeof(replacement), "%d", number);

            int replaceIndex = j;
            memcpy(&encodedMsg[replaceIndex], replacement, strlen(replacement));
            j += strlen(replacement);
        } else if (msg[k] == '!') {
            encodedMsg[j] = '1';
            j++;
        } else if (msg[k] == '?') {
            encodedMsg[j] = '2';
            j++;
        } else if (msg[k] == ',') {
            encodedMsg[j] = '3';
            j++;
        } else if (msg[k] == ';') {
            encodedMsg[j] = '4';
            j++;
        } else if (msg[k] == ':') {
            encodedMsg[j] = '5';
            j++;
        } else if (msg[k] == '%') {
            encodedMsg[j] = '6';
            j++;
        } else if (msg[k] == '.') {
            encodedMsg[j] = '7';
            j++;
        } else if (msg[k] == '$') {
            encodedMsg[j] = '8';
            j++;
        } else if (msg[k] == ' ') {
            encodedMsg[j] = ' ';
            j++;
        } else if (msg[k] >= 'a' && msg[k] <= 'z') {
            shift = shift + msgNum;
            encodedMsg[j] = ((msg[k] - 'a' + shift) % 26) + 'a';
            j++;
        } else if (msg[k] >= 'A' && msg[k] <= 'Z') {
            shift = shift + msgNum;
            encodedMsg[j] = ((msg[k] - 'A' + shift) % 26) + 'A';
            j++;
        }

        k++;
    }

    encodedMsg[j] = '\0'; 
    return encodedMsg;
}


void semaphore_lock(int semaphore_id) {
	    struct sembuf semaphore_op;
	    semaphore_op.sem_num = 0;
	    semaphore_op.sem_op = -1;
	    semaphore_op.sem_flg = 0;

	    if (semop(semaphore_id, &semaphore_op, 1) == -1) {
		perror("Failed to lock semaphore");
		exit(1);
	    }
}

void semaphore_unlock(int semaphore_id) {
	    struct sembuf semaphore_op;
	    semaphore_op.sem_num = 0;
	    semaphore_op.sem_op = 1;
	    semaphore_op.sem_flg = 0;

	    if (semop(semaphore_id, &semaphore_op, 1) == -1) {
		perror("Failed to unlock semaphore");
		exit(1);
	    }
}
