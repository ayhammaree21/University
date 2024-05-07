#include "local.h"
void semaphore_lock(int);
void semaphore_unlock(int);
void setUp_Shmem_Sem_queue();
char* generateOriginalParagraph(char**, int);
void writeToFile(const char*);
void extractPrefix(char*, int*);
char* decodeMessage(const char*, int);
void save_paragraph();
void clearMessages();
void decoded_all_messages();

    key_t key;
    int shmid, semid;
    Memory *shmaddr;
    int msgqid;
    int msgNum=0;
    int numMsgRcv=0;
    int childRunning = 0;
    int finish=0;

char *messagesRcv[MAX_LINE_LENGTH / 2]; 

void signal_stop() {
	 clearMessages();  
	numMsgRcv=0;
	childRunning = 0;
	printf("Stop Receiver\n");
}

void signal_start(){
	clearMessages();
	printf("Start Receiver\n");
	 childRunning = 1;
	 finish=0;

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
		perror("sigaction for SIGUSR1 failed");
		exit(1);
    }


    if (sigaction(SIGINT, &start_action, NULL) == -1) {
		perror("sigaction for SIGINT failed");
		exit(1);
    }

      

    while(1){  

  if( childRunning == 1 && finish==0){
        char message[BUFSIZ];   int MsgNumber;

   		struct timeval tv;
   		gettimeofday(&tv, NULL);
  	        srand(tv.tv_sec * 1000000 + tv.tv_usec ^ getpid()<<16);
   
  		int  location = rand() % msgNum ;

        	semaphore_lock(semid);

                strcpy(message, shmaddr[location].content);
                extractPrefix(message, &MsgNumber);

                if(messagesRcv[MsgNumber-1] == NULL){

                        messagesRcv[MsgNumber-1] = strdup(message);

                     numMsgRcv++;

                }
			printf("\tNumber of Messages Received(Receiver): %d\n", numMsgRcv);

      		struct message msg;
      		msg.mtype = 1;
                snprintf(msg.mtext, BUFSIZ, "Receiver : msg-%d received\n\n",MsgNumber);

            	if (msgsnd(msgqid, &msg, sizeof(msg.mtext), 0) == -1) {
                	perror("Receiver:msgsnd");
                	exit(1);
            	}


                if(numMsgRcv == msgNum){
                       decoded_all_messages();
                       save_paragraph();
                        
                   
                       struct message finishStr;
                       strncpy(finishStr.mtext, "Finish -1", MAX_WORD_LENGTH);
                       finishStr.mtype=1;
                      if (msgsnd(msgqid, &finishStr, sizeof(finishStr.mtext), 0) == -1) {
                	perror("msgsnd");
                	exit(1);
			
                       }
		finish=1;        
                } 

		semaphore_unlock(semid);
               
		
	   sleep(3);


       } else{

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
        perror("shmget---receiver");
        exit(1);
    }

    shmaddr = (Memory*)shmat(shmid, NULL, 0);
    if (shmaddr == (Memory*)-1) {
        perror("shmat");
        exit(1);
    }

    if ( (semid = semget(key, 1, 0)) == -1 ) {
    perror("semget -- receiver -- access");
    exit(3);
    }


    key = ftok(SHARED_FILE, 'h');

    msgqid = msgget(key, IPC_CREAT | 0666);
    if (msgqid == -1) {
        perror("msgget");
        exit(1);
    }

}

void writeToFile(const char* data) {
	    FILE* file = fopen("receiver.txt", "w"); 
	    
	    if (file == NULL) {
		printf("Error opening file.\n");
		return;
	    }
	    
	    fprintf(file, "%s", data); 
	    fclose(file); 
}


