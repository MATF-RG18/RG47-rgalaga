#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include "include/init.h"
#include "include/constants.h"
#include "include/spacecraft.h"
#include "include/callbacks.h"

int main(int argc, char **argv)
{
    rgInitGLUT(&argc, argv);
    rgInitWindow();
    rgInitCallbacks();

    /* OpenGL initialization */
    glClearColor(0, 0, 0, 0);

    glutMainLoop();

    return 0;
}

