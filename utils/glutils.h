#ifndef GLUTILS_H
#define GLUTILS_H

#include <QGLWidget>
#include "vector.h"

using namespace std;

#define PRECISION 360
#define PI 3.14159265

//returns a pseudo-random color given a seed
//the output is deterministic if the seed is the same
Vector3f randColor3f(uint seed);

//draws a circle at (x,y) with radius r using OpenGL
void glDrawCircle(float x, float y, float r);

//creates and returns a display list of a circle with radius r
GLuint glCircleList(float r);


#endif // GLUTILS_H
