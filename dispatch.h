//
// Created by Arthur Efremenko on 4/11/25.
//


#ifndef DISPATCH_H
#define DISPATCH_H

#include <pthread.h>
#include "utils.h"

#define NUMTHRDS 4
pthread_t callThd[NUMTHRDS];
pthread_mutex_t mutexsum;

float global_input_buffer[FRAMES_PER_BUFFER];
float global_output_buffer[FRAMES_PER_BUFFER];

void update_global_buffer(float *in, float *out);

#endif //DISPATCH_H
