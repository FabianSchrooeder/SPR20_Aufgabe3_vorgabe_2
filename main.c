#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/queue.h"
#include "lib/process.h"
#include "lib/FCFS.h"
#include "lib/LCFS.h"
#include "lib/PRIOP.h"
#include "lib/RR.h"
#include "lib/SRTNP.h"
#include "lib/HRRN.h"
#include "lib/MLF.h"
#include "lib/colors.h"


int toggle_color=1;

process** arriving_processes;
int idle_count;

int calc_quantum(int argc, int processes[]){
    if (argc%3!=0){
        return processes[argc-1];
    }
    else {
        return 2;
    }
}


void initialize_processes(int processCount, int processes[]){
    arriving_processes=calloc(processCount/3,sizeof(process*));
    for (int i=0; i<(processCount/3)*3; i=i+3){
        arriving_processes[i/3]=calloc(1,sizeof(process));
        arriving_processes[i/3]->start_time=processes[i];
        arriving_processes[i/3]->time_left=processes[i+1];
        arriving_processes[i/3]->priority=processes[i+2];
        arriving_processes[i/3]->id=i/3;
    }
}

void print_help_long(const char* argv[]){
    printf("Usage: %s [SCHEDULING STRATEGY] [P1 arrival] [P1 duration] [P1 Priority] [P2 arrival]... [round robin quantum (Optional)] [(flags)]\n",argv[0]);
    printf("\nYou can use one of the following SCHEDULING STRATEGIES:\n\nStrategy\tDescription\n-----------------------------------\n");
    printf("FCFS\t\tFirst Come First Served\n");
    printf("LCFS\t\tLast Come First Served (Non-preemtive)\n");
    printf("PRIOP\t\tPriority (Preemtive)\n");
    printf("RR\t\tRound Robin (Specify the RR quantum using the last parameter. Default is 2.\n");
    printf("SRTNP\t\tShortest Remaining Time Next (Preemtive)\n");
    printf("HRRN\t\tHighest Response Ratio Next\n");
    printf("MLF\t\tMultilevel Feedback with at most 4 levels\n\n");
    printf("Example 1: Start two processes A and B. \nA arrives as tick 1 with a duration of 4 and a priority of 3.\nB arrives at 3 with a duration of 3 and priority 7.\n");
    printf("The desired scheduling strategy is Last Come First Served (LCFS):\n");
    printf("%s LCFS 1 4 3 3 3 7\n\n",argv[0]);
    printf("Example 2: Start 3 processes A, B and C\n");
    printf("arrival(A): 0, duration(A): 4, priority(A): 3\n");
    printf("arrival(B): 3, duration(B): 5, priority(B): 2\n");
    printf("arrival(C): 5, duration(C): 1, priority(C): 4\n");
    printf("The desired scheduling strategy is Round Robin(RR) with a quantum of 3:\n");
    printf("%s RR 0 4 3 3 5 2 5 1 4 3\n",argv[0]);
    printf("\nDon't let multiple processes arrive at the same time!\n");
}

void print_help_short(const char* argv[]){
    fprintf(stderr,"Usage: %s [SCHEDULING STRATEGY] [P1 arrival] [P1 duration] [P1 Priority] [P2 arrival]... [round robin quantum (Optional)] [(flags)]\n",argv[0]);
    fprintf(stderr,"Use flag '-help' for further information\n");
}

void print_process(process* running_process){
    if (running_process!=NULL){
        for (int i=0; i< idle_count; i++){
            printf("  |");
        }
        idle_count=0;
        if (toggle_color) set_color(running_process->id);
        printf(" %c ",running_process->id+'A');
        if(toggle_color)reset_color();
        printf("|");
    }
    else {
        idle_count++;
    }
}

int calc_max_time(int process_count){
    int max_time=0;
    for (int i=0;i< process_count;i++){
        max_time+=arriving_processes[i]->start_time+arriving_processes[i]->time_left;
    }
    return max_time;
}

//checks for newly arriving processes at current _time_. Return NULL if there is none
process* check_new_arrival(int process_count, int time){
    process* new_arrival=NULL;
    for(int i=0; i<process_count; i++){
        if (arriving_processes[i]->start_time==time){
            new_arrival=arriving_processes[i];
            break;
        }
    }
    return new_arrival;
}

void free_processes(int process_count){
    for (int i=0; i<process_count;i++){
        free(arriving_processes[i]);
    }
    free(arriving_processes);
}

void handle_flag(int cnt, const char* argv[]){
    if (!strcmp(argv[cnt],"-help")){
        print_help_long(argv);
        exit(0);
    }
    if (!strcmp(argv[cnt],"-c")){
        toggle_color=0;
    }
}

