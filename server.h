//
// Created by Arthur Efremenko on 4/11/25.
//

#ifndef SERVER_H
#define SERVER_H

#define MAX_CONNECTIONS 2

#include <stdlib.h>

int start_server(char *port, char *callback_buffer, size_t buffer_size);

#endif //SERVER_H
