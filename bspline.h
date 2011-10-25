#ifndef BSPLINE_H
#define BSPLINE_H

#include <vector>
#include <QGLWidget>
#include "utils/vector.h"

using namespace std;
class BSpline {
public:
    BSpline(uint order=3);
    ~BSpline();

    /*control point methods*/
    const vector<Vector2f>& getPoints();
    void insertPoint(uint i, Vector2f point);
    void deletePoint(uint i);
    void movePoint(uint i, Vector2f point);
    void clearAllPoints();
    void setPoints(vector<Vector2f> points);
    uint numberOfPoints();

    /*knots methods*/
    const vector<float>& getKnots();
    void setKnots(const vector<float> &knots);
    bool moveKnot(uint position, float newKnot);
    void resetKnots();
    uint numberOfKnots();

    /*order methods*/
    void setOrder(uint order);
    uint getOrder();

    /*OpenGL draw methods*/
    void glDrawControlPoints(int selectedIndex = -1);
    void glDrawControlLines();
    void glDrawCurve(bool showSegments = false);
    void glDrawBasis(bool showSegments = false);

protected:
    //altering knots should only be done internally, since it may
    //corrupt the state of the spline
    void addKnot(float knot);
    bool removeKnot(uint position);

    //initialize knots from initial control points
    void createKnots();

protected:
    vector<Vector2f> m_controlPoints;
    vector<float> m_knots;
    uint m_order;
};

#endif // BSPLINE_H
