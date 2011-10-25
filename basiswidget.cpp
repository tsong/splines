#include "basiswidget.h"
#include "utils/glutils.h"
#include "utils/pointutils.h"
#include "commands/moveknotcommand.h"
#include "commands/resetknotscommand.h"
#include "commands/togglecommand.h"
#include <QDebug>

#define CONTROL_LENGTH 0.13
#define PADDING 0.05

BasisWidget::BasisWidget(QWidget *parent, QUndoStack *undoStack, BSpline *spline) :
    QGLWidget(parent), m_undoStack(undoStack), m_spline(spline),
    m_splineCreated(false), m_isKnotSelected(false), m_lastKnotSize(0),
    m_showSegments(false)
{
    if (!undoStack) {
        undoStack = new QUndoStack(this);
    }

    //If any action occurs, resize the projection and redrawn the scene
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
    //set background color
    glClearColor(0,0,0,0);

    //nice looking anti-aliased lines
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(1.25);
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
        //calculate multiplicity of the knot
        uint multiplicity = 1;
        while (i < knots.size() - 1 && knots[i+1] == knots[i]) {
            multiplicity++;
            i++;
        }

        //get coordinates of the control triangle
        Vector2f v1,v2,v3;
        getControlTriangle(i, v1, v2, v3);

        //color red if control triangle is currently selected
        if (m_isKnotSelected && i == m_selectedIndex)
            glColor3f(1,0,0);

        //draw control triangle
        glBegin(GL_TRIANGLES);
        glVertex2f(v1[0], v1[1]);
        glVertex2f(v2[0], v2[1]);
        glVertex2f(v3[0], v3[1]);
        glEnd();

        //display multiplicity when there is ambiguity
        if (multiplicity > 1) {
            glColor3f(1,0,0);
            renderText(v2[0], v2[1] + PADDING, 0, QString("%1").arg(multiplicity), QFont("Monospace"));
        }

        glColor3f(1,1,1);
    }

    //translate and draw basis functions
    glTranslatef(0, 2*PADDING + CONTROL_LENGTH, 0);
    m_spline->glDrawBasis(m_showSegments);
}

void BasisWidget::reset() {
    m_undoStack->push(new ResetKnotsCommand(*m_spline));
}

void BasisWidget::toggleShowSegments() {
    m_undoStack->push(new ToggleCommand(m_showSegments));
}

void BasisWidget::convertCoordinates(float xIn, float yIn, float &xOut, float &yOut) {
    //ratio of screen coordinates to screen dimensions
    float ratioX = xIn / (float)width();
    float ratioY = yIn / (float)height();

    //scale based on calculated ratio
    xOut = m_leftProj + (m_rightProj-m_leftProj)*ratioX;
    yOut = m_topProj - (m_topProj-m_bottomProj)*ratioY;
}

/*mouse events*/
void BasisWidget::mousePressEvent(QMouseEvent *event) {
    Vector2f p;
    convertCoordinates(event->x(), event->y(), p[0], p[1]);

    //check if a control triangle is selected
    const vector<float> &knots = m_spline->getKnots();
    for (uint i = 0; i < knots.size(); i++) {
        //get dimension of ith control triangle
        Vector2f v1,v2,v3;
        getControlTriangle(i,v1,v2,v3);

        if (inTriangle(v1,v2,v3,p)) {
            m_isKnotSelected = true;
            m_commandId = rand(); //random id so commands will be merged
            if (event->button() == Qt::LeftButton) {
                //left click selects left most knot
                m_selectedIndex = i;
            } else {
                //right click selects right most knot
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
        //get new coordinates
        float x,y;
        convertCoordinates(event->x(), event->y(), x, y);

        const vector<float> &knots = m_spline->getKnots();

        //restrict the movement of the knot to be between its neighbouring knots
        float i = m_selectedIndex;
        float lowerBound = i == 0 ? -INFINITY : knots[i-1];
        float upperBound = i == knots.size()-1 ? INFINITY : knots[i+1];

        float newKnot = max(x, lowerBound);
        newKnot = min(newKnot, upperBound);

        //move knot to new location
        MoveKnotCommand *command = new MoveKnotCommand(m_commandId, i, newKnot, *m_spline);
        m_undoStack->push(command);
        repaint();
    }
}

void BasisWidget::mouseReleaseEvent(QMouseEvent *) {
    //deselect knot
    if (m_isKnotSelected) {
        m_isKnotSelected = false;
        repaint();
    }
}

void BasisWidget::updateProjection() {
    //do not update the projection if the number of knots did not change.
    uint currKnots = m_spline->numberOfKnots();
    if (currKnots == m_lastKnotSize) {
        return;
    }
    m_lastKnotSize = currKnots;

    //clear projection matrix
    makeCurrent();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const vector<float> &knots = m_spline->getKnots();
    if (knots.size() == 0)
        return;

    //calculate dimensions of projection matrix
    m_leftProj = -CONTROL_LENGTH - PADDING;
    m_rightProj = knots.size()-1 + CONTROL_LENGTH + PADDING;
    m_bottomProj = 0;
    m_topProj = 1 + 2*PADDING + CONTROL_LENGTH;

    //set viewport and projection matrix
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


