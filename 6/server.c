#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#define N 3


void* serve_client(void* args)
{
	int ss=*((int*)args);
	struct sockaddr_in caddr;
	int len=sizeof(caddr);
	int ts=accept(ss,(struct sockaddr*)&caddr,&len);
	char data[32],res[3];
	while(1)
	{
		printf("\nWaiting for data from client!\n");
		read(ts,(void*)data,sizeof(data));
       	        data[strcspn(data, "\n")] = '\0';
		if(strcasecmp(data,"exit")==0)
		{
			printf("\nExiting...\n");
			break;
		}
		printf("\nRecieved data:%s",data);
		strcpy(res,(inet_pton(AF_INET,data,data)>0 ? "YES" : "NO" ));
		write(ts,(void*)res,strlen(res)+1);
	}
	close(ts);
	pthread_exit(NULL);
}

int main()
{
	pthread_t th[N];
	int i=0;
	struct sockaddr_in saddr;
	char ipaddr[32];
	strcpy(ipaddr,"127.0.0.1");
	int ss=socket(AF_INET,SOCK_STREAM,0);
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(1234);
	inet_aton(ipaddr,&(saddr.sin_addr));
	bind(ss,(struct sockaddr*)&saddr,sizeof(saddr));
	listen(ss,N);
	for(i=0;i<N;i++){
		pthread_create(&th[i],NULL,&serve_client,(void*)&ss);
	}
	for(i=0;i<N;i++){
		pthread_join(th[i],NULL);
	}
	return 0;
}
