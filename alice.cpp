#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<thread>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<string.h>
#include<string>
int PORT = 8080;
int PORT2 = 8081;

using namespace std;

int server(){
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  &opt, sizeof(opt));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );

  bind(server_fd, (struct sockaddr *)&address, sizeof(address));
  char ch[100];
  while(1){
    /*listen(server_fd, 3);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    valread = read( new_socket , buffer, 1024);
    cout<<endl<<buffer;*/
  }
  return 0;
}

int client(){
  while(1){
    char ha[100];
    cout<<">>";
    /*cin>>ha;
    struct sockaddr_in address;
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
    //connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    //send(sock , ha , strlen(ha) , 0 );
    int LENGTH = 100;
    char sdbuf[LENGTH];
    FILE *fs = fopen(ha, "r");
    bzero(sdbuf, LENGTH);
    int fs_block_sz;
    while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0 ){
      send(sock, sdbuf, fs_block_sz, 0);
      bzero(sdbuf, LENGTH);
    }
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
