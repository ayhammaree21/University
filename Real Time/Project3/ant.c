#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define ANT_SPACING_MIN 0.07 
#define ANT_SPACING_INC 0.05 
#define WINDOW_SIZE 1.8
#define FOOD_PIECES 5

int NUM_ANTS;
float FOOD_TIME;
float SMELL_FOOD_DISTANCE;
float DISTANCE_WITH_ANT_SMELLED;
float PHEROMONE_QUANTITY;
float PORTION_FOOD_PER_ANT;
int TERMINATION_TIME;
time_t START_FOOD_INTERVAL;
time_t START_PROGRAM_INTERVAL;
float angleIncrement = 5.0 * (M_PI / 180.0); 
float radius = 0.1f;
int hours = 0;
int minutes = 0;
int seconds = 0;

//Ant struct
typedef struct {
   float x;
   float y;
   float angle; 
   float speed;
   float pheromone;
   int target_food;

} AntPosition;

AntPosition* antPositions; 
pthread_t* ant_threads;


typedef struct {
   float x;         
   float y;          
   float color[3];  
} Granule;

//Food struct
typedef struct {
   float x;
   float y;
   int antEatingNums;
   int number_of_granules;
   Granule* granules;
} Food;

Food food[FOOD_PIECES];
pthread_mutex_t mutex_food[FOOD_PIECES];  //define mutex for each piece of food

//Calculate distance between two points
float calculateDistance(float x1, float y1, float x2, float y2) {
   float dx = x2 - x1;
   float dy = y2 - y1;
   return sqrt(dx*dx + dy*dy);
}

//Function used to make sure that ant have a mimimum space between ants when initilized them
int isAntTooClose(AntPosition* antPositions, int numAnts, float x, float y) {
   for (int i = 0; i < numAnts; i++) {
      float distance = calculateDistance(antPositions[i].x, antPositions[i].y, x, y);
      if (distance < ANT_SPACING_MIN) {
         return 1; 
      }
   }
   return 0; 
}

//Function used to update the position of the ant
void updateAntPosition(int i) {
   float dx = antPositions[i].speed * cos(antPositions[i].angle); 
   float dy = antPositions[i].speed * sin(antPositions[i].angle);
   antPositions[i].x += dx;
   antPositions[i].y += dy;
}

//Function used to read user_defined_var file
void read_configuration() {
    char read[60];
    FILE* fptr;
    fptr = fopen("user_defined_var.txt", "r");
    if (fptr == NULL) {
        perror("Error while opening the file.\n");
        exit(-1);
    }

   char token[30];
    while (fscanf(fptr, "%s", token) == 1) {
        if (strcmp(token, "NUM_ANT") == 0) {
            fscanf(fptr, "%d", &NUM_ANTS);
           
        } else if (strcmp(token, "FOOD_TIME") == 0) {
            fscanf(fptr, "%f", &FOOD_TIME);
        } else if (strcmp(token, "SMELL_FOOD_DISTANCE") == 0) {
            fscanf(fptr, "%f", &SMELL_FOOD_DISTANCE);
        } else if (strcmp(token, "DISTANCE_WITH_ANT_SMELLED") == 0) {
            fscanf(fptr, "%f", &DISTANCE_WITH_ANT_SMELLED);
        } else if (strcmp(token, "PHEROMONE_QUANTITY") == 0) {
            fscanf(fptr, "%f", &PHEROMONE_QUANTITY);
        } else if (strcmp(token, "PORTION_FOOD_PER_ANT") == 0) {
            fscanf(fptr, "%f", &PORTION_FOOD_PER_ANT);
        } else if (strcmp(token, "TERMINATION_TIME") == 0) {
            fscanf(fptr, "%d", &TERMINATION_TIME);
        }
    }

    if (fclose(fptr) != 0) {
        perror("Error while closing the file.\n");
        exit(-1);
    }
}

