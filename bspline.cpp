#include "bspline.h"
#include "QGLWidget"
#include "utils/glutils.h"

#define TIME_STEP 1E-3

BSpline::BSpline(uint order)
    : m_order(order)
{
}

BSpline::~BSpline() {}


void BSpline::glDrawControlPoints(int selectedIndex) {
    bool isSelected = selectedIndex > 0 && selectedIndex < static_cast<int>(numberOfPoints());

    //draw control points
    glColor3f(0,0,0);
    for (uint i = 0; i < m_controlPoints.size(); i++) {
        Vector2f v = m_controlPoints[i];
        if (isSelected && static_cast<uint>(selectedIndex) == i) {
            glColor3f(1,0,0);
            glDrawCircle(v[0], v[1], 5);
            glColor3f(0,0,0);
        } else {
            glDrawCircle(v[0], v[1], 5);
        }
    }
}

void BSpline::glDrawControlLines() {
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

void BSpline::glDrawCurve() {
    drawBSpline(m_controlPoints, m_knots, m_order);
}

void BSpline::glDrawBasis() {
    if (m_knots.size() < m_order) return;

    //bounds of the parametric space
    float lowerBound = m_knots[m_order-1];
    float upperBound = m_knots[m_knots.size() - m_order];

    //draw each basis function
    for (uint i = 0; i < m_knots.size() - m_order; i++) {
        float t = m_knots[i];

        glBegin(GL_LINE_STRIP);
        while (t <= m_knots[i+m_order]) {
            if (t >= lowerBound && t <= upperBound) {
                glColor3f(1,0,0);
            } else {
                glColor3f(1,1,1);
            }
            glVertex2f(t, deBoorCox(m_knots, i, m_order, t));
            t += TIME_STEP;
        }
        glEnd();

        glColor3f(1,1,1);
    }
}



void BSpline::setOrder(uint order) {
    if (order > 0)
        m_order = order;
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
        createKnots(m_controlPoints);
    }
}

void BSpline::deletePoint(uint i) {
    m_controlPoints.erase(m_controlPoints.begin() + i);
    removeKnot(m_knots.size()-1);
}

void BSpline::movePoint(uint i, Vector2f point) {
    m_controlPoints[i] = point;
}

void BSpline::clearAllPoints() {
    m_controlPoints.clear();
    createKnots(m_controlPoints);

}

void BSpline::setPoints(vector<Vector2f> points) {
    m_controlPoints = points;
    createKnots(m_controlPoints);
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


void BSpline::createKnots(const vector<Vector2f> &controlPoints) {
    m_knots.clear();

    if (controlPoints.size() > 0) {
        for (uint i = 0; i < controlPoints.size() + m_order; i++) {
            m_knots.push_back(i);
        }
    }
}
