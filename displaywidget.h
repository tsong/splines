#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>

#include "vector.h"

#define POINT_RADIUS 8

using namespace std;

class DisplayWidget : public QGLWidget {
    Q_OBJECT
public:
    DisplayWidget(QWidget *parent = 0);
    ~DisplayWidget();

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
    bool m_showControlPoints;
    list<Vector2f> m_controlPoints;

    //tracks selected vertex for movement
    bool m_selected;
    list<Vector2f>::iterator m_selectedIterator;
};

#endif // DISPLAYWIDGET_H
