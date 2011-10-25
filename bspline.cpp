#include "bspline.h"
#include "QGLWidget"
#include "utils/glutils.h"

#define CONTROL_POINT_RADIUS 6
#define PARAMETER_STEP 1E-3

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

BSpline::BSpline(uint order)
    : m_order(order)
{
}

BSpline::~BSpline() {}


void BSpline::glDrawControlPoints(int selectedIndex) {
    bool isSelected = selectedIndex > 0 && selectedIndex < static_cast<int>(numberOfPoints());
    uint index = static_cast<uint>(selectedIndex);

    //draw control points
    glColor3f(0,0,0);
    for (uint i = 0; i < m_controlPoints.size(); i++) {
        Vector2f v = m_controlPoints[i];
        if (isSelected && index == i) {
            glColor3f(1,0,0);
            glDrawCircle(v[0], v[1], CONTROL_POINT_RADIUS);
            glColor3f(0,0,0);
        } else {
            glDrawCircle(v[0], v[1], CONTROL_POINT_RADIUS);
        }
    }
}

void BSpline::glDrawControlLines() {
    glColor3f(0,0,0);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0x1111);
    glBegin(GL_LINE_STRIP);
    for (uint i = 0; i < m_controlPoints.size(); i++) {
        Vector2f v = m_controlPoints[i];
        glVertex2f(v[0],v[1]);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void BSpline::glDrawCurve(bool showSegments) {
    glColor3f(0,0,0);
    const vector<float> &u = m_knots;
    uint k = m_order;
    uint n = m_controlPoints.size();

    //draw each segment
    glBegin(GL_LINE_STRIP);
    for (uint i = 0; i + k-1 < n; i++) {
        //use a pseudo random color to show each segment
        if (showSegments) {
            Vector3f color = randColor3f(i);
            glColor3f(color[0], color[1], color[2]);
        }

        float t = u[i + k - 1];
        //go from u[x] to u[x + 1] where x is the current knot
        while (t <= u[i + k]) {
            Vector2f p(0,0);
            //blend the control points with each basis function
            for (uint j = 0; j < k; j++) {
                float res = deBoorCox(u, i+j, k, t);

                p[0] += m_controlPoints[i+j].get(0)*res;
                p[1] += m_controlPoints[i+j].get(1)*res;
            }

            //draw point and advance the parameter
            glVertex2f(p[0],p[1]);
            t += PARAMETER_STEP;
        }
    }
    glEnd();
}

void BSpline::glDrawBasis(bool showSegments) {
    if (m_knots.size() < m_order) return;

    //bounds of the parametric space
    float lowerBound = m_knots[m_order-1];
    float upperBound = m_knots[m_knots.size() - m_order];

    //draw each basis function
    for (uint i = 0; i < m_knots.size() - m_order; i++) {
        float t = m_knots[i];

        //set initial segment and segment color
        uint segment = i;
        Vector3f color = showSegments ? randColor3f(segment-m_order+1) : Vector3f(1,0,0);

        glBegin(GL_LINE_STRIP);
        while (t <= m_knots[i+m_order]) {
            //check what segment we are currently in and update color
            if (showSegments) {
                if (t >= m_knots[segment+1]) {
                    segment++;
                    color = randColor3f(segment-m_order+1);
                }
            }

            //update the color
            if (t >= lowerBound && t <= upperBound) {
                glColor3f(color[0],color[1],color[2]);
            } else {
                glColor3f(1,1,1);
            }

            //draw point and advance parameter
            glVertex2f(t, deBoorCox(m_knots, i, m_order, t));
            t += PARAMETER_STEP;
        }
        glEnd();

        glColor3f(1,1,1);
    }
}

void BSpline::setOrder(uint order) {
    if (order > 0) {
        m_order = order;
        createKnots();
    }
}

uint BSpline::getOrder() {
    return m_order;
}

void BSpline::setKnots(const vector<float> &knots) {
    if (knots.size() == m_knots.size())
        m_knots = knots;
}

const vector<Vector2f>& BSpline::getPoints() {
    return m_controlPoints;
}

void BSpline::insertPoint(uint i, Vector2f point) {
    m_controlPoints.insert(m_controlPoints.begin() + i, point);
    if (numberOfKnots() > 0) {
        addKnot(m_knots.back()+1);
    } else {
        createKnots();
    }
}

void BSpline::deletePoint(uint i) {
    m_controlPoints.erase(m_controlPoints.begin() + i);
    if (numberOfPoints() > 0)
        removeKnot(m_knots.size()-1);
    else
        m_knots.clear();
}

void BSpline::movePoint(uint i, Vector2f point) {
    m_controlPoints[i] = point;
}

void BSpline::clearAllPoints() {
    m_controlPoints.clear();
    createKnots();

}

void BSpline::setPoints(vector<Vector2f> points) {
    m_controlPoints = points;
    createKnots();
}

uint BSpline::numberOfPoints() {
    return m_controlPoints.size();
}

void BSpline::addKnot(float knot) {
    m_knots.push_back(knot);
}

bool BSpline::removeKnot(uint position) {
    if (position >= m_knots.size())
        return false;

    m_knots.erase(m_knots.begin() + position);
    return true;
}

void BSpline::resetKnots() {
    for (uint i = 0; i < m_knots.size(); i++) {
        m_knots[i] = i;
    }
}

const vector<float>& BSpline::getKnots() {
    return m_knots;
}

bool BSpline::moveKnot(uint position, float newKnot) {
    if (position >= m_knots.size())
        return false;

    m_knots[position] = newKnot;
    return true;
}

uint BSpline::numberOfKnots() {
    return m_knots.size();
}


void BSpline::createKnots() {
    m_knots.clear();

    //initialize with n+k knots, where n is the number
    //of control points and k is the order
    if (m_controlPoints.size() > 0) {
        for (uint i = 0; i < m_controlPoints.size() + m_order; i++) {
            m_knots.push_back(i);
        }
    }
}
