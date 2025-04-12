//
// Created by Arthur Efremenko on 4/11/25.
//

#include "server.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "dispatch.h"

int start_server(char *port)
{
  int sockfd, newsockfd, port_num, n;
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    error("Error opening the socket");
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));

  port_num = atoi(port);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port_num);

  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    error("Binding failed");
  }

  listen(sockfd, MAX_CONNECTIONS);
  clilen = sizeof(cli_addr);

  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

  if(newsockfd < 0) {
    error("Error while accepting a connection");
  }

  for(;;) {
    n = write(newsockfd, global_input_buffer, FRAMES_PER_BUFFER);
    if(n < 0) {
      error("Error while writing to client");
    }

    int i = strncmp("Bye", global_input_buffer, 3);
    if(i == 0) {
      break;
    }
  }

  close(newsockfd);
  close(sockfd);

  return 0;
}
