#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <string>
#include <sstream>
#include "../include/validate.h"
#include "../include/barrier.h"
#include "../include/io_handling.h"

using namespace std;

int main(int argc, char *argv[]){
    int n, m;
    string input_file, output_file;

    //validate command line arguments
    validate_arguments(argc, argv, n, m, input_file, output_file);

    //create shared memory for the barrier
    int shm_wall_id;
    int *wall;
    wall_create(shm_wall_id, wall, m);

    //read input_file into shared memory
    int shm_arr_id;
    int *arr;
    read_input(shm_arr_id, arr, n, input_file);

    //create m child processes

    //write arr to output_file
    write_output(output_file, arr, n);

    //free the shared memory for the barrier and the array
    destroy_shared_mem(shm_wall_id, wall);
    destroy_shared_mem(shm_arr_id, arr);
    
    return 0;
}