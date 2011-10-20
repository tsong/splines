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

//draws a B spline with
void drawBSpline(const vector<Vector2f> &points, const vector<float> &u);

float deBoorCox(const vector<float> &u, uint i, uint k, float t);



#endif // GLUTILS_H
