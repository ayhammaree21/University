#include "local.h"
#include <GL/glut.h>
#include <string.h>

void forkSender();
void create_sharedMemory();
void create_semaphore();
void read_configuration();
void get_columnsNUM();
void fork_receiver();
void create_queue();
void forkHelpers(pid_t *, int );
void fork_masterSpy();
void fork_Spy(pid_t *, int);
void clean (pid_t *, int, pid_t *, int);
void extractLocations(const char*, int*, int*);
void extractMsgNumberRcv(const char*, int*);
void send_start_signalsMasterandRec();
void send_start_signalsHelperandSpy();
void send_stop_signalsHelperandSpy();
void send_stop_signalsMasterandRec();
void fillArray(int[], int);
void empty_all_received();
int compareFiles(const char*, const char*);
void removeTrailingSpaces(char*);
void parentRunning();
void replaceMultipleSpaces(char*);
/////////Opengl////////
void reshape(int , int );
void display();
void drawMemory();
char* checkContinuity(int* , int );
void drawRectangleWithText2(float , float , float, float ,int* );
void startOpenGl();



    int shmem_locations[MAX_MESSAGES];
    int receiver_MsgRcved[MAX_MESSAGES];
    int masterSpy_MsgRcved[MAX_MESSAGES];
    key_t key;
    int shmid, semid;
    Memory *shmaddr;
    union semun arg;
    pid_t senderPid;
    pid_t receiverPid;
    pid_t masterSpyPid;
    pid_t helperPids[MAX_USER_DEFINED];
    pid_t spyPids[MAX_USER_DEFINED];
    int columnsNum=0;
    int fd;
    int msgqid;
    int number_helper_process;
    int number_spy_process;
    int threshold;
    int rounds =0;
    int receiver_win=0;
    int masterSpy_win=0;
    const char* spyfile = "spy.txt";
    const char* receiverfile = "receiver.txt";
    const char* senderfile = "sender.txt";
    int sum1win = 0;
    int swapFlag=0;
    int location1, location2;
    ////////////Opengl//////////////////
    int memoryCells =6;
    char msg_receiver_gl[40];
    char msg_masterSpy_gl[40];
    char winnerText[40];
    char finalResText[40];
    int terminate = 0;




int main(int argc, char *argv[ ]) {

         read_configuration();
         get_columnsNUM();
         fillArray(shmem_locations, columnsNum);
         create_sharedMemory();
         create_semaphore();
   	 

        int status;
        waitpid(senderPid, &status, 0);

        create_queue();
        forkHelpers(helperPids,number_helper_process);
        fork_receiver();
        fork_masterSpy();
        fork_Spy(spyPids,number_spy_process);

 	sprintf(msg_receiver_gl, "...");
	sprintf(msg_masterSpy_gl, "...");
	sprintf(winnerText, " ");
        sprintf(finalResText, " ");

send_start_signalsMasterandRec();
send_start_signalsHelperandSpy();



	
	glutInit(&argc, argv);
	startOpenGl();
       
    return 0;
}

void create_sharedMemory(){


    key = ftok(SHARED_FILE, 'S');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }


    shmid = shmget(key, BUFSIZ * columnsNum, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget---parent");
        exit(1);
    }


    shmaddr = (Memory*)shmat(shmid, NULL, 0);
    if (shmaddr == (Memory*)-1) {
        perror("shmat");
        exit(1);
    }

}
void send_stop_signalsMasterandRec(){
kill(masterSpyPid, SIGUSR1); 
sleep(2);
kill(receiverPid, SIGUSR1);
}
void send_stop_signalsHelperandSpy(){
    for(int i=0; i < number_helper_process; i++){
        kill(helperPids[i], SIGUSR1);
	sleep(1);
   } 
    sleep(1);
    for(int i=0; i < number_spy_process; i++){
        kill(spyPids[i], SIGUSR1);
	sleep(1);

   } 


}


void send_start_signalsMasterandRec(){
kill(masterSpyPid, SIGINT);
sleep(2);
kill(receiverPid, SIGINT);
}


