#include "include/spacecraft.h"
#include "include/constants.h"

void rgRenderPlayer(){
    printf("render player\n");     

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 1, 0, 0, -1, 0, 1, 0);
    
    glPushMatrix();
        glColor3ub(158, 178, 209);
        glScalef(0.8, 1, 0.9);
        glTranslatef(0, -0.85, 0);
        glutSolidSphere(0.1, 50, 50);
    glPopMatrix();
}

void rgPlayerMove(int move){
    if (move == RG_KEY_A){
        printf("moved left\n");
    } else if (move == RG_KEY_D){
        printf("moved right\n");
    }
}

