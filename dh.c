/* A simple client program for server.c

   To compile: gcc client.c -o client

   To run: start the server, then the client */
#include <stdbool.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <netdb.h>

#include <unistd.h>

#include <signal.h>

#include <stdio.h>

#include <string.h>

#include <fcntl.h>

#include <errno.h>

#include <sys/time.h>

#include <stdlib.h>

#include <memory.h>

#include <ifaddrs.h>

#include <net/if.h>

#include <stdarg.h>

// https://www.techiedelight.com/c-program-demonstrate-diffie-hellman-algorithm/

int compute(int g, int b, int p) {
  int r;
  int y = 1;

  while (b > 0) {
    r = b % 2;

    // fgst exponention
    if (r == 1)
      y = (y * g) % p;
    g = g * g % p;

    b = b / 2;
  }

  return y;
}

int main(int argc, char ** argv) {
  int sockfd, portno, n, b, f;
  struct sockaddr_in serv_addr;
  struct hostent * server;
  portno = 7800;
  char buffer[256];

  b = atoi(argv[1]);

  bzero((char * ) & serv_addr, sizeof(serv_addr));
  /* Translate host name into peer's IP address ;
   * This is name translation service by the operating system
   */
  server = gethostbyname("172.26.37.44");

  if (server == NULL) {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }

  /* Building data structures for socket */

  bzero((char * ) & serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;

  bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

  serv_addr.sin_port = htons(portno);

  /* Create TCP socket -- active open
   * Preliminary steps: Setup: creation of active open socket
   */

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(0);
  } else {
    printf("Socket connection successful\n");
  }

  if (connect(sockfd, (struct sockaddr * ) & serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR");
    exit(0);
  }

  bool protocol = true;
  char * message_phase[] = {
    "send_username",
    "send_gb_mod_p",
    "send_ga_mod_p",
    "success",
    "end"
  };
  char * current_phase = "send_username";

  printf("Send Usename");
  char * username = "achattaraj\n";
  bzero(buffer, 256);
  sprintf(buffer, username);
  n = write(sockfd, buffer, strlen(buffer));
  current_phase = message_phase[1];

  printf("send gb mod p \n");
  bzero(buffer, 256);
  b = compute(15, atoi(argv[1]), 97);
  sprintf(buffer, "%d\n", b);
  n = write(sockfd, buffer, strlen(buffer));
  current_phase = message_phase[2];

  printf("send ga mod p \n");
  bzero(buffer, 256);
  n = read(sockfd, buffer, 255);
  int a = atoi(buffer);
  bzero(buffer, 256);
  a = compute(a, atoi(argv[1]), 97);
  sprintf(buffer, "%d\n", a);
  n = write(sockfd, buffer, strlen(buffer));

  n = 0;
  while (strstr(buffer, "Success") == NULL) {
    n = read(sockfd, buffer, 255);

    current_phase = "end";
    buffer[n] = 0;
    printf("%s\n", buffer);

    close(sockfd);
    return 0;

  }
}
