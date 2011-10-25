#include "pointutils.h"


//tests a point p and line ab using cross product test
//returns 1 if p is clockwise to ab
//returns -1 if p is counter-clockwise to ab
//returns 0 if p is colinear
int pointLineTest (Vector2f a, Vector2f b, Vector2f p) {
    Vector2f v1 = b - a;
    Vector2f v2 = p - a;
    Vector2f c = v1.cross(v2);

    if (c[0] < 0)
        return -1;
    else if (c[0] > 0)
        return 1;
    else
        return 0;
}

//returns true if vertex d is in the circumcircle of triangle abc
bool inCircle(Vector2f a, Vector2f b, Vector2f c, Vector2f d) {
    Matrix4f M;

    Vector2f *vectors[4] = {&a, &b, &c, &d};
    for (uint i = 0; i < 4; i++) {
        Vector2f &v = *vectors[i];
        M[i] = Vector4f(v[0], v[1], v[0]*v[0] + v[1]*v[1], 1);
    }

    return M.det() > 0;
}

//returns true if vertex d is in triangle abc
bool inTriangle(Vector2f a, Vector2f b, Vector2f c, Vector2f d) {
    return pointLineTest(a,b,d) == -pointLineTest(a,c,d)
           && pointLineTest(b,c,d) == -pointLineTest(b,a,d)
           && pointLineTest(c,b,d) == -pointLineTest(c,a,d);
}

//hashes two vertex indices for map lookup
uint hash(uint i, uint j) {
    if (i > j) {
        uint tmp = i;
        i = j;
        j= tmp;
    }
    return (i << 16) | j;
}

//unhash a hash to two vertex indices
void unhash(uint h, uint &u, uint &v) {
    u = h >> 16;
    v = (h << 16) >> 16;
}

