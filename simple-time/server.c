/*
 * Simple Time Server For 2016 Introduction to Computer Network
 * Author: vicky-sunshine @ HSNL-TAs
 * 2016/09
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     /* provide functions about string, ex: `bzero()`, `strlen()`*/
#include <sys/types.h>  /* contains a number of basic derived types */
#include <sys/socket.h> /* provide functions and data structures of socket */
#include <arpa/inet.h>  /* convert internet address and dotted-decimal notation */
#include <netinet/in.h> /* provide constants and structures needed for internet domain addresses*/
#include <unistd.h>     /* `read()` and `write()` functions */
#include <time.h>       /* provide functions about time */

#define PORT 8888
#define MAX_SIZE 2048
#define MAX_CONNECTION 5


int main() {

  int svr_fd;                   // socket file descriptor, return by `socket()`
  struct sockaddr_in svr_addr;  // address of server, used by `bind()`

  int cli_fd;                   // descriptor of incomming client, return by `accept()`
  struct sockaddr_in cli_addr;  // address of client, used by `accept()`
  socklen_t addr_len;           // size of address, used by `accept()`

  time_t ticks;                 // store current time
  int write_bytes;              // number of byte, return by `write()`
  char buf[MAX_SIZE];           // buffer to store msg


  /* 1) Create the socket, use `socket()`
        AF_INET: IPv4
        SOCK_STREAM: Stream Socket (TCP)
  */
  svr_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (svr_fd < 0) {
    perror("Create socket failed.");
    exit(1);
  }

  /* 2) Bind the socket to port, with prepared sockaddr_in structure */
  bzero(&svr_addr, sizeof(svr_addr));
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  svr_addr.sin_port = htons(PORT);

  if (bind(svr_fd, (struct sockaddr*)&svr_addr , sizeof(svr_addr)) < 0) {
    perror("Bind socket failed.");
    exit(1);
  }

  /* 3) Listen on socket */
  if (listen(svr_fd, MAX_CONNECTION) < 0) {
    perror("Listen socket failed.");
    exit(1);
  }

  printf("Server started\n");
  printf("Maximum connections set to %d\n", MAX_CONNECTION);
  printf("Listening on %s:%d\n", inet_ntoa(svr_addr.sin_addr), PORT);
  printf("Waiting for client...\n\n");

  addr_len = sizeof(struct sockaddr_in);

  while(1) {
    /* 4) Accept client connections */
    cli_fd = accept(svr_fd, (struct sockaddr*)&cli_addr, (socklen_t*)&addr_len);

    if (cli_fd < 0) {
      perror("Accept failed");
      exit(1);
    }

    printf("Connection accepted\n");
    printf("Client is from %s:%d\n\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

    /* Handle message, write current time to client */
    ticks = time(NULL);
    snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));

    write_bytes = write(cli_fd, buf, strlen(buf));
    if(write_bytes < 0) {
      perror("Write Failed");
      exit(1);
    }

    close(cli_fd);
  }

  close(svr_fd);
  return 0;
}