void send_start_signalsHelperandSpy(){


for(int i=0; i < number_spy_process; i++){
		kill(spyPids[i], SIGINT);
		sleep(1);
	   }
sleep(1); 
 for(int i=0; i < number_helper_process; i++){
		kill(helperPids[i], SIGINT);
		sleep(1);
	   }

    
}


void create_semaphore(){


    key = ftok(SHARED_FILE, 'S');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }


     semid = semget(key, 1, IPC_CREAT | 0666);
       if (semid == -1) {
        perror("parent ---- Failed to create semaphore");
        exit(1);
    }

    arg.val =1;
    
     if ( semctl(semid, 0, SETVAL, arg) == -1 ) {
    	 perror("semctl -- parent -- initialization");
     	 exit(3);
     }

     
}

void get_columnsNUM(){

    if (access(FIFO_NAME, F_OK) == 0) {
    if (unlink(FIFO_NAME) == -1) {
        perror("Error removing existing FIFO");
        exit(-1);
     }
  }
    
      mkfifo(FIFO_NAME, 0666);

      if ((fd = open(FIFO_NAME, O_RDWR)) == -1) {
          perror("open");
          exit(EXIT_FAILURE);
       }

      forkSender();
     char buffer_receive[6];
     if(read(fd,buffer_receive,sizeof(buffer_receive)) <0){
        perror("read");
       exit(1);
       }
        
       sscanf(buffer_receive, "%d", &columnsNum );
       printf("\n\tColumnsNum: %d\n\n",columnsNum);

       close(fd);
}

void forkSender(){

     senderPid = fork();

    if (senderPid == -1) {
        fprintf(stderr, "Fork failed.\n");

    } else if (senderPid == 0) {

        execl("sender", "sender", NULL);
        fprintf(stderr, "Exec failed.\n");

    } 

}

void fork_receiver(){

    receiverPid = fork();

    if (receiverPid == -1) {
        fprintf(stderr, "Fork failed.\n");

    } else if (receiverPid == 0) {

         char columnsNumStr[20];
        sprintf(columnsNumStr, "%d",columnsNum);
        execl("receiver", "receiver",columnsNumStr, NULL);
        fprintf(stderr, "Exec failed.\n");

    } 

}

void fork_masterSpy(){

	    masterSpyPid = fork();

	    if (masterSpyPid == -1) {
		fprintf(stderr, "Fork failed.\n");

	    } else if (masterSpyPid == 0) {

		 char columnsNumStr[20];
		sprintf(columnsNumStr, "%d",columnsNum);
		execl("masterSpy", "masterSpy",columnsNumStr, NULL);
		fprintf(stderr, "Exec failed.\n");

	    } 

}

void forkHelpers(pid_t *helperPids, int size){

	     for (int i = 0; i < size; i++){

	    helperPids[i] = fork();

	    if (helperPids[i] == -1) {
		fprintf(stderr, "Fork failed.\n");

	    } else if (helperPids[i] == 0) {

		   char columnsNumStr[20];
		sprintf(columnsNumStr, "%d",columnsNum);
		execl("helper", "helper",columnsNumStr, NULL);
		fprintf(stderr, "Exec failed.\n");

	    } 
	     sleep(2);
	    }


}

void fork_Spy(pid_t *spyPids, int size){

	     for (int i = 0; i < size; i++){
	    spyPids[i] = fork();
	    if (spyPids[i] == -1) {
		fprintf(stderr, "Fork failed.\n");

	    } else if (spyPids[i] == 0) {

		   char columnsNumStr[20];
		sprintf(columnsNumStr, "%d",columnsNum);
		execl("spy", "spy",columnsNumStr, NULL);
		fprintf(stderr, "Exec failed.\n");

	    } 
	     sleep(2);
	    }

}

void create_queue(){

	    key_t key;

	    key = ftok(SHARED_FILE, 'h');

	    msgqid = msgget(key, IPC_CREAT | 0666);
	    if (msgqid == -1) {
		perror("msgget");
		exit(1);
	    }

}

