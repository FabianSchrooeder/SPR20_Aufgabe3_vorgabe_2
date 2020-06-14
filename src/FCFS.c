#include "../lib/FCFS.h"
#include <stdlib.h>
#include <stdio.h>

process* FCFS_tick (process* running_process){
    if (running_process==NULL || running_process->time_left==0){
        running_process=queue_poll(FCFS_queue);
    }
    if (running_process!=NULL){
        running_process->time_left--;
    
    }

    return running_process;
}

int FCFS_startup(){
    FCFS_queue=new_queue();
    if (FCFS_queue==NULL){
        return 1;
    }
    return 0;
}

process* FCFS_new_arrival(process* arriving_process, process* running_process){
    if(arriving_process!=NULL){
        queue_add(arriving_process, FCFS_queue);
    }
    return running_process;
}

void FCFS_finish(){
    free_queue(FCFS_queue);
}