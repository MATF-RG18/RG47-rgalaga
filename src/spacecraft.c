#include "include/spacecraft.h"
#include "include/constants.h"

void rgRenderPlayer(){
    printf("render player");     
}

void rgPlayerMove(int move){
    if (move == RG_ARROW_RIGHT){
        printf("moved right\n");
    } else if (move == RG_ARROW_LEFT){
        printf("moved left\n");
    }
}