//Function used to initilize location for ants
void initializeAnts(){

	srand(time(NULL) ^ (unsigned) getpid() << 16);
     for ( int numAnts = 0; numAnts < NUM_ANTS;) {
         float xTranslate = ((float)rand() / RAND_MAX) * WINDOW_SIZE - WINDOW_SIZE/2.0;
         float yTranslate = ((float)rand() / RAND_MAX) * WINDOW_SIZE - WINDOW_SIZE/2.0;
         int angleIndex = rand() % 8; 
         float angle = angleIndex * 45.0 * (M_PI / 180.0); 
         float speed = ((float)rand() / RAND_MAX) * 0.0009 + 0.0001; 

         int tooClose = isAntTooClose(antPositions, numAnts, xTranslate, yTranslate);
         if (!tooClose) {

            antPositions[numAnts].x = xTranslate;
            antPositions[numAnts].y = yTranslate;
            antPositions[numAnts].angle = angle;
            antPositions[numAnts].speed = speed;
            antPositions[numAnts].pheromone = 0.0;
	    antPositions[numAnts].target_food = -1;
            numAnts++;
         }

         xTranslate *= ANT_SPACING_INC;
         yTranslate *= ANT_SPACING_INC;
      }

}

//Function used to initilize new food
void initializeFood() {

   srand(time(NULL) ^ (unsigned) getpid() << 16);

    for (int i = 0; i < FOOD_PIECES; i++) {
            pthread_mutex_lock(&mutex_food[i]); 

            food[i].number_of_granules = rand() % 300 + 50;
            food[i].granules = malloc(food[i].number_of_granules  * sizeof(Granule));
            food[i].antEatingNums = 0;
            food[i].x = ((float)rand() / RAND_MAX) * (WINDOW_SIZE-0.4) - (WINDOW_SIZE-0.4)/2.0;
            food[i].y = ((float)rand() / RAND_MAX) * (WINDOW_SIZE-0.4) - (WINDOW_SIZE-0.4)/2.0;

	   for (int j = 0; j < food[i].number_of_granules ; j++) {
              
	      float angle = ((float)rand() / RAND_MAX) * 2 * M_PI; 
	      float r = sqrt((float)rand() / RAND_MAX) * radius;   
              
	      food[i].granules[j].x =  food[i].x + r * cos(angle);
	      food[i].granules[j].y = food[i].y + r * sin(angle);

	      food[i].granules[j].color[0] = (float)rand() / RAND_MAX;   
	      food[i].granules[j].color[1] = (float)rand() / RAND_MAX;     
	      food[i].granules[j].color[2] = (float)rand() / RAND_MAX; 
	   } 

           pthread_mutex_unlock(&mutex_food[i]);
   }


}

//Function used to draw food
void drawFood() {
 for (int i = 0; i < FOOD_PIECES; i++) {
   
       // Draw granules
	   for (int j = 0; j < food[i].number_of_granules; j++) {
	      glColor3fv(food[i].granules[j].color);
	      glPointSize(10.0);
	      glBegin(GL_POINTS);
	      glVertex2f(food[i].granules[j].x, food[i].granules[j].y);
	      glEnd();
	   }


  }
}

