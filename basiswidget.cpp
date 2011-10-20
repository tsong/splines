#include "basiswidget.h"
#include "glutils.h"
#include <QDebug>

#define TIME_STEP 1E-3
#define CONTROL_LENGTH 0.15
#define PADDING 0.05

BasisWidget::BasisWidget(QWidget *parent) :
    QGLWidget(parent), m_order(1)
{
    for (uint i = 0; i < 10; i++) {
        m_knots.push_back(i);
    }
    m_order = 4;
}

BasisWidget::~BasisWidget() {}


void BasisWidget::initializeGL() {
    //set background color to white
    //glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearColor(0,0,0,0);

    //nice looking anti-aliased lines
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    //glLineWidth(0.75);
}

void BasisWidget::resizeGL(int, int) {
    updateProjection();
}

float N(const vector<float> &u, uint i, uint k, float t) {
    if (k == 1) {
        return t >= u[i] && t < u[i+1] ? 1 : 0; //strict lower bound???
    }

    float N1 = N(u, i,k-1,t);
    float N2 = N(u, i+1,k-1,t);

    return (t-u[i])/(u[i+k-1]-u[i])*N1 + (u[i+k]-t)/(u[i+k]-u[i+1])*N2;
}

void BasisWidget::paintGL() {
    //clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

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
            glVertex2f(t, N(m_knots, i, m_order, t) + 2*PADDING + CONTROL_LENGTH);
            t += TIME_STEP;
        }
        glEnd();

        glColor3f(1,1,1);
    }

    //draw knot controls
    for (uint i = 0; i < m_knots.size(); i++) {
        uint multiplicity = 0;
        while (i < m_knots.size() - 1 && m_knots[i+1] == m_knots[i]) {
            multiplicity++;
            i++;
        }

        glBegin(GL_TRIANGLES);
        glVertex2f(m_knots[i], CONTROL_LENGTH + PADDING);
        glVertex2f(m_knots[i] + CONTROL_LENGTH, PADDING);
        glVertex2f(m_knots[i] - CONTROL_LENGTH, PADDING);
        glEnd();

        if (multiplicity > 0) {
            glColor3f(1,0,0);
            renderText(m_knots[i] - CONTROL_LENGTH + CONTROL_LENGTH/2, PADDING, 0, "99", QFont("Monospace"));
            glColor3f(1,1,1);
        }
    }
}

void BasisWidget::updateProjection() {
    float left = m_knots.front() - CONTROL_LENGTH - PADDING;
    float right = m_knots.back() + CONTROL_LENGTH + PADDING;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width(), height());

    //use an orthographic projection in the same dimensions as the draw surface
    //glOrtho(0, width, height, 0, -1, 0);
    glOrtho(left, right, 0, 1 + 2*PADDING + CONTROL_LENGTH, -1, 1);

    glMatrixMode(GL_MODELVIEW);
}

void BasisWidget::setOrder(uint order) {
    m_order = order;
}

void BasisWidget::setKnots(vector<float> knots) {
    m_knots = knots;
    updateProjection();
}

void BasisWidget::addKnot(float knot) {
    m_knots.push_back(knot);
    updateProjection();
}

bool BasisWidget::removeKnot(uint position) {
    if (position >= m_knots.size())
        return false;

    m_knots.erase(m_knots.begin() + position);
    updateProjection();
    return true;
}

bool BasisWidget::moveKnot(uint position, float newKnot) {
    if (position >= m_knots.size())
        return false;

    m_knots[position] = newKnot;
    updateProjection();
    return true;
}
