#ifndef BARRIER_H
#define BARRIER_H

void wall_create(int &shm_wall_id, int *&wall, int m);
void wall_use(int *wall, int i, int m);

#endif // BARRIER_H