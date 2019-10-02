// Game of Drones
//
// Find the drone in a grid
//
// *** Lines marked "Do not remove" should be retained in your code ***
//
// Warning: Return values of calls are not checked for error to keep
// the code simple.
//
// Requires drone.h to be in the same directory
//
// Compilation command on ADA:
//
//   module load intel/2017A
//   icc -o drone.exe drone.c -lpthread -lrt
//
// Sample execution and output ($ sign is the shell prompt):
//
// $ ./drone.exe
//   Need four integers as input
//   Use: <executable_name> <grid_size> <random_seed> <delay_nanosecs> <move_count>
// $ ./drone.exe 256 0 1000000 0
//   Drone = (111,195), success = 1, time (sec) =   3.4672
//
//
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include<iostream>

#include "drone.h"		// Do not remove
struct timespec start, stop; 	// Do not remove
//clock_t start, stop;

double total_time;		// Do not remove
int gridsize;			// Do not remove

unsigned int drone_x, drone_y; 	//Coordinates of drone (to be found)

#define MAX_THREADS     32745

int thread_id[MAX_THREADS];	// User defined id for thread
pthread_t p_threads[MAX_THREADS];// Threads
pthread_attr_t attr;		// Thread attributes

pthread_mutex_t lock_barrier;	// Protects count
pthread_cond_t cond_barrier;	// Monitors count

void *get_grid_pos(void *s);

// -------------------------------------------------------------------------
// Data structures and multithreaded code to find drone in the grid
// ...
// ...
// ...
// ...
// ...
// ...


// -------------------------------------------------------------------------
// Main program to find drone in a grid
int main(int argc, char *argv[]) {

    if (argc != 5) {
	printf("Need four integers as input \n");
	printf("Use: <executable_name> <grid_size> <random_seed> <delay_nanosecs> <move_count>\n");
	exit(0);
    }
    // Initialize grid
    gridsize = abs((int) atoi(argv[argc-4])); 		// Do not remove
    int seed = (int) atoi(argv[argc-3]); 		// Do not remove
    int delay_nsecs = abs((int) atoi(argv[argc-2]));// Do not remove
    int move_count = abs((int) atoi(argv[argc-1]));	// Do not remove
    initialize_grid(gridsize, seed, delay_nsecs, move_count); // Do not remove
    gridsize = get_gridsize();	 			// Do not remove


    // Initialize mutex, condition variable, and attribute structures
    pthread_mutex_init(&lock_barrier, NULL);
    pthread_cond_init(&cond_barrier, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    unsigned int num_threads = MAX_THREADS -100;
    unsigned int num_cells = gridsize*gridsize;

//    print_drone_path();

    clock_gettime(CLOCK_REALTIME, &start); 	// Do not remove
//start = clock();

  std::cout<<"num_cells : "<<num_cells<<" num_threads : "<<num_threads<<std::endl;

    if(num_cells<=num_threads)
    {
      for (int i = 0; i < num_threads; i++)
      {
        thread_id[i] = i;
        pthread_create(&p_threads[i], &attr, get_grid_pos, (void *) &thread_id[i]);
      }

      // Join threads
      /*
      for (int i = 0; i < num_threads; i++)
      {
        pthread_join(p_threads[i], NULL);

      }
      std::cout<<"Done joining"<<std::endl;*/
    }

    /* If number of cells is greater than the number of threads */
    else
    {
      unsigned int diff_bw_cells_threads = num_cells-num_threads;
      unsigned int count_for_threads = num_cells/num_threads;

      for(int j = 0;j<count_for_threads;j++)
      {

      }

    }


    // Multithreaded code to find drone in the grid
    // ...
    // ...
    // ... sample serial code shown below ...
    // ... works when drone is not allowed to move in check_grid() ...
    unsigned int i, j;
    int chk;
    for (i = 0; i < gridsize; i++) {
	for (j = 0; j < gridsize; j++) {
	    chk = check_grid(i,j);
	    if (chk == 0) {
		drone_x = i;
		drone_y = j;
	    }
	}
    }
    // ...
    // ...
    // ...

    // Compute time taken
    clock_gettime(CLOCK_REALTIME, &stop);			// Do not remove
    total_time = (stop.tv_sec-start.tv_sec)			// Do not remove
	+0.000000001*(stop.tv_nsec-start.tv_nsec);		// Do not remove
//stop = clock();
//total_time = (1.0*stop-start)/CLOCKS_PER_SEC;

    // Check if drone found, print time taken
    printf("Drone = (%u,%u), success = %d, time (sec) = %8.4f\n", // Do not remove
    drone_x, drone_y, check_drone_location(drone_x,drone_y), total_time);// Do not remove

    // Other code to wrap up things
    // ...
    // ...
    // ...

}


void *get_grid_pos(void *s)
{
    int index = *((int *)s);
    unsigned int row=(int)(index/(get_gridsize()-1));
    unsigned int column = index%(get_gridsize()-1);
    //std::cout<<"Grid position = "<<row<<" "<<column<<std::endl;
    if(check_grid(row,column) == 0)
    {
      std::cout<<"Success in Grid position = "<<row<<" "<<column<<"; thread id = "<<index<<std::endl;
    }
    pthread_detach(pthread_self());

}
