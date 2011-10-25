#ifndef BSPLINE_H
#define BSPLINE_H

#include <vector>
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

    void setOrder(uint order);
    const vector<float>& getKnots();
    bool moveKnot(uint position, float newKnot);

    void glDrawControlPoints(int selectedIndex = -1);
    void glDrawControlLines();
    void glDrawCurve();

protected:
    void setKnots(const vector<float> &knots);
    void addKnot(float knot);
    bool removeKnot(uint position);
    void createKnots(const vector<Vector2f> &controlPoints);

protected:
    vector<Vector2f> m_controlPoints;
    vector<float> m_knots;
    uint m_order;
};

#endif // BSPLINE_H
