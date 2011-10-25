#include "basiswidget.h"
#include "utils/glutils.h"
#include "utils/pointutils.h"
#include "commands/moveknotcommand.h"
#include "commands/resetknotscommand.h"
#include <QDebug>

#define CONTROL_LENGTH 0.13
#define PADDING 0.05

BasisWidget::BasisWidget(QWidget *parent, QUndoStack *undoStack, BSpline *spline) :
    QGLWidget(parent), m_undoStack(undoStack), m_spline(spline),
    m_splineCreated(false), m_isKnotSelected(false), m_lastKnotSize(0)
{
    if (!undoStack) {
        undoStack = new QUndoStack(this);
    }
    connect(m_undoStack, SIGNAL(indexChanged(int)), this, SLOT(updateProjection()));
    connect(m_undoStack, SIGNAL(indexChanged(int)), this, SLOT(repaint()));

    if (!m_spline) {
        m_spline = new BSpline(3);
        m_splineCreated = true;
    }
}

BasisWidget::~BasisWidget() {
    if (m_splineCreated) {
        delete m_spline;
    }
}


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
    m_lastKnotSize = 0;
    updateProjection();
}


void BasisWidget::paintGL() {
    //clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //draw knot controls
    const vector<float> &knots = m_spline->getKnots();
    for (uint i = 0; i < knots.size(); i++) {
        uint multiplicity = 1;
        while (i < knots.size() - 1 && knots[i+1] == knots[i]) {
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

        if (multiplicity > 1) {
            glColor3f(1,0,0);
            renderText(v2[0], v2[1] + PADDING, 0, QString("%1").arg(multiplicity), QFont("Monospace"));
        }

        glColor3f(1,1,1);
    }

    //translate and draw basis functions
    glTranslatef(0, 2*PADDING + CONTROL_LENGTH, 0);
    m_spline->glDrawBasis();
}

void BasisWidget::reset() {
    m_undoStack->push(new ResetKnotsCommand(*m_spline));
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

    const vector<float> &knots = m_spline->getKnots();
    for (uint i = 0; i < knots.size(); i++) {
        Vector2f v1,v2,v3;
        getControlTriangle(i,v1,v2,v3);

        if (inTriangle(v1,v2,v3,p)) {
            m_isKnotSelected = true;
            m_commandId = rand();
            if (event->button() == Qt::LeftButton) {
                m_selectedIndex = i;
            } else {
                float knot = knots[i];
                while (i+1 < knots.size() && knots[i+1] == knot) {
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

        const vector<float> &knots = m_spline->getKnots();

        float i = m_selectedIndex;
        float lowerBound = i == 0 ? -INFINITY : knots[i-1];
        float upperBound = i == knots.size()-1 ? INFINITY : knots[i+1];

        float newKnot = max(x, lowerBound);
        newKnot = min(newKnot, upperBound);

        //m_spline->moveKnot(i, newKnot);
        MoveKnotCommand *command = new MoveKnotCommand(m_commandId, i, newKnot, *m_spline);
        m_undoStack->push(command);
        repaint();
    }
}

void BasisWidget::mouseReleaseEvent(QMouseEvent *) {
    m_isKnotSelected = false;
    repaint();
}

void BasisWidget::updateProjection() {
    //do not update the projection if the number of knots did not change.
    uint currKnots = m_spline->numberOfKnots();
    if (currKnots == m_lastKnotSize) {
        return;
    }
    m_lastKnotSize = currKnots;

    makeCurrent();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const vector<float> &knots = m_spline->getKnots();

    if (knots.size() == 0)
        return;

    //m_leftProj = knots.front() - CONTROL_LENGTH - PADDING;
    //m_rightProj = knots.back() + CONTROL_LENGTH + PADDING;
    m_leftProj = -CONTROL_LENGTH - PADDING;
    m_rightProj = knots.size()-1 + CONTROL_LENGTH + PADDING;
    m_bottomProj = 0;
    m_topProj = 1 + 2*PADDING + CONTROL_LENGTH;

    glViewport(0, 0, width(), height());
    glOrtho(m_leftProj, m_rightProj, m_bottomProj, m_topProj, -1, 1);

    glMatrixMode(GL_MODELVIEW);
}

void BasisWidget::getControlTriangle(uint i, Vector2f &v1, Vector2f &v2, Vector2f &v3) {
    float knot = m_spline->getKnots()[i];

    v1 = Vector2f(knot, CONTROL_LENGTH + PADDING);
    v2 = Vector2f(knot + CONTROL_LENGTH, PADDING);
    v3 = Vector2f(knot - CONTROL_LENGTH, PADDING);
}