char* generateOriginalParagraph(char** columns, int maxColumnSize) {

char *paragraph = malloc(BUFSIZ*msgNum +1);

paragraph[0] = '\0';


		int maxWordCount = 0;
		for (int i = 0; i < maxColumnSize; i++) {
		    char *column = strdup(columns[i]);
		    int wordCount = 0;
		    char *word = strtok(column, " ");
		    while (word != NULL) {
			wordCount++;
			word = strtok(NULL, " ");
		    }
		    if (wordCount > maxWordCount) {
			maxWordCount = wordCount;
		    }
	          free(column);
		}

           
		for (int wordIndex = 0; wordIndex < maxWordCount; wordIndex++) {
		    for (int i = 0; i < maxColumnSize; i++) {
                           

			char * column = strdup(columns[i]);

			 char *word = strtok(column, " ");
                          
				if (word != NULL) {
                                 
					int c=wordIndex;
					while(c--){
					   word = strtok(NULL, " ");
					}
                                         
					if(word != NULL && strcmp(word,"alright") != 0){
						 strcat(paragraph, word);
			    			  strcat(paragraph, " ");
					}
						
				}
		      free(column);
		    }
		    strcat(paragraph, "\n");
		}
		
		return paragraph;

}



void extractPrefix(char* msg, int* number) {
   
	    sscanf(msg, "%d", number);

	    
	    int numDigits = snprintf(NULL, 0, "%d", *number);

	    
	    memmove(msg, msg + numDigits, strlen(msg) - numDigits + 1);
	    
	    while (isspace((unsigned char)*msg)) {
		memmove(msg, msg + 1, strlen(msg));
	    }
}

void decoded_all_messages(){

	   for(int i =0; i< numMsgRcv; i++){

	   char* decodedMessage = decodeMessage(messagesRcv[i], i+1);
	   strcpy(messagesRcv[i], decodedMessage); 
	    
	    free(decodedMessage);
	   }


   
}


void clearMessages() {
	    int i;
	    for (i = 0; i < msgNum; i++) {
		messagesRcv[i] = NULL;
	    }
	    numMsgRcv = 0;
}



char* decodeMessage(const char* msg, int msgNum) {
    int k = 0;
    int j = 0;
    int shift = 0;
    int number = 0;
    char* decodedMsg = malloc(MAX_WORD_LENGTH * sizeof(char));
    while (msg[k] != '\0') {
       if (msg[k] == '9') {

            char tempStr[7];
            int c = 0;
            while(c < 6){
              tempStr[c] = msg[k];
              c++; k++;
            }
            tempStr[6] = '\0';
            number = atoi(tempStr);
            number = 1000000 - number; 

            char replacement[2];
            snprintf(replacement, sizeof(replacement), "%d", number);
          
            memcpy(&decodedMsg[j], replacement, strlen(replacement));
            k--;

        } else if (msg[k] == '1' && msg[k+1] == '0') {
            decodedMsg[j] = '0';
            k+=6;
        } else if (msg[k] == '1') {
            decodedMsg[j] = '!';
            
        } else if (msg[k] == '2') {
            decodedMsg[j] = '?';
        } else if (msg[k] == '3') {
            decodedMsg[j] = ',';
        } else if (msg[k] == '4') {
            decodedMsg[j] = ';';
        } else if (msg[k] == '5') {
            decodedMsg[j] = ':';
        } else if (msg[k] == '6') {
            decodedMsg[j] = '%';
        } else if (msg[k] == '7') {
            decodedMsg[j] = '.';
        } else if (msg[k] == '8') {
            decodedMsg[j] = '$';
        } else if (msg[k] == ' ') {
            decodedMsg[j] = ' ';
        } else if (msg[k] >= 'a' && msg[k] <= 'z') {
            shift = shift + msgNum;
            decodedMsg[j] = ((msg[k] - 'a' - shift + 26*shift) % 26) + 'a';
        } else if (msg[k] >= 'A' && msg[k] <= 'Z') {
            shift = shift + msgNum;
            decodedMsg[j] = ((msg[k] - 'A' - shift + 26*shift) % 26) + 'A';
        }
        
        k++; j++;
    }

    decodedMsg[j] = '\0'; 
    return decodedMsg;
}

void save_paragraph(){
     
          char* originalParagraph = generateOriginalParagraph(messagesRcv, msgNum);
	  writeToFile(originalParagraph) ;

         free(originalParagraph);

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

