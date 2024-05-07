#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>
#include <GL/glut.h>

#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE 1024
#define NUM_CHILDREN 4

void set_min_max();
void signal_handler(int);
void sent_signals_to_children();
void read_children_rand_num();
void terminate_program();
void display();
void startOpenGl();

int score_team1=0;
int score_team2=0;
int round = 0;
int rounds;
float sum1=0.0, sum2=0.0;
int  min, max = 0;
pid_t pid[NUM_CHILDREN];
int readyChildren=0;
float nums[NUM_CHILDREN];
int fd;
char buffer_send[BUFFER_SIZE];
char buffer_receive[BUFFER_SIZE];
pid_t co_processor_pid;


int main(int argc, char *argv[ ]) {
     
    int status;
    struct sigaction sa;

    // Set up signal handler
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    rounds = 5;  // default value

    if (argc == 2) {
        char *endptr;
        long int val = strtol(argv[1], &endptr, 10);

        // Check if the argument is a valid integer
        if (*endptr == '\0' && val >= 0 && val <= INT_MAX) {
            rounds = (int)val;
        } else {
            printf("Invalid argument: %s\n", argv[1]);
	    return EXIT_FAILURE;
        }
    } else if (argc > 2) {
        printf("Too many arguments.\n");
        printf("Usage: %s [num_rounds]\n", argv[0]);
        return EXIT_FAILURE;
    }


  if (access(FIFO_NAME, F_OK) == 0) {
    //Remove existing FIFO if it already exists 
    if (unlink(FIFO_NAME) == -1) {
        perror("Error removing existing FIFO");
        exit(-1);
     }
  }

    // Create the FIFO if it doesn't already exist
    mkfifo(FIFO_NAME, 0666);
   
    printf("\t---CREATION PROCESS---\n\n");

   // Create the co-processor process
    int pi = fork();
    if (pi == -1) {
        printf("Error with forking\n");
        exit(-1);
    } else if (pi == 0) { // child (co-processor)
         execl("child", "child", "No seed","co-processor", NULL);
        // The above line replaces the child process code with the co-processor code
    }else if(pi != 0){
       co_processor_pid = pi;
       printf("\tCo-processor with ID : %d\n",co_processor_pid);
    }

    // Wait for the co-processor process to finish initialization
    sleep(2);


    // Create child processes
   for (int i = 0; i < NUM_CHILDREN; i++) {
        
        int p = fork();
        if(p ==-1){
              printf("Error with forking");
	      exit(-1);
        }
        else if (p == 0){  //child
            // Seed random number generator with current time and child PID
            srand(time(NULL) ^ (unsigned) getpid() << 16);
            char seed_str[20];
            sprintf(seed_str, "%d", rand());
            execl("child", "child", seed_str,"player", NULL);
           
         }  else if (p != 0){ // parent
           pid[i]=p;
           printf("\tPlayer %d with ID: %d\n",i+1,pid[i]);
         }
         
       sleep(2);
    }

 glutInit(&argc, argv);
 startOpenGl();


}

void set_min_max(){

   // Generate random integers
   struct timeval tv;
   gettimeofday(&tv, NULL);
   srand(tv.tv_sec * 1000000 + tv.tv_usec ^ getpid()<<16);
   min = rand() % 100 + 1;
   max = rand() % 100 + 1;

   while (min == max) {
        min = rand() % 100 + 1;
        max = rand() % 100 + 1;
    }

    if (min > max ){
      int tmp = min;
      min = max;
      max = tmp;
    }
    printf("\n\tThe Range in this round: %d - %d\n", min,max);
    
    // Save the range in file
    FILE *fpf = fopen("range.txt", "w");
    fprintf(fpf, "%d,%d", min, max);
    fclose(fpf);

}

void sent_signals_to_children(){
    for (int i = 0; i < NUM_CHILDREN; i++) {
        kill(pid[i], SIGINT);
        sleep(2);
    }
}

