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
//#include<iostream>

#include "drone.h"		// Do not remove
struct timespec start, stop, stop1; 	// Do not remove
//clock_t start, stop;

double total_time;		// Do not remove
int gridsize;			// Do not remove

unsigned int drone_x, drone_y; 	//Coordinates of drone (to be found)

#define MAX_THREADS    4800

int thread_id[MAX_THREADS];	// User defined id for thread
pthread_t p_threads[MAX_THREADS];// Threads
pthread_attr_t attr;		// Thread attributes

unsigned int num_threads = MAX_THREADS;

unsigned int num_cells_ ;

void *get_grid_pos(void *s);
void *get_grid_pos_(void *s);
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
//

/*Algorithm for Multi-threaded execution :
 * The algorithm creates an optimal number of threads and uses the threads in 
 * the following manner:
 * 1. When the number of grids are lesser than or equal to the  optimal number of threads :
 *    In this case, a total number of threads equal to the number of cells are
 *    created. Individual threads are assigned to a indivdual grid to check if
 *    the drone is present in that grid. After confirming the presence of the drone,
 *    the program prints the grid name, output of check_drone_location and the 
 *    total execution time.
 *  2. When the number of grids are greater than the optimal number of threads :
 *     In this case, a total number of threads equal to the maximum number of available
 *     threads are created. Individual threads are assigned to grid locations 
 *     corresponding to their id's. If the drone exists in that location, the grid location
 *     is captured and the program is ended. If the drone does not exist in that specific 
 *     grid location, it moves on to the grid location which is equal to the sum of the 
 *     optimal number of threads and its id and repeats the above until a threead runs out
 *     of grid spaces.
 *The optimal number of threads to run on a machine at any given time depends on the machine
 *specifically and the best way to find out the optimal number of threads is to run the 
 *with a varied number of threads and find out the number of threads which gives the best 
 *performance.
 * */
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
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    unsigned int num_threads = MAX_THREADS;
    num_cells_ = gridsize*gridsize;

//    print_drone_path();

    clock_gettime(CLOCK_REALTIME, &start); 	// Do not remove
//start = clock();

   /* If number of cells is lesser than or equal to the number of threads */
    if(num_cells_<=num_threads)
    {
      for (int i = 0; i < num_cells_; i++)
      {
        thread_id[i] = i;
        pthread_create(&p_threads[i], &attr, get_grid_pos, (void *) &thread_id[i]);
      }
    }

    /* If number of cells is greater than the number of threads */
    else
    {
      unsigned int diff_bw_cells_threads = num_cells_-num_threads;
      unsigned int count_for_threads = num_cells_/num_threads;
      
      for (int i = 0; i < num_threads; i++)
      {
        thread_id[i] = i;
        pthread_create(&p_threads[i], &attr, get_grid_pos_, (void *) &thread_id[i]);
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

/* Function when number of grids lesser than or equal to the number of threads  */
void *get_grid_pos(void *s)
{
    int index = *((int *)s);
    unsigned int row=(int)(index/(get_gridsize()-1));
    unsigned int column = index%(get_gridsize()-1);
    if(check_grid(row,column) == 0)
    {
      printf("Success in Grid position = %d & %d ",row,column);
      
    clock_gettime(CLOCK_REALTIME, &stop1);
    total_time = (stop1.tv_sec-start.tv_sec)	+0.000000001*(stop1.tv_nsec-start.tv_nsec);
    printf("Drone = (%u,%u), success = %d, time (sec) = %8.4f\n", // Do not remove
    drone_x, drone_y, check_drone_location(drone_x,drone_y), total_time);// Do not remove
    
    pthread_detach(pthread_self());
    exit(0);
    }
    pthread_detach(pthread_self());

}


/* Function when number of grids are greater than the number of threads  */
void *get_grid_pos_(void *s)
{
  int index = *((int *)s);
  unsigned int row = (int)(index/(get_gridsize()-1));
  unsigned int column = index%(get_gridsize()-1);
  unsigned int temprow,tempcol;

  while(check_grid(row,column) != 0)
  {
    index += num_threads;
    if(index>num_cells_)
    {
      pthread_detach(pthread_self());
      return 0;
    }
    row = (int)(index/(get_gridsize()-1));
    column = index%(get_gridsize()-1);
  }

    temprow=row;tempcol=column;
    clock_gettime(CLOCK_REALTIME, &stop1);
    total_time = (stop1.tv_sec-start.tv_sec)	+0.000000001*(stop1.tv_nsec-start.tv_nsec);
    printf("Drone = (%u,%u), success = %d, time (sec) = %8.4f\n", // Do not remove
    temprow, tempcol, check_drone_location(drone_x,drone_y), total_time);// Do not remove
    
    pthread_detach(pthread_self());
    exit(0);

}

