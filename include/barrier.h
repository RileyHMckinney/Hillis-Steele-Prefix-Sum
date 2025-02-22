#ifndef BARRIER_H
#define BARRIER_H

void shm_arr_create(int &shm_id, int *&arr, int size);
void wall_use(int *barrier_vars, int process_number, int m);

#endif // BARRIER_H