void read_children_rand_num(){

      // Read generated numbers from files
     char filename[20];
     FILE *fp;

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t child_pid = pid[i];
        sprintf(filename, "%d.txt", child_pid);
        fp = fopen(filename, "r");
        if (fp == NULL) {
        printf("Failed to open file.\n");
        exit(1);
        }
         
       if (fscanf(fp, "%f", &nums[i]) < 1) {
        printf("Failed to read from file.\n");
        exit(1);
        }
        fclose(fp);
    }

    // Print generated numbers
    for (int i = 0; i < NUM_CHILDREN ; i++) {
        if(i==0){
         printf("\nTeam 1: ");
        }else if( i==2){
         printf("\nTeam 2: ");
        }
        printf("\tPlayer %d generated number: %.1f ", i+1,nums[i]);
    }
    printf("\n");

}


void signal_handler(int sig) {

    if(readyChildren == 3){
       readyChildren++;
       readyChildren=0;
       read_children_rand_num();

   // Write a message to the FIFO
      sprintf(buffer_send, "%.1f,%.1f,%.1f,%.1f", nums[0], nums[1], nums[2],nums[3]);
   
    kill(co_processor_pid, SIGUSR1); 
    sleep(3);

    // Open the FIFO for writing
       if ((fd = open(FIFO_NAME, O_RDWR)) == -1) {
          perror("open");
          exit(EXIT_FAILURE);
       }
      
      if(write(fd, buffer_send, sizeof(buffer_send))<0){
       perror("write");
       exit(1);
      }

      sleep(2);

      if(read(fd,buffer_receive,sizeof(buffer_receive)) <0){
        perror("read");
       exit(1);
       }
       
       close(fd);
       
       sscanf(buffer_receive, "%f,%f", &sum1, &sum2);
       printf("\n\tSummation for team 1: %.1f\t Summation for team 2: %.1f\n\n",sum1,sum2);
       
       if(sum1 > sum2){
        score_team1++;
       }else if(sum1 < sum2){
        score_team2++;
       }else{
        score_team1++;
        score_team2++;
       }
     
    }else{
        readyChildren++;
     }
    
}



void terminate_program(){

	        // remove fifo file
		if(remove(FIFO_NAME) != 0){
			printf("Error with closing fifo\n");
			exit(-1);	
		}
		//kill children
                for(int i=0; i < NUM_CHILDREN; i++){
		      kill(pid[i], SIGTERM);
	        } 
	        //kill co-processor
                 kill(co_processor_pid, SIGTERM);

		//remove children files
                char filename[20];
                for (int i = 0; i < NUM_CHILDREN; i++){

                   sprintf(filename, "%d.txt", pid[i]);
                   // remove range file
                   if (remove(filename) != 0) {
                         printf("Failed to remove file.\n");
                    } 
                }


                if (remove("range.txt") != 0) {
                         printf("Failed to remove (range.txt) file.\n");
                    } 
	
	exit(0);
}

void drawSquare(float x, float y, float size, float value, float r, float g, float b, const char* text)
{

    glColor3f(r, g, b);

    glBegin(GL_QUADS);
    glVertex2f(x - size, y - size);
    glVertex2f(x + size, y - size);
    glVertex2f(x + size, y + size);
    glVertex2f(x - size, y + size);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);

    glRasterPos2f(x - size + 0.11, y + size - 0.06);

    for (int i = 0; i < strlen(text); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }

    char valueText[20];
    sprintf(valueText, "%.1f", value);

    glColor3f(0.0, 0.0, 0.0);

    glRasterPos2f(x - strlen(valueText) * 0.03, y - 0.008);

    for (int i = 0; i < strlen(valueText); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, valueText[i]);
    }

    
}


void drawRect(float x, float y, float width, float height, float val1, float val2, float r, float g, float b, const char* text)
{
 
    glColor3f(r, g, b);

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);

    glRasterPos2f(x + width/2 - strlen(text) * 0.015, y + height - 0.06);

    for (int i = 0; i < strlen(text); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }

    char val1Text[20];
    sprintf(val1Text, "Team1 res: %.1f", val1);
    char val2Text[20];
    sprintf(val2Text, "Team2 res: %.1f", val2);

    glColor3f(0.0, 0.0, 0.0);

    glRasterPos2f(x + 0.01, y + height/2 - 0.05);

    for (int i = 0; i < strlen(val1Text); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, val1Text[i]);
    }

    glColor3f(0.0, 0.0, 0.0);

    glRasterPos2f(x + width - strlen(val2Text) * 0.03 - 0.01, y + height/2 - 0.05);

    for (int i = 0; i < strlen(val2Text); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, val2Text[i]);
    }
}



