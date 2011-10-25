#ifndef BSPLINE_H
#define BSPLINE_H

#include <vector>
#include "utils/vector.h"

using namespace std;
class BSpline {
public:
    BSpline();

protected:
    vector<Vector2f> m_controlPoints;
};

#endif // BSPLINE_H
