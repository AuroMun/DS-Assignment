/* Server code */
/* TODO : Modify to meet your need */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<thread>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string>
#include <sys/sendfile.h>


#define PORT_NUMBER     8000
#define SERVER_ADDRESS  "127.0.0.1"
//#define FILE_TO_SEND    "alice.cpp"

#define PORT_NUMBER     8000
#define SERVER_ADDRESS  "127.0.0.1"
//#define FILENAME        "bob.c"
int PORT = 8080;
int PORT2 = 8081;
char FILENAME[100] = "bob.c";
char FILE_TO_SEND[100] = "alice.cpp";

using namespace std;

int server(){
  int client_socket;
  ssize_t len;
  struct sockaddr_in remote_addr;
  char buffer[BUFSIZ];
  char buff[BUFSIZ];
  int file_size;
  FILE *received_file;
  int remain_data = 0;
  memset(&remote_addr, 0, sizeof(remote_addr));
  remote_addr.sin_family = AF_INET;
  inet_pton(AF_INET, SERVER_ADDRESS, &(remote_addr.sin_addr));
  remote_addr.sin_port = htons(PORT);
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  connect(client_socket, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr));
  recv(client_socket, buffer, BUFSIZ, 0);
  fprintf("Buffer size: %d", buffer, 0);
  //cout<<buffer<<endl;
  //recv(client_socket, buff, BUFSIZ, 0);
  //cout<<buff;
  file_size = atoi(buffer);
  received_file = fopen(FILENAME, "w");
  remain_data = file_size;
  while (((len = recv(client_socket, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0))
  {
          fwrite(buffer, sizeof(char), len, received_file);
          remain_data -= len;
          fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", len, remain_data);
  }
  fprintf(stdout, "Lalaland");
  fclose(received_file);
  close(client_socket);
  return 0;
}

int client(){
  fprintf(stdout, "I started!", 0);
  int server_socket;
  int peer_socket;
  socklen_t       sock_len;
  ssize_t len;
  struct sockaddr_in      server_addr;
  struct sockaddr_in      peer_addr;
  int fd;
  int sent_bytes = 0;
  char file_size[256];
  struct stat file_stat;
  long int offset;
  int remain_data;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  inet_pton(AF_INET, SERVER_ADDRESS, &(server_addr.sin_addr));
  server_addr.sin_port = htons(PORT2);
  bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
  listen(server_socket, 5);
  fd = open(FILE_TO_SEND, O_RDONLY);
  fstat(fd, &file_stat);
  fprintf(stdout, "File Size: \n%d bytes\n", file_stat.st_size);
  sock_len = sizeof(struct sockaddr_in);
  peer_socket = accept(server_socket, (struct sockaddr *)&peer_addr, &sock_len);
  fprintf(stdout, "Accept peer --> %s\n", inet_ntoa(peer_addr.sin_addr));
  sprintf(file_size, "%d", file_stat.st_size);
  len = send(peer_socket, file_size, sizeof(file_size), 0);
  //cout<<"Sending "<<FILE_TO_SEND<<" hahaha"<<endl;
  //send(peer_socket, FILE_TO_SEND, sizeof(FILE_TO_SEND), 0);
  fprintf(stdout, "Hey\n", 0);
  fprintf(stdout, "Server sent %d bytes for the size\n", len);
  offset = 0;
  remain_data = file_stat.st_size;
  while (((sent_bytes = sendfile(peer_socket, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
  {
          fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
          remain_data -= sent_bytes;
          fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
  }
  close(peer_socket);
  close(server_socket);
  return 0;
}

int main(int argc, char const *argv[]){
  PORT = atoi(argv[1]);
  PORT2 = atoi(argv[2]);
  //cin>>FILE_TO_SEND;
  //thread first(server);
  //thread second(client);
  //first.join();
  //second.join();
  server();
  return 0;
}
