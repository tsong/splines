#include "glutils.h"
#include "vector.h"

#define TIME_STEP 5E-3

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


void drawBSpline(const vector<Vector2f> &points, const vector<float> &u) {
    uint k = 3;
    uint n = points.size();

    //draw each segment
    for (uint i = 0; i + k-1 < n; i++) {
        glBegin(GL_LINE_STRIP);

        float t = u[i + k - 1];
        while (t <= u[i + k]) { //go from u[x] to u[x + 1] where x is the current knot
            Vector2f p(0,0);
            for (uint j = 0; j < k; j++) {
                float res = deBoorCox(u, i+j, k, t);

                p[0] += points[i+j].get(0)*res;
                p[1] += points[i+j].get(1)*res;
            }

            glVertex2f(p[0],p[1]);
            t += TIME_STEP;
        }
        glEnd();
    }
}

float deBoorCox(const vector<float> &u, uint i, uint k, float t) {
    if (k == 1) {
        return t >= u[i] && t < u[i+1] ? 1 : 0; //strict lower bound???
    }

    float N1 = deBoorCox(u, i,k-1,t);
    float N2 = deBoorCox(u, i+1,k-1,t);

    float a = u[i+k-1] - u[i] != 0 ? (t-u[i])/(u[i+k-1]-u[i]) : 0;
    float b = u[i+k] - u[i+1] != 0 ? (u[i+k]-t)/(u[i+k]-u[i+1]) : 0;

    return a*N1 + b*N2;
}
