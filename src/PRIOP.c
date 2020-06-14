#include "../lib/PRIOP.h"
#include <stdio.h>


process* PRIOP_tick (process* running_process){
    //TODO
    if (running_process == NULL || running_process->time_left == 0){
        running_process = queue_poll(PRIOP_queue);
    }
    if (running_process != NULL){
        running_process->time_left--;
    }
    return running_process;
}

int PRIOP_startup(){
    //TODO
    PRIOP_queue = new_queue();
    if (PRIOP_queue == NULL){
        return 1;
    }
    return 0;
}

process* PRIOP_new_arrival(process* arriving_process, process* running_process){
    //TODO
    if(arriving_process != NULL){
        queue_object* pos = PRIOP_queue;
        if(running_process != NULL && running_process->priority < arriving_process->priority){
            while(pos->next != NULL && ((process*) pos->next->object)->priority < running_process->priority){
                pos = pos->next;
            }
            queue_add(running_process, pos);
            return arriving_process;
        } else{
            while(pos->next != NULL && ((process*) pos->next->object)->priority < arriving_process->priority){
                pos = pos->next;
            }
            queue_add(arriving_process, pos);
        }
    }
    return running_process;
}

void PRIOP_finish(){
    //TODO
    free(PRIOP_queue);
}