int main(int argc, char const *argv[])
{

    
    //sort out flags
    int process_args[argc];
    int cnt=0;
    for (int i=1; i<argc; i++){
        if(argv[i][0]=='-'){
            handle_flag(i,argv);
        }
        else if (i>=2){
            process_args[cnt]=atoi(argv[i]);
            cnt++;
        }
    }
    //Errorhandling
    if (cnt<3){ //Too few arguments
        print_help_short(argv);
        exit(0);
    }
    

    //Read processes into array "process** arriving_processes"
    initialize_processes(cnt,process_args);
    process* running_process=NULL;
    int process_count=cnt/3;
    int max_time=calc_max_time(process_count);

    idle_count=0;

    
    //Start FCFS Scheduler
    if(!strcmp(argv[1],"FCFS")){
        printf("Starting FCFS scheduler\n|");
        if(FCFS_startup()!=0){
            fprintf(stderr,"Problem starting the FCFS scheduler");
            exit(1);
        }        
        for (int time=0;time<max_time;time++){
            //check for new arrival at given point of time
            process* arriving_process=check_new_arrival(process_count,time);
            
            //Let the FCFS-scheduler handle the processes
            running_process=FCFS_new_arrival(arriving_process,running_process);
            running_process=FCFS_tick(running_process);
            print_process(running_process);
        }
        FCFS_finish();
        
    }

    else if(!strcmp(argv[1],"LCFS")){
        printf("Starting LCFS scheduler\n|");
        if(LCFS_startup()!=0){
            fprintf(stderr,"Problem starting the LCFS scheduler");
            exit(1);
        }        
        for (int time=0;time<max_time;time++){
            //check for new arrival at given point of time
            process* arriving_process=check_new_arrival(process_count,time);
            
            //Let the LCFS-scheduler handle the processes
            running_process=LCFS_new_arrival(arriving_process,running_process);
            running_process=LCFS_tick(running_process);
            print_process(running_process);
        }
        LCFS_finish();
        
    }

    else if (!strcmp(argv[1],"PRIOP")){
        printf("Starting PRIOP scheduler\n|");
        if(PRIOP_startup()!=0){
            fprintf(stderr,"Problem starting the PRIOP scheduler");
            exit(1);
        }        
        for (int time=0;time<max_time;time++){
            //check for new arrival at given point of time
            process* arriving_process=check_new_arrival(process_count,time);
            
            //Let the PRIOP-scheduler handle the processes
            running_process=PRIOP_new_arrival(arriving_process,running_process);
            running_process=PRIOP_tick(running_process);
            print_process(running_process);
        }
        PRIOP_finish();
        
    }

    else if(!strcmp(argv[1],"RR")){
        printf("Starting RR scheduler\n|");
        int RR_quantum=calc_quantum(cnt,process_args);
        if(RR_startup(RR_quantum)!=0){
            fprintf(stderr,"Problem starting the RR scheduler");
            exit(1);
        }        
        for (int time=0;time<max_time;time++){
            //check for new arrival at given point of time
            process* arriving_process=check_new_arrival(process_count,time);
            
            //Let the RR-scheduler handle the processes
            running_process=RR_new_arrival(arriving_process,running_process);
            running_process=RR_tick(running_process);
            print_process(running_process);
        }
        RR_finish();
        
    }

    else if(!strcmp(argv[1],"SRTNP")){
        printf("Starting SRTNP scheduler\n|");
        if(SRTNP_startup()!=0){
            fprintf(stderr,"Problem starting the SRTNP scheduler");
            exit(1);
        }        
        for (int time=0;time<max_time;time++){
            //check for new arrival at given point of time
            process* arriving_process=check_new_arrival(process_count,time);
            
            //Let the SRTNP-scheduler handle the processes
            running_process=SRTNP_new_arrival(arriving_process,running_process);
            running_process=SRTNP_tick(running_process);
            print_process(running_process);
        }
        SRTNP_finish();
        
    }

    else if(!strcmp(argv[1],"HRRN")){
        printf("Starting HRRN scheduler\n|");
        if(HRRN_startup()!=0){
            fprintf(stderr,"Problem starting the HRRN scheduler");
            exit(1);
        }        
        for (int time=0;time<max_time;time++){
            //check for new arrival at given point of time
            process* arriving_process=check_new_arrival(process_count,time);
            
            //Let the HRRN-scheduler handle the processes
            running_process=HRRN_new_arrival(arriving_process,running_process);
            running_process=HRRN_tick(running_process);
            print_process(running_process);
        }
        HRRN_finish();
        
    }

    else if(!strcmp(argv[1],"MLF")){
        printf("Starting MLF scheduler\n|");
        if(MLF_startup()!=0){
            fprintf(stderr,"Problem starting the MLF scheduler");
            exit(1);
        }        
        for (int time=0;time<max_time;time++){
            //check for new arrival at given point of time
            process* arriving_process=check_new_arrival(process_count,time);
            
            //Let the MLF-scheduler handle the processes
            running_process=MLF_new_arrival(arriving_process,running_process);
            running_process=MLF_tick(running_process);
            print_process(running_process);
        }
        MLF_finish();
        
    }
    else{
        print_help_short(argv);
        exit(0);
    }


    free_processes(process_count);
    printf("\n"); 
    return 0;
}
