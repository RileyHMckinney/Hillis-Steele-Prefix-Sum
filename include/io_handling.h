#ifndef READ_INPUT_H
#define READ_INPUT_H

#include <string>

void read_input(int &shm_id, int *&arr, int n, const std::string &input_file);
void write_output(const std::string &output_file, int *arr, int n);
void destroy_shared_mem(int shm_id, int *shm_ptr);

#endif
