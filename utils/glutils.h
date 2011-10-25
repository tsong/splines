#ifndef GLUTILS_H
#define GLUTILS_H

#include <QGLWidget>
#include "vector"
#include "vector.h"

using namespace std;

#define PRECISION 360
#define PI 3.14159265

//draws a circle at (x,y) with radius r using OpenGL
void glDrawCircle(float x, float y, float r);

//creates and returns a display list of a circle with radius r
GLuint glCircleList(float r);


#endif // GLUTILS_H
