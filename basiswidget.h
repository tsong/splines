#ifndef BASISWIDGET_H
#define BASISWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <vector>
#include "bspline.h"
#include "utils/vector.h"

using namespace std;

class BasisWidget : public QGLWidget {
    Q_OBJECT
public:
    BasisWidget(QWidget *parent = 0, BSpline *spline = 0);
    ~BasisWidget();

protected:
    /*OpenGL events*/
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    /*mouse events*/
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

    void updateProjection();
    void convertCoordinates(float xIn, float yIn, float &xOut, float &yOut);
    void getControlTriangle(uint i, Vector2f &v1, Vector2f &v2, Vector2f &v3);
    void getProjectionCoord(float &left, float &right, float &bottom, float &top);

protected:
    BSpline *m_spline;
    bool m_splineCreated;

    bool m_isKnotSelected;
    uint m_selectedIndex;

    float m_leftProj;
    float m_rightProj;
    float m_bottomProj;
    float m_topProj;
};

#endif // BASISWIDGET_H
