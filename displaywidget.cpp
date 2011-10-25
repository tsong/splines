#include <QDebug>
#include "displaywidget.h"
#include "utils/glutils.h"

DisplayWidget::DisplayWidget(QWidget *parent, QUndoStack *undoStack, BSpline *spline) :
    QGLWidget(parent), m_undoStack(undoStack),  m_spline(spline), m_splineCreated(false),
    m_showControlPoints(true), m_showControlLines(true), m_showSegments(false)
{
    if (!undoStack) {
        undoStack = new QUndoStack(this);
    }

    //each action will redraw the scene
    connect(m_undoStack, SIGNAL(indexChanged(int)), this, SLOT(repaint()));

    if (!spline) {
        m_spline = new BSpline();
        m_splineCreated = true;
    }
}

DisplayWidget::~DisplayWidget() {
    if (m_splineCreated) {
        delete m_spline;
    }
}

void DisplayWidget::initializeGL() {
    //set background color to white
    glClearColor(1.0, 1.0, 1.0, 1.0);

    //nice looking anti-aliased lines
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(1.25);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (m_showControlPoints) {
        m_spline->glDrawControlPoints(m_selected ? m_selectedIndex : -1);
    }

    if (m_showControlLines) {
        m_spline->glDrawControlLines();
    }

    m_spline->glDrawCurve(m_showSegments);
}

void DisplayWidget::mousePressEvent(QMouseEvent *event) {
    if (!m_showControlPoints)
        return;

    float x = (float)event->x();
    float y = (float)event->y();

    //iterate through vertices and see if point clicked is inside a vertex
    const vector<Vector2f> controlPoints = m_spline->getPoints();
    m_selected = false;
    for (uint i = 0; i < controlPoints.size(); i++) {
        Vector2f v = controlPoints[i];
        if (abs(v[0]-x) <= POINT_RADIUS && abs(v[1]-y) <= POINT_RADIUS) {
            m_selected = true;
            m_selectedIndex = i;
            break;
        }
    }

    if (m_selected && event->button() == Qt::RightButton) {
        //remove vertex if right mouse button is clicked
        DeletePointCommand *deletePointCommand = new DeletePointCommand(m_selectedIndex, *m_spline);
        m_undoStack->push(deletePointCommand);
        m_selected = false;
    } else if (!m_selected && event->button() == Qt::LeftButton) {
        //add a new control point if left mouse button is clicked
        Vector2f v(x,y);
        AddPointCommand *addPointCommand = new AddPointCommand(v, *m_spline);
        m_undoStack->push(addPointCommand);
    } else {
        //generate a new id for this point movement command
        m_moveCommandId = rand();
    }

    repaint();
}

void DisplayWidget::mouseMoveEvent(QMouseEvent *event) {
    //issue move command if a control point is currently selected
    if (m_selected) {
        float x = (float)event->x();
        float y = (float)event->y();
        Vector2f v(x,y);

        MovePointCommand *movePointCommand = new MovePointCommand(m_moveCommandId, m_selectedIndex, v, *m_spline);
        m_undoStack->push(movePointCommand);
        repaint();
    }
}

void DisplayWidget::mouseReleaseEvent(QMouseEvent *) {
    //unselect control point
    if (m_selected) {
        m_selected = false;
        repaint();
    }
}

void DisplayWidget::clear() {
    m_undoStack->push(new ClearCommand(*m_spline));
}

void DisplayWidget::DisplayWidget::toggleShowControlPoints() {
    m_undoStack->push(new ToggleCommand(m_showControlPoints));
}

void DisplayWidget::toggleShowControlLines() {
    m_undoStack->push(new ToggleCommand(m_showControlLines));
}

void DisplayWidget::toggleShowSegments() {
    m_undoStack->push(new ToggleCommand(m_showSegments));
}
