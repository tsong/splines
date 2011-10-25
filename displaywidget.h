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

    uint numberOfPoints();

signals:
    void pointsChanged(const vector<Vector2f> &points);

public slots:
    //clears all control points
    void clear();
    void toggleShowControlPoints();
    void toggleShowControlLines();

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
    QUndoStack *m_undoStack;

    BSpline *m_spline;
    bool m_splineCreated;

    bool m_showControlPoints;
    bool m_showControlLines;

    //tracks selected vertex for movement
    bool m_selected;
    uint m_selectedIndex;
    int m_moveCommandId;
};

#endif // DISPLAYWIDGET_H
