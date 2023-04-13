#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define datasize 32

typedef struct MsgQueue
{
  long mtype;
  char mdata[datasize];
}MQ;
MQ mq1;

int main()
{
  int mqid;
  mqid=msgget(1234,0666|IPC_CREAT);
  msgrcv(mqid,&mq1,datasize,2,0);
  printf("Octal value = %s\n",mq1.mdata);
  msgctl(mqid,IPC_RMID,NULL);
  return 1;
}

