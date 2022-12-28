#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//thread initialization
int thread_count;
const int MAX_THREAD = 4;
const int MIN_THREAD = 2;
pthread_mutex_t mutexsum; //declar mutex

//program initialization
int i,m,n,thread;
float hypotenuse;
float sides[2];

// functons
void Error(char* prog_name);
void square_side (void *);

int main(int argc , char* argv[])
{
    //check N. Argument
    if(argc != 2){
        // Error Function (Project Name )
        Error(argv[0]);
    }

    // store argv[1] (second Argument) in thread count
    thread_count = atoi(argv[1]); //string to number direct

    // handle thread count to be in range MIN_THREAD and MAX_THREAD
    if(thread_count < MIN_THREAD || thread_count >= MAX_THREAD){
        Error(argv[0]);
    }
    printf("- By this Project you can find Hypotenuse value through Pythagoras theorem \n");
    printf("- now should input the 2 side of Right-angled triangle \n");
    printf ("-------------------------------------------\n");
    // take value of 2 side to triangle
    printf("Enter Number of side1 : ");
    scanf("%d", &m);
    printf("Enter Number of side2 : ");
    scanf("%d", &n);
    // store value of 2 side in array
    sides[0]=(float)m;
    sides[1]=(float) n;
    // check value greater zero
    if ((sides[0] < 1) || (sides[1] < 1)){
        printf("Error: should > 0 : \n");
        //exit program
        exit (EXIT_FAILURE);
    }
    // pointer from type pthread
    pthread_t* thread_handles;
    //Memory Allocation
    thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    if (thread_handles == NULL){
        fprintf (stderr, "File: %s, line %d: Can't allocate memory.",__FILE__, __LINE__);
        exit (EXIT_FAILURE);
    }
    printf ("-------------------------------------------\n");
    printf ("| Pythagoras' theorem :-  a^2 + b^2 = c^2 | \n");
    printf ("-------------------------------------------\n");
    hypotenuse = 0;

    /* Initialize the mutex to protect share data (hypotenuse) 		*/
    // (name of mutex in project - attribute)
    pthread_mutex_init (&mutexsum, NULL);

    /* Create the threads	and calculate the squares on the sides		*/
    // (thread p - attribute p - function - function argument)
    /* pthread_create (&thread_handles[0], NULL, square_side, &sides[0]);
    pthread_create (&thread_handles[1], NULL, square_side, &sides[1]);*/
    for (thread = 0; thread < thread_count; thread++){
      pthread_create (&thread_handles[thread], NULL, square_side, &sides[thread]);
    }
    /* After thread end of - Using join to syncronize the threads				*/
    for (i = 0; i < thread_count; i++){
        // ( thread p - return from thread)
        pthread_join (thread_handles[i], NULL);
    }
    //print main
    printf ("-------------------------------------------\n");
    printf ("Hypotenuse -> sqrt(((a)^2) + ((b)^2))) =  %.2f\n", sqrt(hypotenuse));

    /* Deallocate any memory or resources associated			*/
    pthread_mutex_destroy (&mutexsum);
    free (thread_handles);


    return 0;
}

void Error(char* prog_name){
    fprintf(stderr , "Error %s : thread_count is not available or not range of ( %d to %d ) , please try again \n" , prog_name , MIN_THREAD , MAX_THREAD);
    exit(0);
}
void square_side (void *a)
{
  float side;

  /* Get the value of the triangle side	and print the square 		*/
  side = *( ( float* )a );
  printf ("square side ->((side)^2 ) -> ((%.2f)^2 ) = %.2f \n", side, side * side);

  /* Mutex lock/unlock to safely update the value of hypotenuse		*/
  // only one thread enter
  pthread_mutex_lock (&mutexsum);
  hypotenuse += side * side;
  //a thread is finished executing the code in a critical section
  pthread_mutex_unlock (&mutexsum);

  pthread_exit (EXIT_SUCCESS);		/* Terminate the thread		*/
}


