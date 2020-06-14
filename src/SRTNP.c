#include "../lib/SRTNP.h"

queue_object* SRTNP_queue;

process* SRTNP_tick (process* running_process) {
    if (running_process == NULL || running_process->time_left == 0) {
        running_process = queue_poll(SRTNP_queue);
    }
    if (running_process != NULL) {
        running_process->time_left--;
    }
    return running_process;
}

int SRTNP_startup()
{
    SRTNP_queue=new_queue();
    if (SRTNP_queue==NULL){
        return 1;
    }
    return 0;

}

process* SRTNP_new_arrival(process* arriving_process, process* running_process){
    if (arriving_process ==NULL)
    {
        return NULL;
    }
    if(running_process == NULL)
    {
        running_process = arriving_process;
    }
    if(running_process -> time_left <= arriving_process -> time_left)
    {

        queue_add(arriving_process, SRTNP_queue);
        return running_process;
    }
    else
        {
            queue_add(running_process, SRTNP_queue);
            return arriving_process;
        }
}

void SRTNP_finish()
{
        free_queue(SRTNP_queue);
}