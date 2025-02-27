/*Name: Riley McKinney, Date: 02/26/2025

The purpose of this program is to use parallel processing to perform the Hillis and Steele
concurrent algorithm on an array of integers in an input file and print the resultant 
array to an output file. 

IMPORTANT: The maximum number of integers 'n' is defined in validate.h as "MAX_N" and is set 
to 10,000,000 by default.

Purpose of each file:
- main.cpp - driving program
- barrier.cpp - functions for creating an maintaining a barrier, ensures that each process
    is on the same "round" or "cycle" during the Hillis and Steele algorithm
- io-handling.cpp - creates shared memory array for the input file, prints an array to 
    an output file, and destroys shared memory arrays
- validate.cpp - validates the user input parameters
*/

#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <string>
#include <sstream>
#include <sys/types.h> 
#include <sys/wait.h>  
#include <unistd.h>     
#include <cmath>
#include "../include/validate.h"
#include "../include/barrier.h"
#include "../include/io_handling.h"

using namespace std;

int main(int argc, char *argv[]){
    int n, m;
    string input_file, output_file;

    //validate command line arguments
    m = validate_arguments(argc, argv, n, m, input_file, output_file);

    //create shared memory for the barrier
    int barrier_size = 2;
    int shm_barrier_id;
    int *barrier_vars;
    shm_arr_create(shm_barrier_id, barrier_vars, barrier_size);

    //create shared memory array for the temp array (array of size n)
    //  This array will be used during the algo to avoid overwriting needed values in arr
    int temparr_size = n;
    int shm_temparr_id;
    int *temparr_vars;
    shm_arr_create(shm_temparr_id, temparr_vars, temparr_size);

    //read input_file into shared memory
    int shm_arr_id;
    int *arr;
    read_input(shm_arr_id, arr, n, input_file);

    //calculate the size of each chunk/remainder
    int chunk = n/m;
    int remainder = n%m;

    pid_t pid[m];
    for(int i = 0; i < m; i++){
        pid[i] = fork(); //create m child processes

        if(pid[i] < 0){
            cerr << "Error: Failed to fork." << endl;
            exit(EXIT_FAILURE);
        } else if (pid[i] == 0) {//child process
            int start = i * chunk;
            int end;

            //calculate start and end indices 
            if (i < remainder) {
                start += i;  // Add offset for first 'remainder' processes
                end = start + chunk + 1;
            } else {
                start += remainder; // Offset by remainder
                end = start + chunk;
            }

            // Perform Hillis and Steele’s prefix sum algorithm
            //  Algorithm runs log2(n) + 1 times to account for non-power of two values
            for (int p = 1; p <= log2(n) + 1; p++) { 
                int offset = pow(2, p - 1); // Compute 2^(p-1)
            
                //Adding correctly spaced values from previous array to temparr
                for (int j = start; j < end; j++) {
                    if (j >= offset) {
                        temparr_vars[j] = arr[j] + arr[j - offset]; 
                    } else {
                        temparr_vars[j] = arr[j];
                    }
                }                
            
                //1st Barrier Call: ensures temparr is fully populated
                wall_use(barrier_vars, i, m);
            
                //final process copies temparr back into arr
                if (i == m - 1) {
                    for (int j = 0; j < n; j++) {
                        arr[j] = temparr_vars[j];
                    }
                }
            
                //2nd Barrier Call: ensures all processes will read updated arr
                wall_use(barrier_vars, i, m);
            }
            exit(0); // Ensure process exits
            
        }

    }

    //parent waits for all child processes
    for(int i = 0; i < m; i++){
        if (waitpid(pid[i], NULL, 0) == -1) {
            perror("Error: waitpid failed");
            exit(EXIT_FAILURE);
        }
    }

    //write finished array 'arr' to output_file
    write_output(output_file, arr, n);

    //free the shared memory for the barrier and the array
    destroy_shared_mem(shm_barrier_id, barrier_vars);
    destroy_shared_mem(shm_arr_id, arr);
    
    return 0;
}