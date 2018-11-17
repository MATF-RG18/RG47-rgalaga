#include "include/callbacks.h"

void rgOnKeyboard(unsigned char key, int x, int y){
    switch(key) {
        case RG_ESCAPE:
            exit(0);
            break;
        case RG_ARROW_LEFT:
            rgPlayerMove(RG_ARROW_LEFT);
            break;
        case RG_ARROW_RIGHT:
            rgPlayerMove(RG_ARROW_RIGHT);
            break;
    }
}

void rgOnReshape(int width, int height){
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 10);
}

void rgRenderScene(void)
{
    /* Clear color and depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, RG_INITIAL_WINDOW_WIDTH, RG_INITIAL_WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW); 
    
    /* Generate random number of stars to random positions */
    srand(time(NULL));
    int numOfStars = rand() % RG_MAX_STARS;
    int i;
    for (i = 0; i < numOfStars; i++){
        int x_pos = rand() % RG_INITIAL_WINDOW_WIDTH;
        int y_pos = rand() % RG_INITIAL_WINDOW_HEIGHT;
        int z_pos = -1;
        
        glLoadIdentity();
        
        printf("x: %f, y: %f\n", -1 + 2.0 * x_pos / RG_INITIAL_WINDOW_WIDTH, -1 + 2.0 * y_pos / RG_INITIAL_WINDOW_HEIGHT);
        glColor3ub(255, 251, 211);
        glPushMatrix();
            glTranslatef(-1 + 2.0 * x_pos / RG_INITIAL_WINDOW_WIDTH, -1 + 2.0 * y_pos / RG_INITIAL_WINDOW_HEIGHT, z_pos);
            glutSolidSphere(0.003, 50, 50);       
        glPopMatrix();
    }

    glutSwapBuffers();
}
