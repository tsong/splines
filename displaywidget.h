#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QUndoStack>

#include "bspline.h"
#include "utils/vector.h"

#include "commands/addpointcommand.h"
#include "commands/deletepointcommand.h"
#include "commands/movepointcommand.h"
#include "commands/clearcommand.h"
#include "commands/togglecommand.h"

#define POINT_RADIUS 6

using namespace std;

class DisplayWidget : public QGLWidget {
    Q_OBJECT
public:
    DisplayWidget(QWidget *parent = 0, QUndoStack *undoStack = 0, BSpline *spline = 0);
    ~DisplayWidget();

public slots:
    //clears all control points
    void clear();

    //toggle visibility and segments
    void toggleShowControlPoints();
    void toggleShowControlLines();
    void toggleShowSegments();

protected:
    /*OpenGL events*/
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    /*mouse events*/
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

protected:
    //All interactions are pushed to this stack
    QUndoStack *m_undoStack;

    //B Spline to be drawn
    BSpline *m_spline;
    bool m_splineCreated;

    //controls visibility of control points and control lines
    bool m_showControlPoints;
    bool m_showControlLines;
    bool m_showSegments;

    //tracks selected vertex for movement
    bool m_selected;
    uint m_selectedIndex;
    int m_moveCommandId;
};

#endif // DISPLAYWIDGET_H
