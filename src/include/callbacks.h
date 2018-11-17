#ifndef RG_CALLBACKS_H
#define RG_CALLBACKS_H

#include <GL/glut.h>
#include <time.h>

#include "init.h"
#include "constants.h"
#include "spacecraft.h"

void rgRenderScene(void);
void rgOnKeyboard(unsigned char key, int x, int y);
void rgOnReshape(int width, int height);

#endif
