#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int TOTAL_MEMORY = 0;
int SERIAL_DEVICES = 0;
int TIME_SLICE = 0;
int MEM_REQUIRED = 0;
int PRIORITY = 0;
int JOB_NUMBER = 0;

int sjf();
int fifo();
int rr();


int main(){
    char x[100];
    FILE *ptr = fopen("input.txt", "r");
    fgets(x, 100, ptr);
    if(x[0] == 'C'){
        char m_find = '=';
        int n = 0;
        char *found = strchr(x, m_find);
        char * end;
        found = found + 1;
        found = strtok(found, " ");
        printf(found);
        n = strtol(found, &end, 10);
        printf("\n");
        printf("%x\n", n);

        SERIAL_DEVICES = 0;

        TIME_SLICE = 0;
    }
    else{
        if(x[0] == 'D'){
        }
        else{
            if(x[0] == 'A'){
                MEM_REQUIRED = 0;
                if(PRIORITY == 1){
                    sjf();
                }
                else{
                    fifo();
                }
            }
            else{
                if(x[0] == 'Q'){

                }
                else{
                    if(x[0] == 'L'){

                    }
                }
            }
        }
    }
    printf("Done");
    return 0;
}


int sjf(){
    int turnaroundTime[20], burstTime[20], waitTime[20], p[20], temp, posiiton, i, j, n, total = 0;
    float averageWait, averageTurn;
    printf("Enter # of processes:");
    scanf("%d",&n);

    printf("nEnter Burst Time:n");
    for(i=0;i<n;i++)
    {
        printf("p%d:",i+1);
        scanf("%d",&burstTime[i]);
        p[i]=i+1;         
    }

    //sort times of burst
    for(i=0; i<n; i++){
        posiiton = i;
        for(j=i+1; j<n; j++){
            if(burstTime[j]<burstTime[posiiton])
                posiiton = j;
        }
        temp = burstTime[i];
        burstTime[i] = burstTime[posiiton];
        burstTime[posiiton]=temp;

        temp = p[i];
        p[i] = p[posiiton];
        p[posiiton] = temp;
    }

    waitTime[0] = 0;

    for(i=1; i<n; i++){
        waitTime[i] = 0;
        for(j=0; j<i; j++){
            waitTime[i] = burstTime[j];
        }
        total += waitTime[i];
    }
    averageWait = (float)total/n;
    total = 0;

    printf("nProcces    Burst Time  Wait Time   Turn Time");
    for(i=0; i<n; i++){
        turnaroundTime[i] = burstTime[i] + waitTime[i];
        total += turnaroundTime[i];
        printf("%d  %d  %d  %d", p[i], burstTime[i], waitTime[i], turnaroundTime[i]);
    }

    averageTurn = (float)total/n;
    printf("nnAverage Waiting Time=%f",averageWait);
    printf("nAverage Turnaround Time=%fn",averageTurn);   
}

int fifo(){
    int n,burstTime[20],waitTime[20],turnaroundTime[20],averageWait=0,averageTurn=0,i,j; 
    printf("Enter total number of processes:");
    scanf("%d",&n);

    printf("\nEnter Process Burst Time\n");
    for(i=0;i<n;i++)
    {
        printf("P[%d]:",i+1);
        scanf("%d",&burstTime[i]);
    }
 
    waitTime[0]=0;

   //calculating waiting time
    for(i=1;i<n;i++)
    {
        waitTime[i]=0;
        for(j=0;j<i;j++)
            waitTime[i]+=burstTime[j];
    }
 
    printf("\nProcess\t\tBurst Time\tWaiting Time\tTurnaround Time");
 
    //calculating turnaround time
    for(i=0;i<n;i++)
    {
        turnaroundTime[i]=burstTime[i]+waitTime[i];
        averageWait+=waitTime[i];
        averageTurn+=turnaroundTime[i];
        printf("\nP[%d]\t\t%d\t\t%d\t\t%d",i+1,burstTime[i],waitTime[i],turnaroundTime[i]);
    }
 
    averageWait/=i;
    averageTurn/=i;
    printf("\n\nAverage Waiting Time:%d",averageWait);
    printf("\nAverage Turnaround Time:%d",averageTurn);
 
    return 0;    
}

int rr(){
     int i, n, sum=0,count=0, k, qt, waitTime=0, turnaroundTime=0, arrivalTime[10], burstTime[10], temp[10];
     float averageWait, averageTurn;

     printf("Enter number of processes:");
     scanf("%d", &n);
     k = n;

     for(i=0; i<n; i++){
        printf("\n Enter the Arrival and Burst time of the Process[%d]\n", i+1);  
        printf(" Arrival time is: \t");  // Accept arrival time  
        scanf("%d", &arrivalTime[i]);  
        printf(" \nBurst time is: \t"); // Accept the Burst time  
        scanf("%d", &burstTime[i]);  
        temp[i] = burstTime[i];
     }
    printf("Enter the Time Quantum for the process: \t");  
    scanf("%d", &qt);  
    // Display the process No, burst time, Turn Around Time and the waiting time  
    printf("\n Process No \t\t Burst Time \t\t TAT \t\t Waiting Time ");  
    for(sum=0, i = 0; k!=0; ) {  
        if(temp[i] <= qt && temp[i] > 0){  
            sum = sum + temp[i];  
            temp[i] = 0;  
            count=1;  
            }     
        else if(temp[i] > 0) {  
            temp[i] = temp[i] - qt;  
            sum = sum + qt;    
        }  
        if(temp[i]==0 && count==1){  
            k--; //decrement the process no.  
            printf("\nProcess No[%d] \t\t %d\t\t\t\t %d\t\t\t %d", i+1, burstTime[i], sum-arrivalTime[i], sum-arrivalTime[i]-burstTime[i]);  
            waitTime = waitTime+sum-arrivalTime[i]-burstTime[i];  
            turnaroundTime = turnaroundTime+sum-arrivalTime[i];  
            count =0;     
        }  
        if(i==n-1){  
            i=0;  
        }  
        else if(arrivalTime[i+1]<=sum)  
        {  
            i++;  
        }  
        else  
        {  
            i=0;  
        }  
    }  
    // represents the average waiting time and Turn Around time  
    averageWait = waitTime * 1.0/n;  
    averageTurn = turnaroundTime * 1.0/n;  
    printf("\n Average Turn Around Time: \t%f", averageWait);  
    printf("\n Average Waiting Time: \t%f", averageTurn);  
    getch();      
}
