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
    int i,j;
	int mqid = msgget(1234, 0666|IPC_CREAT);
	MQ mqs;
	//Receive no of students
	msgrcv(mqid, (void *)&mqs, sizeof(mqs)-sizeof(long),3,0);
	
	//Sorting Student Details
	for(i=0;i<n-1;i++){
		for(j=0;j<n-1-i;j++){
			if(mqs.mdata[j].roll>mqs.mdata[j+1].roll){
				STUD temp = mqs.mdata[j];
				mqs.mdata[j] = mqs.mdata[j+1];
				mqs.mdata[j+1] = temp;
			}
		}
	}
	
	printf("\n\nSorted Data by Roll\n\n");
	for(i=0;i<n;i++){
		printf("%s\t- %d\n", mqs.mdata[i].name, mqs.mdata[i].roll);
	}

	
	//Send sorted data to Process 1
	mqs.mtype=1;
	msgsnd(mqid,(void *)&mqs, sizeof(mqs)-sizeof(long), 0);
	return 0;
}
