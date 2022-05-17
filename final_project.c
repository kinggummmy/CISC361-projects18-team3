#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int sjf();
int fifo();
int rr();
void printQueues(char *x);

/*Struct that creates a job, takes the job number, memory to be used, devices, time, and priority
   Num Job mostly used for identification  */
struct Job{
    int num_job;
    int memory;
    int devices_needed;
    int time;
    int priority;
};
/* First hold queue struct, for Priority 1 Jobs */
struct HoldQueue1{
    struct Job;
    struct HoldQueue1 *next;
};
/* Second hold queue struct, for Priority 2 Jobs */
struct HoldQueue2{
    struct Job;
    struct HoldQueue2 *next;
};
/* Ready Queue Jobs */
struct ReadyQueue{
    struct Job;
    struct ReadyQueue *next;
};

int TOTAL_MEMORY = 0;
int SERIAL_DEVICES = 0;
int TIME_SLICE = 0;
int MEM_REQUIRED = 0;
int PRIORITY = 0;
int JOB_NUMBER = 0;

int main(){
    char x[100];
    int q = 0;
    FILE *ptr = fopen("input.txt", "r");
    /*While File does not end, will run D once*/
    while(q == 0){
        fgets(x, 100, ptr);
        if(x[0] == 'C'){
            /*Code for initilizing the machine*/
            char *found[10];
            char * end;
            /*Adding all the values to a char array, seperated by spaces*/
            found[0] = strtok(x, " ");
            for(int i = 1; i < 5; i++){
                found[i] = strtok(NULL, " ");
            }
            TOTAL_MEMORY = strtol(found[2]+2, &end, 10);
            SERIAL_DEVICES = strtol(found[3]+2, &end, 10);
            TIME_SLICE = strtol(found[4]+2, &end, 10);
            printf("System with memory %d, %d Serial Devices, and Quantum Slice of %d Created\n", TOTAL_MEMORY, SERIAL_DEVICES, TIME_SLICE);

        }
        else{
            if(x[0] == 'D'){
                /*If Time = 9999, End the while loop*/
                if(x[2] == '9' && x[3] == '9'){
                    printQueues(x);
                    q = 1;
                }
                /*Else Print at time */
                else{
                    printQueues(x);
                }
            }
            else{
                /*
                        Function for jobs being created
                */
                if(x[0] == 'A'){
                    char *found[10];
                    char * end;
                    int priority, job_num, memory, devices, time;
                    /*Adding all the values to a char array, seperated by spaces*/
                    found[0] = strtok(x, " ");
                    for(int i = 1; i < 7; i++){
                        found[i] = strtok(NULL, " ");
                    }
                    /*Reading all the specifications*/
                    job_num = strtol(found[2]+2, &end, 10);
                    memory = strtol(found[3]+2, &end, 10);
                    devices = strtol(found[4]+2, &end, 10);
                    time = strtol(found[5]+2, &end, 10);
                    priority = strtol(found[6]+2, &end, 10);
                    struct Job new_job = {job_num, memory, devices, time, priority};
                    printf("Job #%d with Priority %d, %d Memory, %d Devices Needed, and a Time of %d Created\n", job_num,priority,memory,devices,time);
                    if(new_job.priority == 1){
                        sjf();
                    }
                    else{
                        fifo();   
                    }
                }
                else{
                    /*
                        Function for jobs reserving devices from the queue
                    */
                    if(x[0] == 'Q'){
                        char *found[10];
                        char * end;
                        int job_num, devices;
                        /*Adding all the values to a char array, seperated by spaces*/
                        found[0] = strtok(x, " ");
                        for(int i = 1; i < 3; i++){
                            found[i] = strtok(NULL, " ");
                        }
                        job_num = strtol(found[2]+2, &end, 10);
                        devices = strtol(found[3]+2, &end, 10);
                        printf("Job #%d requesting %d Devices\n", job_num, devices);
                    }
                    else{
                        /*
                            Function for jobs releasing devices from the queue
                        */
                        if(x[0] == 'L'){
                            char *found[10];
                            char * end;
                            int job_num, devices;
                            /*Adding all the values to a char array, seperated by spaces*/
                            found[0] = strtok(x, " ");
                            for(int i = 1; i < 3; i++){
                                found[i] = strtok(NULL, " ");
                            }
                            job_num = strtol(found[2]+2, &end, 10);
                            devices = strtol(found[3]+2, &end, 10);
                            printf("Job #%d releasing %d Devices\n", job_num, devices);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void printQueues(char *x){
    char *found[1];
    char * end;
    found[0] = strtok(x, " ");
    int time = strtol(found[0]+2, &end, 10);
    printf("At Time: %d\n", time);
    printf("Current Available Main Memory = %d\n", TOTAL_MEMORY);
    printf("Current Devices = %d\n", SERIAL_DEVICES);
}

int sjf(){
    int turnaroundTime[20], burstTime[20], waitTime[20], p[20], temp, posiiton, i, j, n, total = 0;
    float averageWait, averageTurn;
    printf("Enter # of processes:");
    scanf("%d",&n);

    printf("\nEnter Burst Time:\n");
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

    printf("\nProccess    Burst Time  Wait Time   Turn Time\n");
    for(i=0; i<n; i++){
        turnaroundTime[i] = burstTime[i] + waitTime[i];
        total += turnaroundTime[i];
        printf("%d  %d  %d  %d", p[i], burstTime[i], waitTime[i], turnaroundTime[i]);
    }

    averageTurn = (float)total/n;
    printf("\nAverage Waiting Time=%f",averageWait);
    printf("\nAverage Turnaround Time=%f\n",averageTurn);   
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
}