void empty_all_received(){

       for(int e=0; e< columnsNum;e++){
             masterSpy_MsgRcved[e] = 0;
             receiver_MsgRcved[e] =0;
       }

    struct message msg;
    size_t msgSize = sizeof(struct message) - sizeof(long);

    while (msgrcv(msgqid, &msg, msgSize, 0, IPC_NOWAIT) != -1) {
        printf("Discarded message: %s\n", msg.mtext);
    }
  

}


void read_configuration() {
		    char read[40];
		    FILE *fptr;
		    fptr = fopen("user_defind_var.txt","r"); 
		    if (fptr == NULL)
		    {
			perror("Error while opening the file.\n");
			exit(-1);
		    }

		    while (fgets(read, sizeof(read), fptr)) {
			char *token = strtok(read," ");
		   
			if(strcmp(token, "NUMBER_Helper_Process") == 0){ 
			    number_helper_process = atoi(strtok(NULL,"\n")); 
			}else if (strcmp(token, "NUMBER_Spy_Process") == 0){
			    number_spy_process = atoi(strtok(NULL,"\n"));
			}
			else if (strcmp(token, "THRESHOLD") == 0){
			    threshold = atoi(strtok(NULL,"\n"));
			}
		    }
		    if(fclose(fptr)){
			exit(-1);
		    }
}


void extractMsgNumberRcv(const char* message, int* msgNum) {
    const char* firstNumStart = strchr(message, '-') + 1;
    sscanf(firstNumStart, "%d", msgNum);

}


void extractLocations(const char* message, int* location1, int* location2) {
    const char* firstNumStart = strchr(message, '-') + 1;
    const char* secondNumStart = strchr(firstNumStart, '-') + 1;
    
    sscanf(firstNumStart, "%d", location1);
    sscanf(secondNumStart, "%d", location2);
}



void fillArray(int shmem_locations[], int columnsNum) {
	    for (int i = 0; i < columnsNum; i++) {
		shmem_locations[i] = i + 1;
	    }
}


void removeTrailingSpaces(char* str) {
	    int i = strlen(str) - 1;
	    while (i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')) {
		str[i] = '\0';
		i--;
	    }
}

int compareFiles(const char* file1, const char* file2) {
	    FILE *f1, *f2;
	    char line1[256];
	    char line2[256];
	    int lineNum = 1;
	    int diffCount = 0;

	    f1 = fopen(file1, "r");
	    f2 = fopen(file2, "r");

	    if (f1 == NULL || f2 == NULL) {
		printf("Failed to open files.\n");
		return -1;
	    }

	    while (fgets(line1, sizeof(line1), f1) && fgets(line2, sizeof(line2), f2)) {
		removeTrailingSpaces(line1);
		removeTrailingSpaces(line2);
		replaceMultipleSpaces(line1);
		replaceMultipleSpaces(line2);

		if (strcmp(line1, line2) != 0) {
		    printf("Difference Found at Line %d:\n", lineNum);
		    printf("%s\n", line1);
		    printf("%s\n", line2);
		    diffCount++;
		}

		lineNum++;
	    }

	    if (diffCount == 0) {
		printf("No Differences Found.\n");
	    }

	    fclose(f1);
	    fclose(f2);

            return diffCount;
}

void replaceMultipleSpaces(char* str) {
    int i, j;
    int length = strlen(str);

    for (i = 0, j = 0; i < length; i++) {
        if (str[i] != ' ' || i == 0) {
            str[j++] = str[i];
        }
        else if (str[i] == ' ' && str[i - 1] != ' ') {
            str[j++] = str[i];
        }
    }

    str[j] = '\0';
}



