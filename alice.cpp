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

int PORT = 8080;
int PORT2 = 8081;

using namespace std;

int myAtoi(char *str){
  int res = 0;
  for(int i=0; str[i]!='\0'; ++i)
    res = res*10 + str[i]-'0';
  return res;
}
int server(){
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1, file_size=0, remain_data = 0;
  int addrlen = sizeof(address);
  ssize_t len;
  char ch[100];
  char buffer[1024];

  FILE *received_file;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  &opt, sizeof(opt));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );

  bind(server_fd, (struct sockaddr *)&address, sizeof(address));

  listen(server_fd, 5);
  new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
  recv(new_socket, buffer, BUFSIZ, 0);
  cout<<buffer<<"..."<<endl;
  file_size =  myAtoi(buffer);
  //received_file = fopen("test.c", "w");
  remain_data = file_size;
  cout<<file_size<<endl;
  //while(((len = recv(new_socket, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0)){
    //fwrite(buffer, sizeof(char), len, received_file);
    //cout<<"Hello"<<endl;
    //remain_data -= len;
    //fprintf(stdout, "Receive %d bytes, waiting for %d\n", len, remain_data);
  //}
  cout<<"Hey";
  //fclose(received_file);
  close(new_socket);
    //cout<<"Received";//cout<<"Received";
    //valread = read( new_socket , buffer, 1024);
    //cout<<"Here:"<<endl<<buffer;
    //valread = read( new_socket , buffer, 1024);
    //cout<<"Here:"<<endl<<buffer;
  /*struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};
  sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
  connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  recv(sock, buffer, 100, 0);*/
  return 0;
}

int client(){
  while(1){
    char ha[] = "alice.cpp";
    cout<<">>";
    //cin>>ha;
    /*struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT2);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock , ha , strlen(ha) , 0 );*/

    cin>>ha;

    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT2);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    cout<< connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    //send(sock , ha , strlen(ha) , 0 );
    int LENGTH = 100;
    char sdbuf[LENGTH];
    //FILE *fs = fopen(ha, "r");
    int fs;
    fs = open(ha, O_RDONLY);
    bzero(sdbuf, LENGTH);
    int fs_block_sz;
    cout<<ha<<endl;
    /*while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0 ){
      cout<<fs_block_sz<<endl;
      send(sock, sdbuf, fs_block_sz, 0);
      cout<<"Sent"<<endl;
      bzero(sdbuf, LENGTH);
    }*/
    ssize_t len;
    int sent_bytes = 0;
    long int offset = 0;
    char file_size[256];
    //long int BUFSIZ = 100;
    struct stat file_stat;
    fstat(fs, &file_stat);
    sprintf(file_size, "%d", file_stat.st_size);
    int remain_data = file_stat.st_size;
    len = send(sock, file_size, sizeof(file_size), 0);
    while(((sent_bytes = sendfile(sock, fs, &offset, BUFSIZ)) > 0 )&& (remain_data >0)){
      remain_data -= sent_bytes;
    }
    close(sock);
    cout<< "Sent!";
  }
  return 0;
}

int main(int argc, char const *argv[]){
  //int sockfd = socket(domain, type, protocol);
  PORT = atoi(argv[1]);
  PORT2 = atoi(argv[2]);
  thread first(server);
  thread second(client);
  first.join();
  second.join();
  return 0;
}
