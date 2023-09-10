#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

int main( int argc , char* argv[])
{
    char ip[32] , data[64] , code[129];
    strcpy(ip,( argc == 1 ? "127.0.0.1" : argv[1]) );
    int cs = socket(AF_INET, SOCK_DGRAM , 0);

    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
	saddr.sin_port=htons(1234);
	inet_aton(ip,&(saddr.sin_addr));

    while(1){
        printf("\nEnter data('exit' to exit): ");
        fgets(data,sizeof(data),stdin);
        data[strlen(data) - 1]='\0';
        sendto(cs,(void*)data,strlen(data)+1,0,(struct sockaddr*)&saddr,sizeof(saddr));
        if(strcasecmp(data,"exit")==0) break;
		usleep(10000);
		int len=sizeof(saddr);
		recvfrom(cs,(void*)code,sizeof(code),0,(struct sockaddr*)&saddr,&len);
		printf("Received Codeword: %s\n",code);
    }
    puts("Exiting...");
    close(cs);
    return 0;
}
