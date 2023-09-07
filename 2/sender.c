#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define n 5

typedef struct student{	
	char name[32];
	int roll;
}STUD;

typedef struct MsgQ{
	long mtype;
	STUD mdata[n];
}MQ;

int main(){
int i;
	int mqid = msgget(1234, 0666|IPC_CREAT);
	MQ mqs, mqr1, mqr2;
	
	//Store Students details
	mqs.mtype = 2;
	for(i=0;i<n;i++){
	    printf("Enter Student Name:");
		scanf("%s",mqs.mdata[i].name);
	    printf("Enter Student Roll:");
		scanf("%d",&(mqs.mdata[i].roll));
	}
	
	//Send students details to Process 2
	msgsnd(mqid,(void *)&mqs, sizeof(mqs)-sizeof(long),0);
	
	//Receiving Sorted data by name from Process 2
	msgrcv(mqid, (void *)&mqr1, sizeof(mqr1)-sizeof(long),1,0);

	printf("\n\nSorted Data by Names\n\n");
	for(i=0;i<n;i++){
		printf("%s\t- %d\n", mqr1.mdata[i].name, mqr1.mdata[i].roll);
	}
	
	//Send students details to Process 3
	mqs.mtype = 3;
	msgsnd(mqid,(void *)&mqs, sizeof(mqs)-sizeof(long),0);
	
	//Receiving Sorted data by name from Process 3
	msgrcv(mqid, (void *)&mqr2, sizeof(mqr2)-sizeof(long),1,0);

	printf("\nSorted Data by Roll\n\n");
	for(i=0;i<n;i++){
		printf("%s\t- %d\n", mqr2.mdata[i].name, mqr2.mdata[i].roll);
	}
	
	msgctl(1234, IPC_RMID, NULL);
}
