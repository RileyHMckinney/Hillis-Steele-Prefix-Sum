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
    validate_arguments(argc, argv, n, m, input_file, output_file);

    //create shared memory for the barrier (array of size 2)
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
            if (i < remainder) {
                start += i;  // Add offset for first 'remainder' processes
                end = start + chunk + 1;
            } else {
                start += remainder; // Offset by remainder
                end = start + chunk;
            }

            exit(0); //exit child process
        }

    }

    //parent waits for all child processes
    for(int i = 0; i < m; i++){
        waitpid(pid[i], NULL, 0);
    }

    //write arr to output_file
    write_output(output_file, arr, n);

    //free the shared memory for the barrier and the array
    destroy_shared_mem(shm_barrier_id, barrier_vars);
    destroy_shared_mem(shm_arr_id, arr);
    
    return 0;
}