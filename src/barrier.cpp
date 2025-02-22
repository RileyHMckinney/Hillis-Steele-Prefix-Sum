#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>  
#include <cstdlib>
#include "../include/barrier.h"

using namespace std;

// Create shared memory for barrier control variables (also used to create a temp array)
void shm_arr_create(int &shm_id, int *&arr, int size) {
    // Allocate shared memory for an array of `size` integers
    shm_id = shmget(IPC_PRIVATE, size * sizeof(int), IPC_CREAT | 0666);
    if (shm_id == -1) {
        cerr << "Error: Failed to allocate shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory
    arr = (int *)shmat(shm_id, NULL, 0);
    if (arr == (int *)-1) {
        cerr << "Error: Failed to attach shared memory." << endl;
        exit(EXIT_FAILURE);
    }

    // Initialize the shared memory to zero
    memset(arr, 0, size * sizeof(int));
}

// Function to enforce the barrier
void wall_use(int *barrier_vars, int process_number, int m) {
    while (true) {
        if (barrier_vars[0] == process_number) { // Check if it's this process's turn
            barrier_vars[1] += 1; // Increment barrier count
            barrier_vars[0] += 1; // Move to the next process
            break; // Exit once updated
        }
        sleep(1); // Wait if not its turn
    }

    // Wait for all processes to reach the barrier
    while (barrier_vars[1] < m) {
        sleep(1);
    }
}
