#ifndef POINTSUTIL_H
#define POINTSUTIL_H

#include "matrix.h"
#include "vector.h"

//utility functions for points
uint hash(uint i, uint j);
void unhash(uint h, uint &u, uint &v);
bool inCircle(Vector2f a, Vector2f b, Vector2f c, Vector2f d);
bool inTriangle(Vector2f a, Vector2f b, Vector2f c, Vector2f d);
int pointLineTest (Vector2f a, Vector2f b, Vector2f p);

#endif // POINTSUTIL_H
