#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h" //the actual structure.
#include <unistd.h>
#include <time.h> //time library to use sleep()


//START DATE MODIFICATION: 05-OCT-2022
//END DATE MODIFICATION: 15-OCT-2022

//The following program will demonstrate scheduling algorithm: Round Robin
//Round Robin is a scheduling algorithm that will prioritise all processes equally, and run given processes
//based on a time limit known as a time slice quantum, in the following example file it is required to use
//time slice quantum of 2, which indicates that the time of each process will run for 2 seconds before
//switching to the other process, and will execute until all the service time has been satisfied.

//Example INPUT:

//time slice of 3
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
//TIME 6: P2 EXITS
//TIME 7: P1 RUNS
//TIME 8: P1 EXITS
//TIME 8: P2 RUNS
//TIME 9: P2 RUNS


//Student ID: 30864941
//Student Name: Andrew Lee


#define INTERVAL 1

void Task2(pcb_t *elements,int numOfLines);
int main(int argc,char *argv[]){

    FILE *infile,*outfile;

    char process_name[11];
    int entry;
    int service;
    int remaining;
    pcb_t *elements;
    int counter = 0;

    //READFILE
    //open the file for the first argument
    infile = fopen(argv[1], "r");
    //if the argument is NULL
    if (infile == NULL)
        //open processes.txt
        infile = fopen("processes.txt", "r");
    //fscanf function to scan in all the elements in its respective formatter
    //fscan will read the entire file, but it will be at the end of the file after its done reading
    while (fscanf(infile, "%s %d %d %d\n", process_name, &entry, &service, &remaining) > 0) {
        //increment the counter
        counter++;
        printf("%d\n",counter);
    }

    //set the infile to 0
    fseek(infile, 0, SEEK_SET);

    //allocate the correct memory for reading processes.txt
    elements = (pcb_t *)malloc(counter * sizeof(pcb_t));
    for (int i = 0; i < counter; i++) {
        //scan all the elements and put it in the respective arrray
        fscanf(infile, "%s %d %d %d\n", elements[i].process_name, &elements[i].entryTime, &elements[i].serviceTime, &elements[i].remainingTime);
        elements[i].remainingTime = elements[i].serviceTime;
        elements[i].state = READY;
        printf("%s %d %d %d\n", elements[i].process_name,elements[i].entryTime,elements[i].serviceTime,elements[i].remainingTime);
    }

    //implement Round Robin Scheduling Algorithm
    Task2(elements,counter);
    //write the respective files to results-task2_30864941
    outfile = fopen("results-task2_30864941","w");
    for (int i =0;i<counter;i++) {
        fprintf(outfile, "%s %d %d\n", elements[i].process_name,elements[i].entryTime,elements[i].serviceTime);
    }

    //closing files
    fclose(infile);
    fclose(outfile);
    return 0;

}


void Task2(pcb_t *processes,int numOfLines){

    int time = 0;
    int time_quantum = 2;
    int currentProcessRunTime = 0;
    int current_running_index = 0;
    int current_entered_index = 0;
    pcb_t *enteredProcess = NULL; //
    pcb_t *currentProcess = NULL; //ONLY USE ONCE for pcb_t
    int totalProcessRunningTime = 0;
    int total_service_time = 0;
    for (int i = 0; i < numOfLines; i++) {
        total_service_time += processes[i].serviceTime;
    }
    enteredProcess = (pcb_t *)malloc(numOfLines * sizeof(pcb_t));

    while (time < total_service_time+1) { //total service time is 15 + 1

        //ENTRY STATE
        while (processes[current_entered_index].entryTime == time) {
            enteredProcess[current_entered_index] = processes[current_entered_index]; //the process is currently entered
            enteredProcess[current_entered_index].state = READY; //put the current process state
            printf("TIME:%d %s has entered the system\n", time, processes[current_entered_index].process_name); //print out which process has entered the system
            current_entered_index++; //increment the current entered index
            current_running_index = current_entered_index - 1;
            printf("%d\n",current_running_index);

        }

        //READY, the current process has to be ready, and less than time and needs to be set to NULL
        if (enteredProcess[current_running_index].state == READY && enteredProcess[current_running_index].entryTime <= time && currentProcess == NULL) {
            printf("TIME:%d %s is in running state\n", time, enteredProcess[current_running_index].process_name);
            currentProcess = &enteredProcess[current_running_index]; //currentprocess will get the value of the array
            totalProcessRunningTime = time + currentProcess->remainingTime; //calculate remaining time
            current_running_index++; //increment the running index
            current_running_index %= current_entered_index; //use modulo so we can get the number of elements in the array in order to place them in the queue.
            printf("%d\n",current_running_index);

        }

        //EXIT STATE
        if (time == totalProcessRunningTime)
        {
            current_running_index--; //decrement the current running index
            printf("TIME:%d %s has exited the system\n", time, currentProcess->process_name); //print out which process has exited the system
            currentProcess->state = EXIT; //put it on exit


            for (int i = current_running_index;i < current_entered_index;i++){
                //element position swap
                enteredProcess[current_running_index] = enteredProcess[current_running_index +1];
            }
            //decrement the size of the array by 1
            current_entered_index--;
            //if the entrytime is less than time
            if (enteredProcess[current_running_index].entryTime <= time)
            {
                //perform the following logic
                currentProcess = &enteredProcess[current_running_index];
                //have to check for the  current process remaining time and add to the current time.
                totalProcessRunningTime = time + currentProcess->remainingTime;
                //increment the current running index
                current_running_index++;
                //check for the number of elements, because since processes entered, it cannot enter again, it is a quick way to check for elements.
                current_running_index %= current_entered_index;
                printf("%d\n",current_running_index);
                printf("TIME:%d %s is in running state\n",time,currentProcess->process_name);
                printf("%d\n",current_running_index);
            }
            else
            {
                currentProcess = NULL;
            }
        }
        //printing out remaining time, total process time, current running index
        if(currentProcess->remainingTime > 0){
            currentProcessRunTime++;
            currentProcess->remainingTime --;
            printf("TIME:%d %s Remaining time: %d Current time: %d TPT: %d CRI: %d\n",time,currentProcess->process_name, currentProcess->remainingTime, time, totalProcessRunningTime, current_running_index);
            if (currentProcessRunTime == time_quantum)
            {
                currentProcess = NULL;
                currentProcessRunTime = 0;
            }
        }
        //simulate the actual processes, which takes 1 second each
        time++;
        sleep(INTERVAL);
    }

}




































