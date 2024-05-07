#include "local.h"
void create_queue_to_spy();
void extractPrefix(char*, int*);
void writeToFile(const char*);
void connect_parent_queue();
void save_paragraph();
void decoded_all_messages();
void clearMessages();
char* decodeMessage(const char*, int);
char* generateOriginalParagraph(char**, int);

int msgqid_spy;
int msgqid_parent;
int msgNum=0;
int numMsgRcv=0;
char *messagesRcv[MAX_LINE_LENGTH / 2]; 
int childRunning = 0;
int finish=0;


void signal_stop(){
	  clearMessages();
	  numMsgRcv=0;
	  childRunning = 0;
	  printf("Stop MasterSpy\n"); 
}


void signal_start(){
	clearMessages();
	printf("Start MasterSpy\n");
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

       create_queue_to_spy();
       connect_parent_queue();

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
           if (childRunning == 1 && finish == 0){
		 struct message rcvMsg;
	    
		if (msgrcv(msgqid_spy, &rcvMsg, BUFSIZ, 0, 0) == -1) {
		    printf("masterSpy --- Failed to get message from queue\n");
		}


		char message[BUFSIZ];   int MsgNumber;
		strcpy(message, rcvMsg.mtext);

	        extractPrefix(message, &MsgNumber);

	       if(messagesRcv[MsgNumber-1] == NULL){

		    messagesRcv[MsgNumber-1] = strdup(message);
		    numMsgRcv++;

	        }

		printf("\tNumber of Messages Received(MasterSpy): %d\n", numMsgRcv);


      		struct message flagParent;
      		flagParent.mtype = 1;
                snprintf(flagParent.mtext, BUFSIZ, "MasterSpy : msg-%d received\n\n",MsgNumber);

            	if (msgsnd(msgqid_parent, &flagParent, sizeof(flagParent.mtext), 0) == -1) {
                	perror("msgsnd");
                	exit(1);
            	}

	      if(numMsgRcv == msgNum){

			    decoded_all_messages();
			    save_paragraph();

			   struct message finishStr;
			   strncpy(finishStr.mtext, "Finish -0", MAX_WORD_LENGTH);
			   finishStr.mtype=1;
			   if (msgsnd(msgqid_parent, &finishStr, sizeof(finishStr.mtext), 0) == -1) {
					perror("msgsnd");
					exit(1);
			    }
		finish=1;
		  		
	      }
            sleep(5);

          }else{
               pause();  
          } 


      }


 return 0;


}



void connect_parent_queue(){

	 key_t key = ftok(SHARED_FILE, 'h');

	 msgqid_parent = msgget(key, IPC_CREAT | 0666);
	 if (msgqid_parent == -1) {
	      perror("msgget");
	      exit(1);
	 }
}


void create_queue_to_spy(){

    key_t key;
    key = ftok(SHARED_FILE, 'y');

    msgqid_spy = msgget(key, IPC_CREAT | 0666);
    if (msgqid_spy == -1) {
        perror("msgget");
        exit(1);
    }

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


void writeToFile(const char* data) {
    FILE* file = fopen("spy.txt", "w");
    
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    fprintf(file, "%s", data);
    fclose(file);
}



char* generateOriginalParagraph(char** columns, int maxColumnSize) {

char *paragraph = malloc(BUFSIZ*maxColumnSize+1);

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





