#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
int main (int ac, char **av)
{
	struct sockaddr_in sad;
 	int sid, i, j, n;
  	char sip_addr[32], msg[32], t;

  	if (ac == 1)
    		strcpy (sip_addr, "127.0.0.1");
  	else
    		strcpy (sip_addr, av[1]);
  	sid = socket (AF_INET, SOCK_STREAM, 0);
  	sad.sin_family = AF_INET;
  	sad.sin_addr.s_addr = inet_addr (sip_addr);
  	sad.sin_port = htons(1234);
  	connect (sid, (struct sockaddr *) &sad, sizeof (sad));
  
	while(1)
    	{
        	char new_msg[64];
        	printf("Enter ip or type 'exit' to exit: ");
        	fgets(msg,32,stdin);
        	write(sid,(void*)msg,strlen(msg)+1);
        	if(strncmp(msg, "exit", 4) == 0) break;
        	int n = read(sid,(void*)new_msg,sizeof(new_msg));
        	new_msg[n]='\0';
        	printf("IS IP VALID?: %s\n",new_msg);
    	}
    	puts("Exiting...");
    	close(sid);
  	return 0;
}
