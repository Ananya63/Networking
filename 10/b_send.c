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
    int sock,t=1;                        
    struct sockaddr_in broadcastAddr; 
    char *broadcastIP;               
    char sendString[256];                 
    int broadcastPermission;        
    unsigned int sendStringLen;      

    if (argc !=2)                     
    {
       printf("Give Broadcast IP as argument please!");
       exit(1);
    }
    else
       broadcastIP = argv[1];
   
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        DieWithError("socket() failed");

    broadcastPermission = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0)
        DieWithError("setsockopt() failed");

    broadcastAddr.sin_family = AF_INET;                 
    broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP);
    broadcastAddr.sin_port = htons(broadcastPort);         

    while(t) 
    {
        printf("\n\t\tEnter the messege you want to send: ");
	    fgets(sendString,sizeof(sendString),stdin);    
        sendStringLen = strlen(sendString);  
         
        if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen)
           DieWithError("sendto() sent a different number of bytes than expected");
        if(!strcmp(sendString, "QUIT\n"))
	    {
	       t=0;
	    }
    }
    close(sock);
    return(0);
}