//Function used to draw ants
void drawAnts() {

 for (int i = 0; i < NUM_ANTS; i++) {
    glPushMatrix();
   glColor3f(0.0, 0.0, 0.0);
   glTranslatef(antPositions[i].x, antPositions[i].y, 0.0);
   glRotatef(antPositions[i].angle * 180.0 / M_PI, 0.0, 0.0, 1.0);

    glPushMatrix();
    glScalef(0.02, 0.013, 0.013);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.04, 0.0, 0.0);
    glScalef(0.01, 0.009, 0.009);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.025, 0.0, 0.0);
    glutSolidSphere(0.00625, 20, 20);
    glPopMatrix();

    const float legLength = 0.018;
    const float legAngle = 45.0;

    // Top left leg
    glPushMatrix();
    glBegin(GL_LINE_STRIP);
    float x= -0.02,y=0.007;
    glVertex2f(-x, -y);
    glVertex2f(-x - legLength * cosf(legAngle * M_PI / 180.0), -y - legLength * sinf(legAngle * M_PI / 180.0));
    glVertex2f(-x - legLength * cosf((legAngle + 15.0) * M_PI / 180.0), -y - legLength * sinf((legAngle + 15.0) * M_PI / 180.0));
    glEnd();
    glPopMatrix();

    // Top right leg
    glPushMatrix();
    glBegin(GL_LINE_STRIP);
    float x1= 0.03,y1=0.006;
    glVertex2f(x1, -y1);
    glVertex2f(x1 + legLength * cosf(legAngle * M_PI / 180.0), -y1 - legLength * sinf(legAngle * M_PI / 180.0));
    glVertex2f(x1 + legLength * cosf((legAngle + 15.0) * M_PI / 180.0), -y1 - legLength * sinf((legAngle + 15.0) * M_PI / 180.0));
    glEnd();
    glPopMatrix();

    // Bottom left leg
    glPushMatrix();
    glBegin(GL_LINE_STRIP);
    float x2=- 0.02,y2=-0.007;
    glVertex2f(-x2, -y2);
    glVertex2f(-x2 - legLength * cosf(legAngle * M_PI / 180.0), -y2 + legLength * sinf(legAngle * M_PI / 180.0));
    glVertex2f(-x2 - legLength * cosf((legAngle + 15.0) * M_PI / 180.0), -y2 + legLength * sinf((legAngle + 15.0) * M_PI / 180.0));
    glEnd();
    glPopMatrix();

    // Bottom right leg
    glPushMatrix();
    glBegin(GL_LINE_STRIP);
    float x4= 0.03,y4=-0.006;
    glVertex2f(x4, -y4);
    glVertex2f(x4 + legLength * cosf(legAngle * M_PI / 180.0), -y4 + legLength * sinf(legAngle * M_PI / 180.0));
    glVertex2f(x4 + legLength * cosf((legAngle + 15.0) * M_PI / 180.0), -y4 + legLength * sinf((legAngle + 15.0) * M_PI / 180.0));
    glEnd();
    glPopMatrix();

    glPopMatrix();

  }
}

//Function used to terminate program
void terminate_program(){

  for (int i = 0; i < NUM_ANTS; i++) {
        pthread_cancel(ant_threads[i]);
    }

  free(ant_threads);
  free(antPositions);

  for(int i=1;i<FOOD_PIECES;i++){
         free(food[i].granules);
         pthread_mutex_destroy(&mutex_food[i]);
   }

   exit(0);
}

void display() {

   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);

     char timerStr[20];
     glColor3f(1.0, 0.0, 0.0);
     glRasterPos2f(-0.05, 0.95);
     sprintf(timerStr, "%02d:%02d:%02d", hours, minutes, seconds);
       
    int length = strlen(timerStr);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, timerStr[i]);
    }

   time_t CURRENT_TIME=time(NULL);
		if(CURRENT_TIME - START_FOOD_INTERVAL >= FOOD_TIME){

                initializeFood();
	        START_FOOD_INTERVAL =time(NULL);
      }

   drawFood();
   drawAnts();
 
   glutPostRedisplay(); 
   glFlush();
}


//Function used to update timer every 1 sec.
void timer(int value) {
    
    if (hours == 0 && minutes == 0 && seconds == 0) {
         printf("Timer expired!\n");
        terminate_program();
    } else {
        if (seconds == 0) {
            if (minutes == 0) {
                hours--;
                minutes = 59;
            } else {
                minutes--;
            }
            seconds = 59;
        } else {
            seconds--;
        }

        glutTimerFunc(1000, timer, 0);
    }
}

//Fucntion used to calculate distance between ant and food
float calculateDistanceAntFood(float x1, float y1, float x2, float y2) {
    float distance;
    distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distance;
}