void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
   
    drawSquare(-0.5, 0.5, 0.25, nums[0], 0.0, 1.0, 1.0,"Player 1");
    drawSquare(-0.5, -0.1, 0.25, nums[1], 0.0, 1.0, 1.0,"Player 2");
    drawSquare(0.5, 0.5, 0.25, nums[2], 0.5, 1.0, 0.5,"Player 3");
    drawSquare(0.5, -0.1, 0.25, nums[3], 0.5, 1.0, 0.5,"Player 4");

    drawRect(-0.7, -0.7, 1.4, 0.2,sum1,sum2 ,1.0, 1.0, 1.0, "Co-processor");

    glColor3f(1.0, 1.0, 1.0);

    //Team1
    glRasterPos2f(-0.6, 0.8);
    const char* team1 = "Team 1";
    for (int i = 0; i < strlen(team1); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, team1[i]);
    }

    //Team2
    glRasterPos2f(0.4, 0.8);
    const char* team2 = "Team 2";
    for (int i = 0; i < strlen(team2); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, team2[i]);
    }

     glColor3f(1.0, 0.0, 0.0);
     glRasterPos2f(-0.1, 0.9);

    // Print the round number
    char roundText[10];
    sprintf(roundText, "Round %d", round);
    int length = strlen(roundText);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, roundText[i]);
    }

     glColor3f(1.0, 0.0, 0.0);
     glRasterPos2f(-0.18, 0.8);

    char rangeText[20];
    sprintf(rangeText, "Range: %d - %d", min ,max);
     length = strlen(rangeText);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, rangeText[i]);
    }

     glColor3f(1.0, 0.0, 0.0);
     glRasterPos2f(-0.7, -0.8);
    char team1Score[30];
    sprintf(team1Score, "Team 1 score = %d", score_team1);
     length = strlen(team1Score);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, team1Score[i]);
    }


     glRasterPos2f(0.2, -0.8);
    char team2Score[30];
    sprintf(team2Score, "Team 2 score = %d", score_team2);
     length = strlen(team2Score);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, team2Score[i]);
    }


     glRasterPos2f(-0.4, -0.9);
    char win[40];
    
    if(score_team1 > score_team2){

         sprintf(win, "*** The winner is Team 1 ****");

    }else if(score_team1 < score_team2){

        sprintf(win, "*** The winner is Team 2 ****");

    }else{
    
       sprintf(win, "*** The two teams are tied ****");
    }

   if(round == rounds){

       for (int i = 0; i < strlen(win); i++)
       {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, win[i]);
       }
 
   }
  
    
    glFlush();
     if(round <rounds){
        printf("\n\t**************************** Round #%d ****************************\n",round+1);
           set_min_max();

          sent_signals_to_children();
          round++;
          if(round !=1 ){
          sleep(7);
          }
          glutPostRedisplay();
          glutSwapBuffers();

    
    }else{
        
 	 printf("\n\t**************************** Final Result ****************************\n");
 	 printf("\n\t\t\tTeam 1 score = %d \tTeam 2 score = %d\n", score_team1,score_team2);

 	 if(score_team1 > score_team2){
     	  printf("\n\t\t\t*** The winner is Team 1 ****\n\n");
 	 }else if(score_team1 < score_team2){
     	  printf("\n\t\t\t*** The winner is Team 2 ****\n\n");
 	 }else{
   	    printf("\n\t\t\t*** The two teams are tied ****\n\n");
 	 }
       	  sleep(7);
          terminate_program();
   	 }

}


void startOpenGl() {

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("OpenGL Square");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(display);
    glutMainLoop();
}

