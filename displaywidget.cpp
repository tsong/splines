#include <QDebug>
#include "displaywidget.h"
#include "glutils.h"

//#define ABS(x) (x < 0 ? -x : x)

DisplayWidget::DisplayWidget(QWidget *parent, QUndoStack *undoStack) :
    QGLWidget(parent), m_undoStack(undoStack), m_showControlPoints(true)
{
    if (!undoStack) {
        undoStack = new QUndoStack(this);
    }
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
        for (uint i = 0; i < m_controlPoints.size(); i++) {
            Vector2f v = m_controlPoints[i];
            if (m_selected && m_selectedIndex == i) {
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
    for (uint i = 0; i < m_controlPoints.size(); i++) {
        Vector2f v = m_controlPoints[i];
        if (abs(v[0]-x) <= POINT_RADIUS && abs(v[1]-y) <= POINT_RADIUS) {
            m_selected = true;
            m_selectedIndex = i;
            break;
        }
    }

    if (m_selected && event->button() == Qt::RightButton) {
        //remove vertex if right mouse button is clicked
        //m_controlPoints.erase(m_selectedIterator);
        DeletePointCommand *deletePointCommand = new DeletePointCommand(m_selectedIndex, *this);
        m_undoStack->push(deletePointCommand);
        m_selected = false;
    } else if (!m_selected && event->button() == Qt::LeftButton) {
        //add a new control point if left mouse button is clicked
        Vector2f v(x,y);
        AddPointCommand *addPointCommand = new AddPointCommand(v, *this);
        m_undoStack->push(addPointCommand);
    }

    repaint();
}

void DisplayWidget::mouseMoveEvent(QMouseEvent *event) {
    if (m_selected) {
        float x = (float)event->x();
        float y = (float)event->y();
        m_controlPoints[m_selectedIndex] = Vector2f(x,y);

        repaint();
    }
}

void DisplayWidget::mouseReleaseEvent(QMouseEvent *) {
    m_selected = false;
    repaint();
}

void DisplayWidget::clear() {
    m_controlPoints.clear();
    repaint();
}
