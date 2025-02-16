#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>  
#include <cstdlib>
#include "../include/barrier.h"

using namespace std;

//create an array of size m in shared memory
void wall_create(int &shm_wall_id, int *&wall, int m){
    //allocate shared memory for wall[]
    shm_wall_id = shmget(IPC_PRIVATE, m * sizeof(int), IPC_CREAT | 0666);
    if (shm_wall_id == -1) {
        cerr << "Error: Failed to allocate shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    //attach shared memory to the process address space
    wall = (int *)shmat(shm_wall_id, NULL, 0);
    if(wall == (int *)-1){
        cerr << "Error: Failed to attach barrier shared memory." << endl;
        exit(EXIT_FAILURE);
    }

    //initialize wall[] to '0's
    for(int i = 0; i < m; i++){
        wall[i] = 0;
    }
}

//increment wall counter and wait for other processes
void wall_use(int *wall, int i, int m){
    wall[i] += 1; //increment wall at i

    bool done = false; //assume not done
    while(!done){   //if not done
        done = true;    //assume done
        //if a process is not done, done = false
        for(int j = 0; j < m; j++){ 
            if(wall[j] < wall[i]){
                done = false; 
                break;
            }
        }
        //sleep
        if(!done) sleep(1);
    }
}