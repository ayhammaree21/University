#include "local.h"
char* generateOriginalParagraph(char** , int );
int main() {


    FILE *file;
    char line[MAX_LINE_LENGTH];
    char *columns[MAX_LINE_LENGTH / 2];  
    int fd;

    file = fopen("sender.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int lineCount = 0;
    int maxColumnSize = 0;


    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';  

        char *word = strtok(line, " ");
        int wordCount = 0;
        while (word != NULL) {
            if (wordCount >= maxColumnSize) {
                maxColumnSize++;
                columns[wordCount] = malloc(strlen(word) + 2);
                strcpy(columns[wordCount], word);
            } else {
                size_t oldSize = strlen(columns[wordCount]);
                size_t newSize = oldSize + strlen(word) + 2;
                columns[wordCount] = realloc(columns[wordCount], newSize);
                strcat(columns[wordCount], " ");
                strcat(columns[wordCount], word);
            }

            word = strtok(NULL, " ");
            wordCount++;
        }

        while (wordCount < maxColumnSize) {
            size_t oldSize = strlen(columns[wordCount]);
            size_t newSize = oldSize + strlen(" alright") + 2;
            columns[wordCount] = realloc(columns[wordCount], newSize);
            strcat(columns[wordCount], " alright");
            wordCount++;
        }

        lineCount++;
    }

    fclose(file);
 
   int numWords[MAX_LINE_LENGTH / 2];  

   for (int i = 0; i < maxColumnSize; i++) {
	    numWords[i] = 0;  
	    char* column = columns[i];


	    for (int j = 0; column[j] != '\0'; j++) {
		if (column[j] == ' ') {
		    numWords[i]++;
		}
    }

    numWords[i]++;  
   }

   
   int maxWords = numWords[0]; 


    for (int i = 1; i < maxColumnSize; i++) {
	       if (numWords[i] > maxWords) {
		   maxWords = numWords[i];
	       }
    }


    for (int i = 0; i < maxColumnSize; i++) {
    int numWordsToAdd = maxWords - numWords[i];
    if(numWordsToAdd == 0){
       continue;
    }

    char modifiedColumn[MAX_LINE_LENGTH];
    strcpy(modifiedColumn, "alright");

    for (int j = 1; j < numWordsToAdd; j++) {
        strcat(modifiedColumn, " alright");
    }

    strcat(modifiedColumn, " ");
    strcat(modifiedColumn, columns[i]);

    columns[i] = malloc(strlen(modifiedColumn) + 1);
    strcpy(columns[i], modifiedColumn);
   }

    for (int i = 0; i < maxColumnSize; i++) {
        printf("\nParent Process - Column %d: %s\n", i + 1, columns[i]);
    }

       if ((fd = open(FIFO_NAME, O_RDWR)) == -1){
		   perror("open");
		   exit(EXIT_FAILURE);
          }

		char buffer_send[6];
		sprintf(buffer_send, "%d", maxColumnSize);
		if( write(fd, buffer_send, sizeof(buffer_send)) <0){
		perror("write");
		exit(1);
      }

     close(fd);



   /////////////////////QUEUE/////////////////////////
   
    key_t key;
    int msgqid;
    pid_t pid;
    int i;

    key = ftok(SHARED_FILE, 'q');

    msgqid = msgget(key, IPC_CREAT | 0666);
    if (msgqid == -1) {
        perror("msgget");
        exit(1);
    }

    for (i = 0; i < maxColumnSize; i++) {
       		    pid = fork();
        if (pid == -1) {
		    perror("fork");
		    exit(1);
        } else if (pid == 0) {
		      char columnsNum[20];
		      char numberofMsg[20];
		      sprintf(columnsNum, "%d",maxColumnSize);
		      sprintf(numberofMsg, "%d",i+1);
		      execl("child", "child",numberofMsg,columnsNum ,NULL);
		      perror("Error: Failed to execute child process");
		    
		      exit(0);
        } else {
		    struct message msg;
		    msg.mtype = 1;
		    strncpy(msg.mtext, columns[i], MAX_WORD_LENGTH); 
		    if (msgsnd(msgqid, &msg, sizeof(msg), 0) == -1) {
		        perror("Sender--msgsnd");
		        exit(1);
		    }
        }
		sleep(1);
    }

    for (int i = 0; i < maxColumnSize; i++) {
        free(columns[i]);
    }

	if (msgctl(msgqid, IPC_RMID, NULL) == -1) {
        perror("Error: Failed to remove message queue");
        exit(1);
    }
  


    return 0;
}



