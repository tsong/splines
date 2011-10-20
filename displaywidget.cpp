#include <QDebug>
#include "displaywidget.h"
#include "glutils.h"

//#define ABS(x) (x < 0 ? -x : x)

DisplayWidget::DisplayWidget(QWidget *parent, QUndoStack *undoStack) :
    QGLWidget(parent), m_undoStack(undoStack), m_showControlPoints(true),
    m_showControlLines(true), m_order(3)
{
    if (!undoStack) {
        undoStack = new QUndoStack(this);
    }

    for (uint i = 0; i < 10000; i++) {
        m_knots.push_back(i);
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

    glLineWidth(0.75);

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
                glColor3f(1,0,0);
                glDrawCircle(v[0], v[1], POINT_RADIUS);
                glColor3f(0,0,0);
            }else {
                glDrawCircle(v[0], v[1], POINT_RADIUS);
            }
        }
    }

    //draw control lines
    if (m_showControlLines) {
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

    drawBSpline(m_controlPoints, m_knots, m_order);
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
    } else {
        //generate a new id for this point movement command
        m_moveCommandId = rand();
    }

    repaint();
}

void DisplayWidget::mouseMoveEvent(QMouseEvent *event) {
    if (m_selected) {
        float x = (float)event->x();
        float y = (float)event->y();
        Vector2f v(x,y);
        //m_controlPoints[m_selectedIndex] = Vector2f(x,y);

        MovePointCommand *movePointCommand = new MovePointCommand(m_moveCommandId, m_selectedIndex, v, *this);
        m_undoStack->push(movePointCommand);
        repaint();
    }
}

void DisplayWidget::mouseReleaseEvent(QMouseEvent *) {
    m_selected = false;
    repaint();
}
void DisplayWidget::setOrder(uint order) {
    if (order > 0)
        m_order = order;
    repaint();
}

void DisplayWidget::clear() {
    m_undoStack->push(new ClearCommand(*this));
    emit pointsChanged(m_controlPoints);
    repaint();
}

void DisplayWidget::setKnots(const vector<float> &knots) {
    m_knots = knots;
    repaint();
}

void DisplayWidget::insertPoint(uint i, Vector2f point) {
    m_controlPoints.insert(m_controlPoints.begin() + i, point);
    emit pointsChanged(m_controlPoints);
    repaint();
}

void DisplayWidget::deletePoint(uint i) {
    m_controlPoints.erase(m_controlPoints.begin() + i);
    emit pointsChanged(m_controlPoints);
    repaint();
}

void DisplayWidget::movePoint(uint i, Vector2f point) {
    m_controlPoints[i] = point;
    //emit pointsChanged(m_controlPoints);
    repaint();
}

void DisplayWidget::clearAllPoints() {
    m_controlPoints.clear();
    emit pointsChanged(m_controlPoints);
    repaint();
}

void DisplayWidget::setPoints(vector<Vector2f> points) {
    m_controlPoints = points;
    emit pointsChanged(m_controlPoints);
    repaint();
}

uint DisplayWidget::numberOfPoints() {
    return m_controlPoints.size();
}

