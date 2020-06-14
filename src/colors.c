#include "../lib/colors.h"
#include <stdio.h>

void set_color(int index){
    printf("\033[%c;3%cm",'0'+index%2,'1'+index%6);
}

void reset_color(){
    printf("\033[0m");
}