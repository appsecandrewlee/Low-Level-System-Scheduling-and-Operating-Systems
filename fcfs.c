#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include <unistd.h>
#include <time.h>


//START DATE MODIFICATION: 05-OCT-2022
//END DATE MODIFICATION: 16-OCT-2022

//The following program will demostrate scheduling algorithm first come first serve, a non pre-emptive scheduling that will run the entire process
//and swap until the current process is finished before jumping to the next. which indicates the queue for the process is sequential.
//the aforementioned information indicates the process will start after the current process is NULL or finished, and will execute until all current processes
//are completely executed.

//INPUT:
//Entry,Service
//P1 0 3
//P2 1 4

//OUTPUT:
//TIME 0: P1 ENTERS
//TIME 1: P1 RUNS
//TIME 1: P2 ENTERS
//TIME 2: P1 RUNS
//TIME 3: P1 RUNS
//TIME 3: P1 EXITS
//TIME 4: P2 RUNS
//TIME 5: P2 RUNS
//TIME 6: P2 RUNS
//TIME 7: P2 RUNS
//TIME 7: P2 EXITS


//Student ID: 30864941
//Student Name: ANDREW LEE

//


#define INTERVAL 1 //time interval

void Task1(pcb_t *elements,int numOfLines);
int main(int argc,char *argv[]){

    FILE *infile,*outfile;

    char process_name[11];
    int entry;
    int service;
    int remaining;
    pcb_t *elements;
    int counter = 0;

    //open the file for the first argument
    infile = fopen(argv[1], "r");
    if (infile == NULL)
        //if it is null then open processes.txt
        infile = fopen("processes.txt", "r");

    //scan the infile
    while (fscanf(infile, "%s %d %d %d\n", process_name, &entry, &service, &remaining) > 0) {
        counter++;
        printf("%d\n",counter);
    }
    //seek the start of the file and set it at 0
    fseek(infile, 0, SEEK_SET);

    //allocate enough memory for the counter
    elements = (pcb_t *)malloc(counter * sizeof(pcb_t));
    for (int i = 0; i < counter; i++) {
        fscanf(infile, "%s %d %d %d\n", elements[i].process_name, &elements[i].entryTime, &elements[i].serviceTime, &elements[i].remainingTime);
        elements[i].state = READY;
        printf("%s %d %d %d\n", elements[i].process_name,elements[i].entryTime,elements[i].serviceTime,elements[i].remainingTime);
    }
    //execute first come first serve algorithm
    Task1(elements,counter);
    //write to the output
    outfile = fopen("results-task1_30864941","w");
    for (int i =0;i<counter;i++) {
        fprintf(outfile, "%s %d %d\n", elements[i].process_name,elements[i].entryTime,elements[i].serviceTime);
    }


    fclose(infile);
    fclose(outfile);
    return 0;

}


void Task1(pcb_t *processes,int numOfLines){

    int time = 0;
    int current_running_index = 0;
    int current_entered_index = 0;
    pcb_t *enteredProcess = NULL; //
    pcb_t *currentProcess = NULL; //ONLY USE ONCE for pcb_t
    int currentProcessRunTime = 0;
    //accounting for edge cases, if the entrytime is greater than the service time
    int total_service_time = processes[numOfLines - 1].entryTime;
    //adding up all the service time
    for (int i = 0; i < numOfLines; i++) {
        total_service_time += processes[i].serviceTime;
    }
    //allocate memory for the number of lines which is 4
    enteredProcess = (pcb_t *)malloc(numOfLines * sizeof(pcb_t));

    while (time < total_service_time) {
        //Check if the entrytime is the same as current time, since it is first come first serve
        while (processes[current_entered_index].entryTime == time) { //0
            enteredProcess[current_entered_index] = processes[current_entered_index]; //the process is currently entered
            enteredProcess[current_entered_index].state = READY; //put the current process state
            printf("TIME:%d %s has entered the system\n", time, processes[current_entered_index].process_name); //print out which process has entered the system
            current_entered_index++;
        }

        //the processes are all ready, now starting to run, it checks for 3 conditions, process has to be ready, it has to be less or equal to the time and the current process has to be NULL
        if (enteredProcess[current_running_index].state == READY && enteredProcess[current_running_index].entryTime <= time && currentProcess == NULL) {
            printf("TIME:%d %s is in running state\n", time, enteredProcess[current_running_index].process_name);
            currentProcess = &enteredProcess[current_running_index];
            currentProcessRunTime = time + currentProcess->serviceTime;
            current_running_index++;
        }


        if (time == currentProcessRunTime) {
            printf("TIME:%d %s has exited the system\n", time, currentProcess->process_name);
            if (enteredProcess[current_running_index].entryTime <= time) {
                currentProcess = &enteredProcess[current_running_index];
                currentProcessRunTime = time + currentProcess->serviceTime;
                current_running_index++;
                printf("TIME:%d %s is in running state\n",time,currentProcess->process_name);
            }else {
                currentProcess = NULL;
            }
        }
        //real time simulation for sleep() and increment the time variable after all logic has been cleared.
        time++;
        sleep(INTERVAL);
    }

}































//TRASH THOUGHTS
//  int queue_count = 0;
//  char* tokenArr[4];
//  char buffer[100];
//  char * token;
//LINE BY LINE READ INTO THE BUFFER
/*while (fgets(buffer,sizeof(buffer),infile)) {
    token = strtok(buffer, " ");
    int inCount = 0;
    while (token != NULL) //if the token is not null
    {
        tokenArr[inCount] = token;
        inCount++;
        token = strtok(NULL, " "); //For every character that contains a space,make it NULL.
    }

    queue[queue_count].process_name[0] = *tokenArr[0];
    queue[queue_count].process_name[1] = '1';
    queue[queue_count].entryTime = atoi(tokenArr[1]);
    queue[queue_count].serviceTime = atoi(tokenArr[2]);
    queue[queue_count].remainingTime = atoi(tokenArr[3]);

    printf("%c\n",queue[queue_count].process_name[0]);
    memset(&tokenArr,'\0',4);
    bzero(tokenArr,4);
    printf("after memset %s",tokenArr[0]);
    queue_count++;
    //  for (int i = 0; i < 4; i++) {

*/




//elements.process_name[0] = token[0];
//elements.entryTime = atoi(&token[1]);
// elements.serviceTime = atoi(&token[2]);
//elements.remainingTime = atoi(&token[3]);
//elements.state = READY;
//printf("%c\n", token[0]);

//memset(queue,0,100*sizeof(queue))

