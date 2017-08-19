#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<thread>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<cstdlib>
#include<string.h>
#include<string>
int PORT = 8080;
int PORT2 = 8081;
int accpt = 0;
#define LENGTH 100024

using namespace std;

void printProgBar( int percent );

int get_file_size(std::string filename) // path to file
{
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    fclose(p_file);
    return size;
}
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
      if (bind(s, (struct sockaddr *) &si_me, sizeof(si_me))==-1){
        printf("Not bound\n");
      };

      if (recvfrom(s, buf, BUFSIZ, 0, (struct sockaddr *) &si_other, &slen)==-1);
      printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(si_me.sin_addr), ntohs(si_me.sin_port), buf);

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
  si_other.sin_port = htons(PORT);
  si_other.sin_addr.s_addr = INADDR_ANY;

  //inet_aton("127.0.0.1", &si_other.sin_addr);
  printf("Sending packet with message %s\n", hello);
  if(sendto(s, hello, strlen(hello), 0, (struct sockaddr *) &si_other, slen)<0);

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
    char *sname, *tname, *foname;
    listen(server_fd, 5);
    while((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))>0){
      //fprintf(stdout, "Accepting connection\n");
    valread = read( new_socket , buffer, 1024);
    fw = strtok(buffer, ":");
    char fname[] = "new.txt";
    //cout<<fw<<endl;

    if(!strstr(fw, "Sending")){
      buffer[strlen(buffer)] = '\0';
      fprintf(stdout, "\nMessage (Press enter to continue): %s\n>>", buffer);
      bzero(buffer, strlen(buffer));
    }
    else{
      sname = strtok(0, ":");
      FILE *fr = fopen(fname, "w");
      fprintf(stdout, "Receiving file %s\n>>", sname);
      int fsz, fsz_copy;
      tname = strtok(0, ":");
      foname = strtok(0, ":");
      fsz = atoi(foname);
      fsz_copy = fsz;
      if(strstr(tname, "TCP")){
        bzero(buffer, LENGTH);
        int fr_block_sz = 0;

        while(1)
        {
          fr_block_sz = recv(new_socket, buffer, LENGTH, 0);
          if(fsz > 0){
            int write_sz = fwrite(buffer, sizeof(char), fr_block_sz, fr);
            //printf("%s\n", buffer);
            fsz -= fr_block_sz;
            if(fsz<0)fsz=0;
            int pN = (fsz_copy-fsz)*100.0/fsz_copy;
            //printf("%d %d %d %d\n", fsz_copy, fsz, fsz_copy-fsz, pN);
            printProgBar(pN);
            bzero(buffer, LENGTH);
          }
          else break;
        }
        fprintf(stdout, "\nReceived file, wrote to %s \n>>", fname);
        fclose(fr);
        close(new_socket);
      }
      else{
        close(new_socket);
        int fr_block_sz = 0;
        struct sockaddr_in si_me, si_other;
        int s, i;
        unsigned int slen=sizeof(si_other);
        char buf[BUFSIZ];

        if ((s=socket(AF_INET, SOCK_DGRAM, 0))==-1);

        memset(&si_me, 0, sizeof(si_me));
        si_me.sin_family = AF_INET;
        si_me.sin_port = htons(PORT2);
        si_me.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(s, (struct sockaddr *) &si_me, sizeof(si_me))==-1){
          printf("Not bound\n");
        };
        bzero(buf, BUFSIZ);
        printf("Made new socket to receive:\n");
        //if (recvfrom(s, buf, BUFSIZ, 0, (struct sockaddr *) &si_other, &slen)==-1);
        while(1)
        {
          fr_block_sz = recvfrom(s, buf, BUFSIZ, 0, (struct sockaddr *) &si_other, &slen);
          fsz -= fr_block_sz;
          if(fsz<0)fsz=0;
          int pN = (fsz_copy-fsz)*100.0/fsz_copy;
          //printf("%d %d %d %d\n", fsz_copy, fsz, fsz_copy-fsz, pN);
          printProgBar(pN);
          fwrite(buf, sizeof(char), fr_block_sz, fr);
          if(fsz > 0){
            bzero(buf, BUFSIZ);
          }
          else break;

        }
        fclose(fr);
        fprintf(stdout, "\nDone. Print Enter to Continue\n>>");
        //printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(si_me.sin_addr), ntohs(si_me.sin_port), buf);

        close(s);
      }
    }
    //fprintf(stdout, "Closed connection\n");
  }
  return 0;
}

int client(){
  if(accpt==0)return 0;
  while(1){
    char ha[LENGTH], cop[LENGTH];
    cout<<">>";
    cin>>ha;

    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *sname, *tname;
    char buffer[LENGTH] = {0};
    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT2);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    fprintf(stdout, "Ha: %s\n", ha);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));


    int fs_block_sz;
    char *fw;
    strcpy(cop, ha);
    fw = strtok(ha, ":");
    if(!strstr(fw, "Sending")){
        send(sock , ha , strlen(ha) , 0 );
        bzero(ha, strlen(ha));
        close(sock);
    }
    else{
      sname = strtok(0, ":");
      fprintf(stdout, "File: %s\n", sname);
      tname = strtok(0, ":");
      fprintf(stdout, "Mode: %s\n", tname);
      int sz = get_file_size(sname);
      strcat(cop, ":");
      char z[100];
      sprintf(z, "%d", sz);
      strcat(cop, z);
      printf("%s\n", cop);
      send(sock , cop , strlen(cop) , 0 );
      if(strstr(tname, "TCP")){
        FILE *fp = fopen(sname, "r");
        bzero(buffer, LENGTH);
        while((fs_block_sz = fread(buffer, sizeof(char), LENGTH, fp)) > 0)
        {
          if(send(sock, buffer, fs_block_sz, 0) < 0);
          bzero(buffer, LENGTH);
        }
        fclose(fp);
        close(sock);
      }
      else{
        close(sock);
        sleep(1);
        struct sockaddr_in si_other;
        int s, i, slen=sizeof(si_other);
        char buf[BUFSIZ];
        if ((s=socket(AF_INET, SOCK_DGRAM, 0))==-1);
        memset( &si_other, 0, sizeof(sockaddr_in));
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(PORT);
        si_other.sin_addr.s_addr = INADDR_ANY;

        //printf("Sending packet with message %s\n", hello);
        //if(sendto(s, hello, strlen(hello), 0, (struct sockaddr *) &si_other, slen)<0);
        //cin>>ha;
        FILE *fp = fopen(sname, "r");
        bzero(buf, BUFSIZ);
        while((fs_block_sz = fread(buf, sizeof(char), BUFSIZ, fp)) > 0)
        {
          if(sendto(s, buf, BUFSIZ, 0, (struct sockaddr *) &si_other, slen)<0);
          //printf("Sending\n");
          usleep(1000);
          bzero(buf, BUFSIZ);
        }
        fclose(fp);
        printf("Ooh Sent!\n");
      }
    }
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

void printProgBar( int percent ) {
 string bar;

 for(int i = 0; i < 50; i++){
   if( i < (percent/2)){
     bar.replace(i,1,"=");
   }else if( i == (percent/2)){
     bar.replace(i,1,">");
   }else{
     bar.replace(i,1," ");
   }
 }

 cout<< "\r" "[" << bar << "] ";
 cout.width( 3 );
 cout<< percent << "%     " << std::flush;
}
