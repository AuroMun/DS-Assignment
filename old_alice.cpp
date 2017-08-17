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
int accpt = 0;


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
    listen(server_fd, 1);
    fprintf(stdout, "Waiting for connection\n");
    while((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))>0){
    fprintf(stdout, "Accepted connection on port: %d\n", PORT);
    valread = read( new_socket , buffer, 1024);
    fprintf(stdout, "%s\n>>", buffer);
    close(new_socket);
  }
  return 0;
}

int client(){
  if(accpt==0)return 0;
  while(1){
    char ha[100];
    cout<<">>";
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
    fprintf(stdout, "Beginning to connect\n");
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock , ha , strlen(ha) , 0 );
    fprintf(stdout, " Sent!\n");
    close(sock);
  }
  return 0;
}

int main(int argc, char const *argv[]){
  //int sockfd = socket(domain, type, protocol);
  PORT = atoi(argv[1]);
  PORT2 = atoi(argv[2]);
  accpt = atoi(argv[3]);
  thread first(server);
  thread second(client);
  first.join();
  second.join();
  return 0;
}
