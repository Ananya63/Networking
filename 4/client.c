#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int cs = socket(AF_UNIX, SOCK_STREAM , 0);
	struct sockaddr_un saddr;
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path,"mysocket");
    int success = connect(cs,(struct sockaddr*)&saddr,sizeof(saddr));
    assert(success == 0);
    char data[100], divisor[32],crc[32];
    while(1)
    {
        printf("Enter data or type 'exit' to exit: ");
        scanf("%s",data);
        printf("Sending data\n");
        write(cs,(void*)data,strlen(data)+1);
        if(strncmp(data, "exit", 4) == 0) {
            puts("Exiting...");
            exit(0);
        }
        printf("Enter the divisor: ");
        scanf("%s",divisor);
        printf("Sending divisor\n");
        write(cs,(void*)divisor,strlen(divisor)+1);
        printf("Waiting for data from server\n");
        read(cs,(void*)crc,sizeof(crc));
        printf("The crc: %s \n",crc);
        strcat(data,crc);
        printf("The Codeword: %s \n\n",data);
    }
    close(cs);
    return 0;
}

