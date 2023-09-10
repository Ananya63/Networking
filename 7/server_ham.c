#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

char* HammingCode( char *data )
{
    int m = strlen(data);
    int i,j,r=1,k=0,count = 0, shift = 0;
    for ( i = 0 ; i < m/2 ; i++ )
    {
        char temp = data[i];
        data[i] = data[m -1 -i];
        data[m -1 -i] = temp; 
    }

    while((1<<r < (m+r+1))) {r++;}

    char *code = (char*)malloc(sizeof(char)*(m+r+1));
    code[m+r] = '\0';

    for(i=0;i<m+r;i++)
    {
		if( (i+1) & i)
			code[i]=data[k++];
		else
			code[i]='r';
	}

    for(i=0;i<r;i++){
		count=0;
		shift=1<<i;
		for(j=1;j<=(m+r);j++){	
			if(j&shift){
				if(code[j-1]=='1')
					count++;
			}
		}
		code[shift-1]='0'+(count&1);
	}
    int Hm = strlen(code);
    for ( i = 0 ; i < Hm/2 ; i++ )
    {
        char temp = code[i];
        code[i] = code[Hm -1 -i];
        code[Hm -1 -i] = temp; 
    }

    return code;
}

int main( int argc , char* argv[])
{
    char ip[32] , data[64] , code[129];
    strcpy(ip,( argc == 1 ? "127.0.0.1" : argv[1]) );
    int ss = socket(AF_INET, SOCK_DGRAM , 0);
    struct sockaddr_in saddr , caddr;
    saddr.sin_family=AF_INET;
	saddr.sin_port=htons(1234);
	inet_aton(ip,&(saddr.sin_addr));
	bind(ss,(struct sockaddr*)&saddr,sizeof(saddr));

    while(1){
        printf("\nWaiting for data from client\n");
		int len=sizeof(caddr);
        recvfrom(ss,(void*)data,sizeof(data),0,(struct sockaddr*)&caddr,&len);
		if(strcasecmp(data,"exit")==0) break;
		printf("Received dataword from client: %s\n",data);
		strcpy(code,HammingCode(data));
		printf("Finding Codeword: %s\n",code);
		sendto(ss,(void*)code,strlen(code)+1,0,(struct sockaddr*)&caddr,sizeof(caddr));
    }
    puts("Exiting...");
    close (ss);
    return 0;
}
