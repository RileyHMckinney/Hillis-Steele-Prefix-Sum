#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/io_handling.h"

using namespace std;

/*creates shared memory array "arr", 
  reads 'n' integers from input_file into a shared memory array 'arr'
    - &shm_id - reference to the shared memory id
    - *&arr - the array pointer passed by reference
    - n - number of integers to be read
    - &input_file - the name of the input file
*/
void read_input(int &shm_id, int *&arr, int n, const string &input_file){
    // Allocate shared memory for the prefix sum array
    shm_id = shmget(IPC_PRIVATE, n * sizeof(int), IPC_CREAT | 0666);
    if (shm_id == -1) {
        cerr << "Error: Failed to allocate shared memory." << endl;
        exit(EXIT_FAILURE);
    }

    //Attach array to shared memory location
    arr = (int *)shmat(shm_id, NULL, 0);
    if (arr == (int *)-1) {
        cerr << "Error: Failed to attach array pointer to shared memory." << endl;
        exit(EXIT_FAILURE);
    }

    // Read input file into shared memory array
    ifstream infile(input_file);
    for (int i = 0; i < n; i++) {
        infile >> arr[i]; //automatically skips spaces, letters, symbols, etc
    }
    infile.close(); //close the input_file filestream
}

/*writes a shared memory array 'arr' into output_file
    - &output_file - the output filename
    - *arr - array pointer
    - n - number of values printed to the output file
*/
void write_output(const string &output_file, int *arr, int n){
    //establish output_file stream
    ofstream outfile(output_file);
    if (!outfile) { //verify output file stream established
        cerr << "Error: Cannot open output file: " << output_file << endl;
        exit(EXIT_FAILURE);
    }

    //print the values of the array into output_file
    outfile << "[";
    for(int i = 0; i < n - 1; i ++){
        outfile <<  arr[i] << ",";
    }
    outfile << arr[n-1] << "]" << endl;
    outfile.close();
}

/*destroys a shared memory array with memory id "shm_id" and address "shm_ptr"
    - shm_id - the shared memory id
    - shm-ptr - the pointer to the shared memory
*/
void destroy_shared_mem(int shm_id, int *shm_ptr){
    //detach shared mem from process
    if (shmdt(shm_ptr) == -1) {
        cerr << "Error: Failed to detach barrier shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    //delete shared memory once all processes are detached
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        cerr << "Error: Failed to free barrier shared memory" << endl;
        exit(EXIT_FAILURE);
    }
}