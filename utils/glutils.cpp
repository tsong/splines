#include "glutils.h"
#include "vector.h"

#define PARAMETER_STEP 1E-2

void glDrawCircle(float x, float y, float r) {
    //translate to x,y
    glPushMatrix();
    glTranslatef(x,y,0);

    //divide circle into sections
    double angleDelta = 2.0*PI/PRECISION;
    double angle = 0;

    //draw points on the circle
    glBegin(GL_POLYGON);
       for (int i = 0; i < PRECISION; i++) {
            double x = r * cos(angle);
            double y = r * sin(angle);
            angle += angleDelta;

            glVertex2d(x,y);
        }
    glEnd();
    glPopMatrix();
}

GLuint glCircleList(float r) {
    GLuint listIndex = glGenLists(1);
    glNewList(listIndex, GL_COMPILE);
        glDrawCircle(0,0,r);
    glEndList();

    return listIndex;
}
