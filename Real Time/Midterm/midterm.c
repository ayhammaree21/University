#include "local.h"

#define FEMALE 0;
#define MALE 1;


void read_configuration();
void* chickBehavior(void* arg);


void* chickBehavior(void* arg) {
    Chick* chick = (Chick*)arg;
    printf("Chick %d: I am a chick!\n", chick.id);

    printf("Chick %d: I am growing feathers.\n", chick.id);
    sleep(1);

    printf("Chick %d: I am learning to walk.\n", chick.id);
    sleep(1);
    sleep(rand() % 5 + 1);
    
    printf("Chick %d: I have developed into a hen or rooster.\n", chick.id);
   
    sleep(rand() % 5 + 1);

	if (chick.gender == FEMALE) {
            printf("Chick %d: I am female.\n", chick.id);
            sleep(1);
        }

        if (chick.gender == MALE) {
            printf("Chick %d: i am male !\n", chick.id);
            sleep(1);
        }
        if (isSoldOrDead(chick)) {
            break;
        }
    }
    
    printf("Chick %d: I am being sold or I have died.\n", chick.id);
    
    free(chick);
    return NULL;
}

void read_configuration() {
	    char read[60];
	    FILE* fptr;
	    fptr = fopen("arguments.txt", "r");
	    if (fptr == NULL) {
		perror("Error while opening the file.\n");
		exit(-1);
	    }

	   char token[30];
	    while (fscanf(fptr, "%s", token) == 1) {
		if (strcmp(token, "Number_of_cows") == 0) {
		    fscanf(fptr, "%d", &Number_of_cows);
		   
		} else if (strcmp(token, "Number_of_mares") == 0) {
		    fscanf(fptr, "%d", &Number_of_mares);
		} 
	    }


 	while (fscanf(fptr, "%s", token) == ) {
		if (strcmp(token, "Milk_Price_Range") == 0) {
		    fscanf(fptr, "%f", &Milk_Price_Range);
		   
		} else if (strcmp(token, "Cheese_Price_Range") == 0) {
		    fscanf(fptr, "%f", &Cheese_Price_Range);
		} 
	    }

	    if (fclose(fptr) != 0) {
		perror("Error while closing the file.\n");
		exit(-1);
	    }
	}


int main(int argc, char *argv[]){
	srand(time(NULL)); 
	pthread_t chickThreads[10];
	 for (int i = 0; i < 10; i++) {
		Chick* chick = (Chick*)malloc(sizeof(Chick));
		chick.id = i + 1;
		pthread_create(&chickThreads[i], NULL, chickBehavior, (void*)chick);
	    }
	   
	    for (int i = 0; i < 10; i++) {
		pthread_join(chickThreads[i], NULL);
	    }
	    
	    return 0;
}

