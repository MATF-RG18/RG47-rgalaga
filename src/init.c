#include "include/init.h"
#include "include/callbacks.h"
 
void rgInitGLUT(int *argc, char **argv){
    /* GLUT initialization */
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
}

void rgInitCallbacks(){
    /* Callback functions registration */
    glutDisplayFunc(rgRenderScene);
    glutKeyboardFunc(rgOnKeyboard);
    glutReshapeFunc(rgOnReshape);
}

void rgInitWindow(){
    /* Window creation */
    glutInitWindowSize(RG_INITIAL_WINDOW_WIDTH, RG_INITIAL_WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(RG_APP_NAME);
}