void drawRectangleWithText2(float x, float y, float width, float height,int* arr ) {


    glColor3f(0.0, 0.0, 1.0); 
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x, y + height);
    glVertex2f(x + width, y + height);
    glVertex2f(x + width, y);
    glEnd();


    glColor3f(1.0, 1.0, 1.0); 
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x, y + height);
    glVertex2f(x + width, y + height);
    glVertex2f(x + width, y);
    glEnd();

    glColor3f(1.0, 1.0, 1.0); 
    glRasterPos2f(x + width / 2.0 - 6.0, y + height -25.0);
    const char* text = checkContinuity(arr, columnsNum);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)text);
}


    char* checkContinuity(int* arr, int size) {
    char* result = (char*)malloc(size * sizeof(char) * 3); 
    strcpy(result, ""); 

    char str[30];

    strcat(result, "");
    for (int i = 0; i < size; i++) {
       if(arr[i] != 0){
        sprintf(str, "\n%d", arr[i]); 
        strcat(result, str); 
       }else{
          sprintf(str, "\n "); 
        strcat(result, str);
      }

    }

    return result;
}



void drawMemory() {
    float memoryWidth = 0.6 * WINDOW_WIDTH;
    float memoryHeight = 0.8 * WINDOW_HEIGHT;
    int numRows = memoryCells;  

    float cellWidth = memoryWidth / memoryCells;
    float cellHeight = memoryHeight / numRows;


    float memoryX = (WINDOW_WIDTH - memoryWidth) / 2.0;
    float memoryY = (WINDOW_HEIGHT - memoryHeight) / 2.0;

    glPushMatrix();
    glTranslatef(memoryX, memoryY, 0.0);


    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(4.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.0, 0.0);
    glVertex2f(0.0, memoryHeight);
    glVertex2f(memoryWidth, memoryHeight);
    glVertex2f(memoryWidth, 0.0);
    glEnd();


    int cellNumber = 1; 
    int k = 0;
   for (int i = numRows - 1; i >= 0; i--) {
    for (int j = 0; j < memoryCells; j++) {
            glPushMatrix();
            glTranslatef(j * cellWidth, i * cellHeight, 0.0);

            glColor3f(0.0, 0.0, 0.0);
            glLineWidth(2.0);
            glBegin(GL_LINE_LOOP);
            glVertex2f(0.0, 0.0);
            glVertex2f(0.0, cellHeight);
            glVertex2f(cellWidth, cellHeight);
            glVertex2f(cellWidth, 0.0);
            glEnd();

            float smallRectWidth = cellWidth * 0.8;
            float smallRectHeight = cellHeight * 0.8;
            glColor3f(0.5, 0.5, 0.5);
            glPushMatrix();
            glTranslatef((cellWidth - smallRectWidth) / 2.0, (cellHeight - smallRectHeight) / 2.0, 0.0);
            glBegin(GL_QUADS);
            glVertex2f(0.0, 0.0);
            glVertex2f(0.0, smallRectHeight);
            glVertex2f(smallRectWidth, smallRectHeight);
            glVertex2f(smallRectWidth, 0.0);
            glEnd();
            glPopMatrix();
           
                glColor3f(1.0, 1.0, 1.0);
                glRasterPos2f(cellWidth / 2.0 - 6.0, cellHeight / 2.0 - 6.0);
                char cellNumberText[3];
                int y= shmem_locations[k];
                if(k> columnsNum){y=0;}
                sprintf(cellNumberText, "%d", y);
                glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)cellNumberText);
            	 k++;


		if(swapFlag == 1){
			       
				if( y==shmem_locations[location2] ){
				 glColor3f(1.0, 0.0, 0.0);
               			 glRasterPos2f(cellWidth / 2.0 - 6.0, cellHeight / 2.0 - 6.0);
               			 char cellNumberText[3];
				 sprintf(cellNumberText, "%d",y);
               			 glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)cellNumberText);

				}

				if( y==shmem_locations[location1] ){
				 glColor3f(1.0, 0.0, 0.0);
               			 glRasterPos2f(cellWidth / 2.0 - 6.0, cellHeight / 2.0 - 6.0);
               			 char cellNumberText[3];
				 sprintf(cellNumberText, "%d",y);
               			 glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)cellNumberText);

				}

			}
            glPopMatrix();
            
            cellNumber++; 
        }
    }

    glPopMatrix();
}



