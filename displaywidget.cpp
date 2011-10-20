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

void BSpline(vector<Vector2f> points);
float N(uint i, uint k, float t);

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

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0x1111);
    glBegin(GL_LINE_STRIP);
    for (uint i = 0; i < m_controlPoints.size(); i++) {
        Vector2f v = m_controlPoints[i];
        glVertex2f(v[0],v[1]);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    /*float STEP = 1E-3;
    for (uint i = 3; i < m_controlPoints.size(); i += 3) {
        vector<Vector2f> &CP = m_controlPoints;
        Vector2f P[] = {CP[i-3], CP[i-2], CP[i-1], CP[i-0]};

        glBegin(GL_LINE_STRIP);
        glVertex2f(P[0][0],P[0][1]);
        glVertex2f(P[1][0],P[1][1]);
        glVertex2f(P[2][0],P[2][1]);
        glVertex2f(P[3][0],P[3][1]);
        glEnd();

        glBegin(GL_LINE_STRIP);
        float t = 0;
        while (t <= 1) {
            float x = P[0][0]*(1-t)*(1-t)*(1-t) + P[1][0]*3*t*(1-t)*(1-t) + P[2][0]*3*t*t*(1-t) + P[3][0]*t*t*t;
            float y = P[0][1]*(1-t)*(1-t)*(1-t) + P[1][1]*3*t*(1-t)*(1-t) + P[2][1]*3*t*t*(1-t) + P[3][1]*t*t*t;

            glVertex2f(x,y);
            t += STEP;
        }
        glEnd();

    }*/

    BSpline(m_controlPoints);
}

float u[10000];

void BSpline(vector<Vector2f> points) {
    for (uint i = 0; i < 10000; i++) {
        u[i] = i;
    }

    uint k = 3;
    uint n = points.size();
    float STEP = 5E-3;

    //draw each segment
    for (uint i = 0; i + k-1 < n; i++) {
        glBegin(GL_LINE_STRIP);

        float t = i + k - 1;
        while (t <= i + k) { //go from u[x] to u[x + 1] where x is the current knot
            Vector2f p(0,0);
            for (uint j = 0; j < k; j++) {
                float res = N(i+j, k, t);
                p[0] += points[i+j][0]*res;
                p[1] += points[i+j][1]*res;
            }

            glVertex2f(p[0],p[1]);
            t += STEP;
        }
        glEnd();
    }
}

float N(uint i, uint k, float t) {
    if (k == 1) {
        return t >= u[i] && t < u[i+1] ? 1 : 0; //strict lower bound???
    }

    float N1 = N(i,k-1,t);
    float N2 = N(i+1,k-1,t);

    return (t-u[i])/(u[i+k-1]-u[i])*N1 + (u[i+k]-t)/(u[i+k]-u[i+1])*N2;
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

void DisplayWidget::clear() {
    m_undoStack->push(new ClearCommand(*this));
    repaint();
}
