//
// Created by Arthur Efremenko on 4/11/25.
//

#include "client.h"
#include "utils.h"
#include "dispatch.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int start_client(char *host, char *port)
{
  int sockfd, port_num, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  port_num = atoi(port);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd < 0) {
    error("Error occurred when opening the socket");
  }

  server = gethostbyname(host);
  if(server == NULL) {
    fprintf(stderr, "Error, no host found for %s", host);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  bcopy((char *) server->h_addr,
    (char *) &serv_addr.sin_addr.s_addr,
    server -> h_length);

  serv_addr.sin_port = htons(port_num);

  if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    error("Error during connection to the server");
  }

  printf("Client: ");

  for(;;) {
    n = read(sockfd, global_output_buffer, FRAMES_PER_BUFFER);
    if (n < 0) {
      error("Error reading from the socket");
    }
    printf("Server sent buffer with %lu bytes\n", sizeof(global_input_buffer));

    int i = strncmp("Bye", global_output_buffer, 3);
    if (i == 0) {
      break;
    }
  }

  close(sockfd);
  return 0;
}