void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    float memoryWidth = 0.6 * WINDOW_WIDTH;
    float memoryHeight = 0.8 * WINDOW_HEIGHT;
    float memoryX = (WINDOW_WIDTH - memoryWidth) / 2.0;
    float memoryY = (WINDOW_HEIGHT - memoryHeight) / 2.0;

    const char* text = "Shared Memory";
    int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);

    glColor3f(0.0, 0.0, 0.0); 
    glRasterPos2f(memoryX + (memoryWidth - textWidth) / 2.0, memoryY + memoryHeight + 38.0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text);

   
   

     if(masterSpy_win == threshold){
				sprintf(finalResText,"Operation Failed!");
				}
     else if (receiver_win == threshold){
				sprintf(finalResText, "Operation Successful!");
    }else{
          sprintf(finalResText, "  ");
    }
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, finalResText);
    glColor3f(1.0, 0.0, 0.0); 
    glRasterPos2f(memoryX + (memoryWidth - textWidth) / 2.0, memoryY + memoryHeight + 15.0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, finalResText);

    
    text = "Receiver";
    textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);
    glRasterPos2f(675.0, memoryY + memoryHeight + 20.0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text);

      text = "Master Spy";
    textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text);
    glRasterPos2f(20.0, memoryY + memoryHeight + 20.0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text);


    drawMemory();


    float senderRectX = memoryX - 150.0;
    float senderRectY = memoryY + memoryHeight - 400.0;


    drawRectangleWithText2(senderRectX, senderRectY, 125.0, 400.0,masterSpy_MsgRcved );

    
    textWidth = glutBitmapLength(GLUT_BITMAP_9_BY_15, (const unsigned char*)msg_masterSpy_gl);


    glColor3f(1.0, 0.0, 0.0); 
    glRasterPos2f(17, 100);
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)msg_masterSpy_gl);

    float receiverRectX = memoryX + memoryWidth + 15.0;  
    float receiverRectY = memoryY + memoryHeight - 400.0;

    drawRectangleWithText2(receiverRectX, receiverRectY, 125.0, 400.0,receiver_MsgRcved);

    
    textWidth = glutBitmapLength(GLUT_BITMAP_9_BY_15, (const unsigned char*)msg_receiver_gl);


    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2f(657, 100);
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)msg_receiver_gl);

  
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, winnerText);

    glColor3f(0.0, 0.0, 0.0); 
    glRasterPos2f(400,20);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, winnerText);

    char roundText[40];
    sprintf(roundText, "Round %d - ", rounds+1);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, roundText);

    glColor3f(0.0, 0.0, 0.0); 
    glRasterPos2f(280,20);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, roundText);


    char MasterSpy_score[40];
    sprintf(MasterSpy_score, "Score: %d",masterSpy_win);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, MasterSpy_score);


    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(35, 50);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)MasterSpy_score);


    char receiver_score[40];
    sprintf(receiver_score, "Score: %d", receiver_win);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, receiver_score);

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(680, 50);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)receiver_score);


           if(masterSpy_win == threshold || receiver_win ==  threshold){
			   
                       if(terminate == 0){

                          glutPostRedisplay();
		          glutSwapBuffers();
                          terminate=1;

                        }else{
                             clean (helperPids,number_helper_process,spyPids,number_spy_process);
  
			    sleep(7);
                            
                            exit(0);
                        }
			

            }else{

                     if (sum1win==1){
                   
		        rounds++;
			sum1win=0;
			printf("Stop\n");
		     	 send_stop_signalsMasterandRec();
			 send_stop_signalsHelperandSpy();

		            
			printf("Start\n");
		        send_start_signalsMasterandRec();
 			send_start_signalsHelperandSpy();                
			

		   }

                    parentRunning();
		    sleep(1);
		    glutPostRedisplay();
		    glutSwapBuffers();
		

               }

}



void reshape(int width, int height) {
	    glViewport(0, 0, width, height);

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0, width, 0, height);

	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
}

