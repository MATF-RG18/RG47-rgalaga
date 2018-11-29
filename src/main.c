#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include "include/init.h"
#include "include/constants.h"
#include "include/spacecraft.h"
#include "include/callbacks.h"

#define RG_APP_NAME "RGalaga"
#define RG_MAX_STARS 256
#define RG_ESCAPE 27
#define RG_ARROW_LEFT 97
#define RG_ARROW_RIGHT 100
#define RG_INITIAL_WINDOW_WIDTH 500
#define RG_INITIAL_WINDOW_HEIGHT 500

int main(int argc, char **argv)
{
    rgInitGLUT(&argc, argv);
    rgInitWindow();
    rgInitCallbacks();
    rgInitEnableOptions();

    glutMainLoop();

    return 0;
}

