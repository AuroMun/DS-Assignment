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
#define LENGTH 100024

using namespace std;
int udp_client(){
      struct sockaddr_in si_me, si_other;
      int s, i;
      unsigned int slen=sizeof(si_other);
      char buf[BUFSIZ];

      if ((s=socket(AF_INET, SOCK_DGRAM, 0))==-1);

      memset(&si_me, 0, sizeof(si_me));
      si_me.sin_family = AF_INET;
      si_me.sin_port = htons(PORT2);
      si_me.sin_addr.s_addr = htonl(INADDR_ANY);
      printf("Waiting to receive!\n");
      if (bind(s, (struct sockaddr *) &si_me, sizeof(si_me))==-1){
        printf("Not bound\n");
      };
      printf("Kewl\n");
      for (i=0; i<10; i++) {
        printf("Hi\n");
        if (recvfrom(s, buf, BUFSIZ, 0, (struct sockaddr *) &si_other, &slen)==-1){
          printf("Wth\n");
        }
        else{
          printf("Yay\n");
        }
        printf("Received packet from %s:%d\nData: %s\n\n",
        inet_ntoa(si_me.sin_addr), ntohs(si_me.sin_port), buf);
      }

      close(s);
}

int udp_server(){
  struct sockaddr_in si_other;
  int s, i, slen=sizeof(si_other);
  char buf[BUFSIZ];
  char hello[100];
  cin>>hello;
  if ((s=socket(AF_INET, SOCK_DGRAM, 0))==-1);
  memset( &si_other, 0, sizeof(sockaddr_in));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT2);
  si_other.sin_addr.s_addr = INADDR_ANY;

  //inet_aton("127.0.0.1", &si_other.sin_addr);
  for(i=0; i<10; i++){
    printf("Sending packet %d\n", i);
    if(sendto(s, hello, strlen(hello), 0, (struct sockaddr *) &si_other, slen)<0){
      printf("Failed!");
    }
  }
}

int server(){

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[LENGTH] = {0};
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    char ch[LENGTH];
    char *fw;
    char *sname;
    listen(server_fd, 5);
    while((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))>0){
      //fprintf(stdout, "Accepting connection\n");
    valread = read( new_socket , buffer, 1024);
    fw = strtok(buffer, ":");
    char fname[] = "/home/doodledron/Documents/Academics/DS/new.txt";
    FILE *fr = fopen(fname, "w");
    //cout<<fw<<endl;

    if(!strstr(fw, "Sending")){
      buffer[strlen(buffer)] = '\0';
      fprintf(stdout, "\nMessage (Press enter to continue): %s\n>>", buffer);
      bzero(buffer, strlen(buffer));
    }
    else{
      sname = strtok(0, " ");
      fprintf(stdout, "Receiving file %s\n>>", sname);

      bzero(buffer, LENGTH);
      int fr_block_sz = 0;
      if(!fr){
        fprintf(stdout, "LOSER \n");
      }
      else cout<<fr<<endl;

      while(1)
      {
        fr_block_sz = recv(new_socket, buffer, LENGTH, 0);
        if(fr_block_sz > 0){
          int write_sz = fwrite(buffer, sizeof(char), fr_block_sz, fr);
          printf("%s\n", buffer);
          bzero(buffer, LENGTH);
        }
        else break;
      }
      fprintf(stdout, "Received file, wrote to %s \n", fname);
      fclose(fr);
      fprintf(stdout, "Closed file %s\n", fname);
    }
    close(new_socket);
    //fprintf(stdout, "Closed connection\n");
  }
  return 0;
}

int client(){
  if(accpt==0)return 0;
  while(1){
    char ha[LENGTH];
    cout<<">>";

    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *sname;
    char buffer[LENGTH] = {0};
    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT2);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    cin>>ha;
    fprintf(stdout, "Ha: %s\n", ha);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    send(sock , ha , strlen(ha) , 0 );
    int fs_block_sz;
    char *fw;
    fw = strtok(ha, ":");
    if(!strstr(fw, "Sending")){
    //if(1){
        //send(sock, ha, strlen(ha), 0);
        bzero(ha, strlen(ha));
        //fprintf(stdout, "Sent a message! %s\n ", ha);
        close(sock);
        //fprintf(stdout, "Closed connection\n");
    }
    else{
      sname = strtok(0, " ");
      fprintf(stdout, "File %s\n", sname);

      FILE *fp = fopen(sname, "r");
      bzero(buffer, LENGTH);

      while((fs_block_sz = fread(buffer, sizeof(char), LENGTH, fp)) > 0)
      {
        if(send(sock, buffer, fs_block_sz, 0) < 0);
        bzero(buffer, LENGTH);
      }
      fclose(fp);
    }
    close(sock);
  }
  return 0;
}

int main(int argc, char const *argv[]){
  //int sockfd = socket(domain, type, protocol);
  PORT = atoi(argv[1]);
  PORT2 = atoi(argv[2]);
  accpt = atoi(argv[3]);
  thread first(udp_server);
  thread second(udp_client);
  first.join();
  second.join();
  return 0;
}
