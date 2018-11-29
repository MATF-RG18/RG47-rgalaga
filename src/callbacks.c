#include "include/callbacks.h"

void rgOnKeyboard(unsigned char key, int x, int y){
    switch(key) {
        case RG_ESCAPE:
            exit(0);
            break;
        case RG_KEY_A:
            rgPlayerMove(RG_KEY_A);
            break;
        case RG_KEY_D:
            rgPlayerMove(RG_KEY_D);
            break;
    }
}

void rgOnReshape(int width, int height){
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 10);
    
    glLoadIdentity();
    gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
}

void rgRenderScene(void){
    GLint width = glutGet(GLUT_WINDOW_WIDTH);
    GLint height = glutGet(GLUT_WINDOW_HEIGHT);

    /* Clear color and depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 10);

    /* Podesava se vidna tacka. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 1, 0, 0, -1, 0, 1, 0);
    
    /* Generate random number of stars to random positions */
    srand(time(NULL));
    int numOfStars = rand() % RG_MAX_STARS;
    int i;
    for (i = 0; i < numOfStars; i++){
        int x_pos = rand() % width;
        int y_pos = rand() % height;
        int z_pos = -1;
        
        glLoadIdentity();
        
        printf("x: %f, y: %f\n", -1 + 2.0 * x_pos / width, -1 + 2.0 * y_pos / height);
        glColor3ub(255, 251, 211);
        glPushMatrix();
            glTranslatef(-1 + 2.0 * x_pos / width, -1 + 2.0 * y_pos / height, z_pos);
            glutSolidSphere(0.003, 50, 50);       
        glPopMatrix();
    }

    rgRenderPlayer();

    glutSwapBuffers();
}
