#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define datasize 32
#define BUFSIZE sizeof(int) * 8

typedef struct MsgQueue
{
  long mtype;
  char mdata[datasize];
}MQ;
MQ mq1,mq2,mq3;

char* decToBase(int number, int baseLg) 
{
    char* converted_number_str = malloc(BUFSIZE + 1);
    for(int i = BUFSIZE - 1; i >= 0; i--) 
	{
        int digit = number & (1 << baseLg) - 1;
        number >>= baseLg;
        if(digit > 9)
            converted_number_str[i] = 'A' + (digit - 10);
        else
            converted_number_str[i] = '0' + digit;
    }
    converted_number_str[BUFSIZE] = '\0';
    return converted_number_str;
}

int main()
{
  int mqid, n;
  mqid=msgget(1234,0666|IPC_CREAT);
  printf("\nEnter a positive decimal value: ");
  scanf("%d",&n);
  strcpy(mq1.mdata,decToBase(n,1));
  strcpy(mq2.mdata,decToBase(n,3));
  strcpy(mq3.mdata,decToBase(n,4));
  mq1.mtype=1; 
  mq2.mtype=2; 
  mq3.mtype=3; 
  msgsnd(mqid,&mq1,datasize,0);
  msgsnd(mqid,&mq2,datasize,0);
  msgsnd(mqid,&mq3,datasize,0);
  msgctl(mqid,IPC_RMID,NULL);
  return 1;
}

