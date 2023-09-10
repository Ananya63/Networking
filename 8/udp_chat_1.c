#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>

char sip_addr[32], rip_addr[32];
struct sockaddr_in sad, rad;

void* snd(void* args)
{
	char msg[32];
	int sid = (*(int*)args);
	while(1)
	{
		//printf("\t\t\tEnter the messege you want to send: ");
		scanf("%s",msg);
		msg[strlen(msg)] = '\0';
		sendto(sid, (void*)msg, strlen(msg), 0, (struct sockaddr*)&rad, sizeof(rad)); 
		if(!strcmp(msg, "tata"))
		{
			printf("\t\t\t\t\t\tYOU: %s\n", msg);
			break; 
	         }
		printf("\t\t\t\t\t\tYOU: %s\n", msg);
		sleep(1);
	}
	printf("\t\t\t\t\tYou have exited the chat..\n");
	pthread_exit(NULL);
}

void* rcv(void* args)
{
	char msg[32];
	int sid = (*(int*)args);
	int l = sizeof(rad);
	int n;
	while(1)
	{
		n = recvfrom(sid, (void*)msg, sizeof(msg), 0, (struct sockaddr*)&rad, &l);
		msg[n] = 0;
		if(!strcmp(msg, "tata"))
			break;
		printf("\nOTHER USER: %s\n",msg);
	}
	printf("\n\t The OTHER USER has exited the chat..\n");
	pthread_exit(NULL);
}

int main (int ac, char **av)
{
	int i, j, l, n, sid, cid;	

	if (ac == 1){
	  	strcpy (sip_addr, "127.0.0.2");
		strcpy (rip_addr, "127.0.0.1");
	}
	else{
	  	strcpy (sip_addr, av[1]);
	  	strcpy(rip_addr, av[2]);
	}
	sid = socket (AF_INET, SOCK_DGRAM, 0);
	sad.sin_family = AF_INET;
	inet_aton(sip_addr, &(sad.sin_addr));
	sad.sin_port = htons(4322);
	bind (sid, (struct sockaddr *) &sad, sizeof (sad));
	rad.sin_family = AF_INET;
	inet_aton(rip_addr, &(rad.sin_addr));
	rad.sin_port = htons(4321);
	pthread_t th[2];
	printf("Messages are end-to-end encrypted. You are Online now.\n\n");
	pthread_create(&th[0], NULL, &snd, &sid);
	pthread_create(&th[1], NULL, &rcv, &sid);
	pthread_join(th[0], NULL);
	pthread_join(th[1], NULL);
	printf("\nYou are offline now.");
	close(sid);
	return 0;
}
