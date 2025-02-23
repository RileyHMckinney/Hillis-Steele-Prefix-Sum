#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>  //for usleep
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
#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>  // for usleep
#include <cstdlib>
#include "../include/barrier.h"

using namespace std;

void wall_use(int *barrier_vars, int process_number, int m) {
    if (m == 1) return; // No need for a barrier if there's only one process

    int *barrier = &barrier_vars[0];      // Shared barrier counter
    int *cycle_count = &barrier_vars[1];  // Shared cycle counter

    // Step 1: Read the current cycle count before entering the barrier
    int local_cycle = *cycle_count;

    // Step 2: Wait until it's this process's turn to increment the barrier
    while (*barrier != process_number) {  
        usleep(100);  // Reduce CPU load while waiting
    }

    // Step 3: Increment the barrier count safely
    (*barrier)++;
    cout << "Process " << process_number << " passed barrier. Barrier count: " << *barrier << endl;

    // Step 4: The last process resets the barrier and increments cycle_count
    if (process_number == m - 1) {
        cout << "Process " << process_number << " resetting barrier for next round." << endl;
        *barrier = 0;  // Reset for the next iteration
        (*cycle_count)++;  // Increment cycle count
    }

    // Step 5: Wait until `cycle_count` changes, signaling the next iteration
    while (*cycle_count == local_cycle) {
        usleep(1000);  // Prevents excessive busy-waiting
    }

    cout << "Process " << process_number << " detected cycle count increment. Proceeding to next iteration." << endl;
}