//Function used to return the nearest food to ant
int smell_food(int antNum){

        pthread_mutex_lock(&mutex_food[0]);
	float min_distance =calculateDistanceAntFood(food[0].x,food[0].y,antPositions[antNum].x,antPositions[antNum].y);
        pthread_mutex_unlock(&mutex_food[0]);

	int near_food_num=0;
	for(int i=1;i<FOOD_PIECES;i++){
                 pthread_mutex_lock(&mutex_food[i]);
		float distance = calculateDistanceAntFood(food[i].x,food[i].y,antPositions[antNum].x,antPositions[antNum].y);
		if(distance<min_distance){
			min_distance = distance;
			near_food_num = i;
			}
                pthread_mutex_unlock(&mutex_food[i]);
	}

  // check if there are any granules left in the food
  if(food[near_food_num].number_of_granules ==0){
         return -1;
   }

   return near_food_num;
}

//Function used to create ant_thread
void* ant_thread(void* data ) {
  
    int i = *((int*)data);     
    srand(time(NULL) ^ (unsigned) getpid() << 16);
    while (1) {

	char antStatus[50];
        strcpy(antStatus, "walk normal");
	    
	int near_food_num = smell_food(i);

       if(near_food_num != -1){


                         pthread_mutex_lock(&mutex_food[near_food_num]);
                         //Find distance between ant and nearest food
			 float distance =calculateDistanceAntFood(food[near_food_num].x,food[near_food_num].y,antPositions[i].x,antPositions[i].y);  
                         pthread_mutex_unlock(&mutex_food[near_food_num]);

		        // check if the ant is on the top  of the food
			if (distance < 0.06 ){

					strcpy(antStatus, "stop on top food");
					antPositions[i].target_food = near_food_num;
					if(distance != 0 ){
						antPositions[i].pheromone = 1 /distance;
					}

					else {
						antPositions[i].pheromone = 16; //maximum pheromone
					}

			}
			//If ant smells food within SMELL_FOOD_DISTANCE (user-defined)
			else if(distance <= SMELL_FOOD_DISTANCE){

					strcpy(antStatus, "forward to food");
					antPositions[i].pheromone = 1 /distance;
					antPositions[i].target_food = near_food_num;
							
			}

		       //If ant does not smell food within SMELL_FOOD_DISTANCE, then smell pheronome from near ants
			else if(distance > SMELL_FOOD_DISTANCE){  

				float max_phermone_near_ant=0.0; 


				for(int j=0;j<NUM_ANTS;j++){ // smell pheromone from near ants
		 
				       //Find distance between ant and near ants
					float smell_ant_distance = calculateDistanceAntFood(antPositions[j].x,antPositions[j].y,antPositions[i].x,antPositions[i].y); 
		 
					if(smell_ant_distance <= DISTANCE_WITH_ANT_SMELLED){  

						if(antPositions[j].pheromone > max_phermone_near_ant){ //check for the highest pheromone

								max_phermone_near_ant = antPositions[j].pheromone;
								near_food_num = antPositions[j].target_food;

						}			
					}
				}

			      
				//check if the ant smells pheronome from another ant within SMELL_FOOD_DISTANCE,then go forward to food 				
				if(max_phermone_near_ant >= 1/SMELL_FOOD_DISTANCE){ 

				        strcpy(antStatus, "forward to food");

                                        pthread_mutex_lock(&mutex_food[near_food_num]);
				        float distance = calculateDistanceAntFood(food[near_food_num].x,food[near_food_num].y,antPositions[i].x,antPositions[i].y);
                                        pthread_mutex_unlock(&mutex_food[near_food_num]);

					antPositions[i].pheromone = 1 /distance;
					antPositions[i].target_food = near_food_num;

				}else{
		  
				 	 // else if the ant smells lesser quantity of pheronomethen ,then rotate by 5 degree towards the food
				    	if(max_phermone_near_ant >= PHEROMONE_QUANTITY ){ 
							strcpy(antStatus, "rotate 5 degree");
							antPositions[i].target_food = near_food_num;
							antPositions[i].pheromone = 0.0;
					}

					//if pheronone was too small(ant does not smell) ,then continue as normal
					else if(max_phermone_near_ant < PHEROMONE_QUANTITY){ 
							 strcpy(antStatus, "walk normal");
							antPositions[i].pheromone = 0.0;
							antPositions[i].target_food = -1;
					}

				}
	
			}
           }

		//Update the ant position based on the antStatus
		if (strcmp(antStatus, "stop on top food") == 0){
                       pthread_mutex_lock(&mutex_food[antPositions[i].target_food]);

                       food[antPositions[i].target_food].antEatingNums++;
		       // each ant eats portion of food = PORTION_FOOD_PER_ANT (decrease portion of each ant) 
                       food[antPositions[i].target_food].number_of_granules -=  PORTION_FOOD_PER_ANT;
                       if ( food[antPositions[i].target_food].number_of_granules  < 0){
                             food[antPositions[i].target_food].number_of_granules=0;
                       }
                      
                      pthread_mutex_unlock(&mutex_food[antPositions[i].target_food]);
                      sleep(1);
          	}

		
		else if(strcmp(antStatus, "forward to food") == 0){

                        pthread_mutex_lock(&mutex_food[near_food_num]);
			float angle = atan2(food[near_food_num].y - antPositions[i].y, food[near_food_num].x - antPositions[i].x);
			pthread_mutex_unlock(&mutex_food[near_food_num]);

			antPositions[i].angle = angle;
			
			updateAntPosition(i);

		}else if(strcmp(antStatus, "rotate 5 degree") == 0){

                        pthread_mutex_lock(&mutex_food[near_food_num]);
                        float desiredAngle = atan2f(food[near_food_num].y - antPositions[i].y, food[near_food_num].x - antPositions[i].x);
                        pthread_mutex_unlock(&mutex_food[near_food_num]);

                        float angleDiff = desiredAngle - antPositions[i].angle;
                        
			    if (fabsf(angleDiff) < angleIncrement) {
				antPositions[i].angle = desiredAngle;
			    } else {

				if (angleDiff > angleIncrement) {
				    antPositions[i].angle += angleIncrement;
				} else if (angleDiff < -angleIncrement) {
				    antPositions[i].angle -= angleIncrement;
				}

			    }
                          updateAntPosition(i);
                          usleep(500000); 
                          
                }

		else{

			  updateAntPosition(i);
			  // if the ant hits the limit of the simulation window, then rotate by 45 degree and continue walking
	    		  if (antPositions[i].x < -WINDOW_SIZE/2.0 || antPositions[i].x > WINDOW_SIZE/2.0 ||
					   antPositions[i].y < -WINDOW_SIZE/2.0 || antPositions[i].y > WINDOW_SIZE/2.0) {
					   antPositions[i].angle += M_PI;
                                           updateAntPosition(i);
					   //Choose how the ant rotate (CW/CCW)
                                           if((rand() % 10 + 1) % 2 == 0){
                                             antPositions[i].angle += M_PI/4.0;
                                           }else{
                                              antPositions[i].angle -= M_PI/4.0;
                                           }

				           updateAntPosition(i);              
	     		  }	
		}

     usleep(16000); 	
     pthread_testcancel();	  
     }
       
}


int main(int argc, char** argv) {

   read_configuration();
   minutes = TERMINATION_TIME;
   antPositions =malloc(NUM_ANTS * sizeof(AntPosition));

   initializeAnts(); 
   
   for (int i = 0; i < FOOD_PIECES; i++) {
        pthread_mutex_init(&mutex_food[i], NULL);
    }

   initializeFood();

    ant_threads = malloc(NUM_ANTS * sizeof(pthread_t));
    // create thread for each ant
    for (int i = 0; i < NUM_ANTS; i++) {
        pthread_create(&ant_threads[i], NULL,(void*) ant_thread, (void*)&i);
        usleep(10000);
    }

    
   START_FOOD_INTERVAL = time(NULL);


   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(1200, 1200);
   glutCreateWindow("Ant Searching for Food Simulation");
   glClearColor(1.0, 1.0, 1.0, 0.0);
   gluOrtho2D(-1, 1, -1, 1);
   glutDisplayFunc(display);
   glutTimerFunc(1000, timer, 0);
   glutMainLoop();
   return 0;
}

