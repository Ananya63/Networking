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
    char data[32] = {0};
    while(1)
    {
        printf("Enter a bit stream or type 'exit' to exit: ");
        scanf("%s",data);
        printf("Sending data\n");
        write(cs,(void*)data,strlen(data)+1);
        if(strncmp(data, "exit", 4) == 0) {
            puts("Exiting...");
            exit(0);
        }
        printf("Waiting for data from server\n");
        read(cs,(void*)data,sizeof(data));
        printf("Modified bit stream with added parity bit: %s \n\n",data);
    }
    close(cs);
    return 0;
}
