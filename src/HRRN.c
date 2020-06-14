#include "../lib/HRRN.h"


process* HRRN_tick (process* running_process){
    //TODO
    if (running_process == NULL || running_process->time_left == 0){
        running_process = queue_poll(HRRN_queue);
    }
    if (running_process != NULL){
        running_process->time_left--;
    }
    queue_object* q = HRRN_queue;
    while(q->next != NULL){
        ((process*) q->next->object)->priority += 1 / ((process*) q->next->object)->time_left;		//increment priority by 1/Bedienzeit

        queue_object* pos = HRRN_queue;										//check for new downstream position
        while(pos->next != NULL && ((process*) pos->next->object)->priority < ((process*) q->next->object)->priority){
            pos = pos->next;
        }
        if(pos->next != q->next){											//move process to downstream position
            queue_object* tmp = q->next;
            q->next = q->next->next;
            tmp->next = pos->next;
            pos->next =tmp;
        } else{
            q = q->next;
        }
    }

    return running_process;
}


int HRRN_startup(){
    //TODO
    HRRN_queue = new_queue();
    if (HRRN_queue == NULL){
        return 1;
    }
    return 0;
}

process* HRRN_new_arrival(process* arriving_process, process* running_process){
    //TODO
    if(arriving_process != NULL){
        arriving_process->priority = arriving_process->start_time / arriving_process->time_left;
        queue_object* pos = HRRN_queue;
        while(pos->next != NULL && ((process*) pos->next->object)->priority < arriving_process->priority){
            pos = pos->next;
        }
        queue_add(arriving_process, pos);
    }
    return running_process;
}

void HRRN_finish(){
    //TODO
    free_queue(HRRN_queue);
}