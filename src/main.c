#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

#define APP_NAME "RGalaga"
#define MAX_STARS 256
#define ESCAPE 27

int windowWidth = 700;
int windowHeight = 500;

/* Callback functions declarations */
static void renderScene(void);
static void onKeyboard(unsigned char key, int x, int y);
static void onReshape(int width, int height);

int main(int argc, char **argv)
{
    /* GLUT initialization */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    /* Window creation */
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(APP_NAME);

    /* Callback functions registration */
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(onKeyboard);
    glutReshapeFunc(onReshape);

    /* OpenGL initialization */
    glClearColor(0, 0, 0, 0);

    glutMainLoop();

    return 0;
}

static void onKeyboard(unsigned char key, int x, int y){
    switch(key) {
        case ESCAPE:
            exit(0);
            break;
    }
}

static void onReshape(int width, int height){
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 10);
}

static void renderScene(void)
{
    /* Clear color and depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_MODELVIEW); 
    
    /* Generate random number of stars to random positions */
    srand(time(NULL));
    int numOfStars = rand() % MAX_STARS;
    int i;
    for (i = 0; i < numOfStars; i++){
        int x_pos = rand() % windowWidth;
        int y_pos = rand() % windowHeight;
        int z_pos = -1;
        
        glLoadIdentity();
        
        printf("x: %f, y: %f\n", -1+2.0*x_pos/windowWidth, -1+2.0*y_pos/windowHeight);
        glColor3ub(255, 251, 211);
        glPushMatrix();
            glTranslatef(-1 + 2.0 * x_pos / windowWidth, -1 + 2.0 * y_pos / windowHeight, z_pos);
            glutSolidSphere(0.003, 50, 50);       
        glPopMatrix();
    }

    glutSwapBuffers();
}
