#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int n = 5;
	int ss = socket(AF_UNIX, SOCK_STREAM , 0);
	struct sockaddr_un saddr, caddr;
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path,"mysocket");
    unlink("mysocket");
	int success = bind (ss , (struct sockaddr*)&saddr, sizeof(saddr));
    assert(success == 0);
	listen(ss,n);
	socklen_t len = sizeof(caddr);
	int ts = accept(ss, (struct sockaddr*)&caddr, &len);
	char bit[2] = "0";
	char data[32] = {0};
	while(1)
	{
		printf("\nWaiting for data from client\n");
		read(ts,(void*)data,sizeof(data));
        if(strncmp(data, "exit", 4) == 0) {
            unlink("mysocket");
            puts("Exiting...");
            exit(1);
        }
		printf("Recieved data : %s\n",data);
		int count=0,i;
		for(i = 0; i < strlen(data); i++) {
			if(data[i]=='1')
				count++;
        }
        
        bit[0] = '0' + (count % 2);
		strncat(data,bit,1);
		printf("Sending modified data\n");
		write(ts,(void*)data,strlen(data)+1);
	}
	close(ts);
	close(ss);
	return 0;
}