void parentRunning(){
		

       struct message msg;
    
        if (msgrcv(msgqid, &msg, sizeof(msg.mtext), 0, 0) == -1) {
            printf("parent --- Failed to get message from queue\n");
        }

        printf("\n---------> %s\n", msg.mtext);

	    char firstWord[MAX_WORD_LENGTH];
	    if (sscanf(msg.mtext, "%s", firstWord) != 1) {
		perror("No first word");
	    }
	    
	    if (strcmp(firstWord, "Helper") == 0) {

                swapFlag=1;
		extractLocations(msg.mtext, &location1, &location2);

			       int tmp;
			       tmp = shmem_locations[location2];
			       shmem_locations[location2] = shmem_locations[location1];
			       shmem_locations[location1] = tmp;

              printf("\n\n");

	    }

           if(strcmp(firstWord, "Receiver") == 0){
                   int num_msg_receiver;
                   
		   sprintf(winnerText, " ");
                   extractMsgNumberRcv(msg.mtext, &num_msg_receiver);
			if(receiver_MsgRcved[num_msg_receiver-1] != 0){
				 sprintf(msg_receiver_gl, "Msg-%d received\n(dropped)!\n",num_msg_receiver);
			}
			else{
				 sprintf(msg_receiver_gl, "Msg-%d received",num_msg_receiver);	
			}
                   receiver_MsgRcved[num_msg_receiver-1] = num_msg_receiver;
		   sprintf(msg_masterSpy_gl, "...");
		  

           }

           
             if(strcmp(firstWord, "MasterSpy") == 0){
                   int num_msg_MasterSpy;
		    sprintf(winnerText, " ");
                   extractMsgNumberRcv(msg.mtext, &num_msg_MasterSpy);
			if(masterSpy_MsgRcved[num_msg_MasterSpy-1] != 0){
				 sprintf(msg_masterSpy_gl, "Msg-%d received\n(dropped)!\n",num_msg_MasterSpy);
			}
			else{
				 sprintf(msg_masterSpy_gl, "Msg-%d received",num_msg_MasterSpy);	
			}
            
		   	masterSpy_MsgRcved[num_msg_MasterSpy-1] = num_msg_MasterSpy;
			sprintf(msg_receiver_gl, "...");
                  

            }

                         if(strcmp(firstWord, "Finish") == 0){
                  		          int process_finish;

					   extractMsgNumberRcv(msg.mtext, &process_finish);
					   if(process_finish == 0){
					      
					      int result = compareFiles(spyfile, senderfile);
						  if (result == 0) {
						      masterSpy_win++;
						      printf("masterSpy_win %d:",masterSpy_win );
							sprintf(msg_masterSpy_gl,"I am Fininshed!");
							sprintf(winnerText,"MasterSpy win ");

						 } 
						
                      
                   }else if(process_finish == 1){
		               int result = compareFiles(receiverfile, senderfile);
		                 if (result == 0) {
		                      receiver_win++;
		                      printf("receiver_win %d:",receiver_win );
					sprintf(msg_receiver_gl,"I am Fininshed!");
					sprintf(winnerText,"Receiver win ");
		                 } 


                   }
		empty_all_received();
		sum1win=1;
	
                
             }
}

void startOpenGl() {

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Encoding & Decoding Competition");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
}


void clean (pid_t *helperPids, int helperSize, pid_t *spyPids, int spySize){

  
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(1);
    }


   if(remove(FIFO_NAME) != 0){
	printf("Error with closing fifo\n");
	exit(-1);	
   }

    msgctl(msgqid, IPC_RMID, NULL);

    int key = ftok(SHARED_FILE, 'y');

    int msgqid2 = msgget(key, IPC_CREAT | 0666);
    if (msgqid2 == -1) {
        perror("msgget");
        exit(1);
    }

    msgctl(msgqid2, IPC_RMID, NULL);

   for(int i=0; i < helperSize; i++){
        kill(helperPids[i], SIGTERM);
   } 

    for(int i=0; i < spySize; i++){
        kill(spyPids[i], SIGTERM);
   } 

   kill(masterSpyPid, SIGTERM);
   kill(senderPid, SIGTERM);
   kill(receiverPid, SIGTERM);


}
