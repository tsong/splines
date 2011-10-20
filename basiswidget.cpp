#include "basiswidget.h"
#include "glutils.h"
#include "pointsutil.h"
#include <QDebug>

#define TIME_STEP 1E-3
#define CONTROL_LENGTH 0.13
#define PADDING 0.05

BasisWidget::BasisWidget(QWidget *parent) :
    QGLWidget(parent), m_order(1), m_isKnotSelected(false)
{
    for (uint i = 0; i < 10; i++) {
        m_knots.push_back(i);
    }
    m_order = 3;
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


void BasisWidget::paintGL() {
    //clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

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
            glVertex2f(t,deBoorCox(m_knots, i, m_order, t) + 2*PADDING + CONTROL_LENGTH);
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

        Vector2f v1,v2,v3;
        getControlTriangle(i, v1, v2, v3);

        if (m_isKnotSelected && i == m_selectedIndex)
            glColor3f(1,0,0);

        glBegin(GL_TRIANGLES);
        glVertex2f(v1[0], v1[1]);
        glVertex2f(v2[0], v2[1]);
        glVertex2f(v3[0], v3[1]);
        glEnd();

        if (multiplicity > 0) {
            glColor3f(0,0,0);
            //renderText(m_knots[i] - CONTROL_LENGTH + CONTROL_LENGTH/2, PADDING, 0, "99", QFont("Monospace"));
            renderText(v2[0] + CONTROL_LENGTH/2, v2[1], 0, "99", QFont("Monospace"));
        }

        glColor3f(1,1,1);
    }
}

void BasisWidget::convertCoordinates(float xIn, float yIn, float &xOut, float &yOut) {
    float ratioX = xIn / (float)width();
    float ratioY = yIn / (float)height();


    xOut = m_leftProj + (m_rightProj-m_leftProj)*ratioX;
    yOut = m_topProj - (m_topProj-m_bottomProj)*ratioY;
}

/*mouse events*/
void BasisWidget::mousePressEvent(QMouseEvent *event) {
    Vector2f p;
    convertCoordinates(event->x(), event->y(), p[0], p[1]);

    for (uint i = 0; i < m_knots.size(); i++) {
        Vector2f v1,v2,v3;
        getControlTriangle(i,v1,v2,v3);

        if (inTriangle(v1,v2,v3,p)) {
            m_isKnotSelected = true;
            if (event->button() == Qt::LeftButton) {
                m_selectedIndex = i;
            } else {
                float knot = m_knots[i];
                while (i+1 < m_knots.size() && m_knots[i+1] == knot) {
                    i++;
                }
                m_selectedIndex = i;
            }
            break;
        }
    }

    repaint();
}

void BasisWidget::mouseMoveEvent(QMouseEvent *event) {
    if (m_isKnotSelected) {
        float x,y;
        convertCoordinates(event->x(), event->y(), x, y);

        float i = m_selectedIndex;
        float lowerBound = i == 0 ? -INFINITY : m_knots[i-1];
        float upperBound = i == m_knots.size()-1 ? INFINITY : m_knots[i+1];

        float newKnot = max(x, lowerBound);
        newKnot = min(newKnot, upperBound);

        moveKnot(i, newKnot);
        emit knotsChanged(m_knots);
        repaint();
    }
}

void BasisWidget::mouseReleaseEvent(QMouseEvent *) {
    m_isKnotSelected = false;
    repaint();
}

void BasisWidget::updateProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (m_knots.size() == 0)
        return;

    m_leftProj = m_knots.front() - CONTROL_LENGTH - PADDING;
    m_rightProj = m_knots.back() + CONTROL_LENGTH + PADDING;
    m_bottomProj = 0;
    m_topProj = 1 + 2*PADDING + CONTROL_LENGTH;

    glViewport(0, 0, width(), height());
    glOrtho(m_leftProj, m_rightProj, m_bottomProj, m_topProj, -1, 1);

    glMatrixMode(GL_MODELVIEW);
}

void BasisWidget::getControlTriangle(uint i, Vector2f &v1, Vector2f &v2, Vector2f &v3) {
    v1 = Vector2f(m_knots[i], CONTROL_LENGTH + PADDING);
    v2 = Vector2f(m_knots[i] + CONTROL_LENGTH, PADDING);
    v3 = Vector2f(m_knots[i] - CONTROL_LENGTH, PADDING);
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
    return true;
}
