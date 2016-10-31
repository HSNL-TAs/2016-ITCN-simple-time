/*
 * Simple Time Client For 2016 Introduction to Computer Network
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

#define MAX_SIZE 2048
#define PORT 8888

int main (int argc , char **argv) {
  int cli_fd;                   // descriptor of client, used by `socket()`
  struct sockaddr_in svr_addr;  // address of server, used by `connect()`

  int read_bytes;               // number of bytes, return by `read()`
  char buf[MAX_SIZE];           // buffer to store msg

  /* 1) Create the socket, use `socket()`
        AF_INET: IPv4
        SOCK_STREAM: Stream Socket (TCP)
  */
  cli_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (cli_fd < 0) {
    perror("Create socket failed.");
    exit(1);
  }

  /* 2) Connect to server with prepared sockaddr_in structure */
  bzero(&svr_addr, sizeof(svr_addr));
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  svr_addr.sin_port = htons(PORT);

  if (connect(cli_fd, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0) {
    perror("Connect failed");
    exit(1);
  }
  printf("Server connected\n");

  /* Handle message, read current from the server */
  read_bytes = read(cli_fd, buf, sizeof(buf));
  if (read_bytes < 0) {
    perror("Read failed");
    exit(1);
  }
  buf[read_bytes] = '\0';
  printf("Server datetimes: %s\n", buf);
  close(cli_fd);

  return 0;
}
