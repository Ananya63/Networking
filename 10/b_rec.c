#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h>     

#define broadcastPort 4321

void DieWithError(char errorMessage[])
{
  printf("%s\n",errorMessage);
} 

int main(int argc, char *argv[])
{
    int sock;                         
    struct sockaddr_in broadcastAddr;
    char recvString[256]; 
    int recvStringLen;                

    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        DieWithError("socket() failed");
  
    broadcastAddr.sin_family = AF_INET;                
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    broadcastAddr.sin_port = htons(broadcastPort);      
    int reuse=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    
    if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0)
        DieWithError("bind() failed");
    while(1)
    {
      if ((recvStringLen = recvfrom(sock, recvString, sizeof(recvString), 0, NULL, 0)) < 0)
         DieWithError("recvfrom() failed");
    
      recvString[recvStringLen] = '\0';
      if(!strcmp(recvString, "QUIT\n"))
         break;
      printf("Received: %s\n", recvString);   
    }
    close(sock);
    exit(0);
}
