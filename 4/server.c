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
    char data[100], divisor[32],crc[32],temp[32],q[100],rem[32],key[32],d[100]={0};
    while(1)
    {
	printf("\nWaiting for data from client\n");
	read(ts,(void*)data,sizeof(data));
	if(strncmp(data, "exit", 4) == 0) 
        {
            unlink("mysocket");
            puts("Exiting...");
            exit(1);
        }
	printf("\nWaiting for divisor from client\n");
	read(ts,(void*)divisor,sizeof(divisor));
        printf("Recieved data : %s\n",data);
	printf("Divisor: %s\n",divisor);
	int i,j;
	int gen_len=strlen(divisor);
	int data_len=strlen(data);
	for(i=0;i<gen_len-1;i++)
	{
	    data[data_len+i]='0';
	}
	printf("Dividend with zeros appended: %s\n",data);
	strcpy(d,data);
	strcpy(key,divisor);
	for(i=0;i<gen_len;i++)
	    temp[i]=data[i];
	for(i=0;i<data_len;i++)
	{    
	    q[i]=temp[0];
	    if(q[i]=='0')
	       for(j=0;j<gen_len;j++)
	           divisor[j]='0';
	    else
	       for(j=0;j<gen_len;j++)
	            divisor[j]=key[j];
	    for(j=gen_len-1;j>0;j--)
	    {
	        if(temp[j]==divisor[j])
	             rem[j-1]='0';
	        else
	             rem[j-1]='1';
	    }
	    rem[gen_len-1]=data[i+gen_len];
	    strcpy(temp,rem);
	}
	strcpy(crc,temp);
	printf("Sending codeword\n");
	write(ts,(void*)crc,strlen(crc)+1);
     }
     close(ts);
     close(ss);
     return 0;
}

