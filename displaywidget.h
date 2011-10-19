#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QUndoStack>

#include "vector.h"

#include "addpointcommand.h"
#include "deletepointcommand.h"
#include "movepointcommand.h"
#include "clearcommand.h"

#define POINT_RADIUS 6

using namespace std;

class DisplayWidget : public QGLWidget {
    Q_OBJECT
public:
    DisplayWidget(QWidget *parent = 0, QUndoStack *undoStack = 0);
    ~DisplayWidget();

    friend class AddPointCommand;
    friend class DeletePointCommand;
    friend class MovePointCommand;
    friend class ClearCommand;

public slots:
    //clears all control points
    void clear();

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

    bool m_showControlPoints;
    vector<Vector2f> m_controlPoints;

    //tracks selected vertex for movement
    bool m_selected;
    uint m_selectedIndex;
    int m_moveCommandId;
};

#endif // DISPLAYWIDGET_H
