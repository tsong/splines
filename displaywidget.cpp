#include <QDebug>
#include "displaywidget.h"
#include "glutils.h"

//#define ABS(x) (x < 0 ? -x : x)

DisplayWidget::DisplayWidget(QWidget *parent) :
    QGLWidget(parent), m_showControlPoints(true)
{
}

DisplayWidget::~DisplayWidget() {}

void DisplayWidget::initializeGL() {
    //set background color to white
    glClearColor(1.0, 1.0, 1.0, 1.0);

    //nice looking anti-aliased lines
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glLineWidth(0.75);
}

void DisplayWidget::resizeGL(int width, int height) {
    //change viewport to new dimensions
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);

    //use an orthographic projection in the same dimensions as the draw surface
    glOrtho(0, width, height, 0, -1, 0);

    glMatrixMode(GL_MODELVIEW);
}

void DisplayWidget::paintGL() {
    //clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //draw control points
    if (m_showControlPoints) {
        glColor3f(0,0,0);
        for (list<Vector2f>::iterator it = m_controlPoints.begin(); it != m_controlPoints.end(); it++) {
            Vector2f v = *it;
            if (m_selected && m_selectedIterator == it) {
                glColor3f(0,0,1);
                glDrawCircle(v[0], v[1], POINT_RADIUS);
                glColor3f(0,0,0);
            }else {
                glDrawCircle(v[0], v[1], POINT_RADIUS);
            }
        }
    }
}

void DisplayWidget::mousePressEvent(QMouseEvent *event) {
    float x = (float)event->x();
    float y = (float)event->y();

    //iterate through vertices and see if point clicked is inside a vertex
    m_selected = false;
    for (list<Vector2f>::iterator it = m_controlPoints.begin(); it != m_controlPoints.end(); it++) {
        Vector2f v = *it;
        if (abs(v[0]-x) <= POINT_RADIUS && abs(v[1]-y) <= POINT_RADIUS) {
            m_selected = true;
            m_selectedIterator = it;
            break;
        }
    }

    if (m_selected && event->button() == Qt::RightButton) {
        //remove vertex if right mouse button is clicked
        m_controlPoints.erase(m_selectedIterator);
        m_selected = false;
    } else if (!m_selected && event->button() == Qt::LeftButton) {
        //add a new control point if left mouse button is clicked
        Vector2f v(x,y);
        m_controlPoints.push_back(v);
    }

    repaint();
}

void DisplayWidget::mouseMoveEvent(QMouseEvent *event) {
    if (m_selected) {
        float x = (float)event->x();
        float y = (float)event->y();
        *m_selectedIterator = Vector2f(x,y);

        repaint();
    }
}

void DisplayWidget::mouseReleaseEvent(QMouseEvent *) {
    m_selected = false;
    repaint();
}

void DisplayWidget::clear() {
    m_controlPoints.clear();
}
