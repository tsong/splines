#ifndef BASISWIDGET_H
#define BASISWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QUndoStack>
#include <vector>
#include "bspline.h"
#include "utils/vector.h"

using namespace std;

class BasisWidget : public QGLWidget {
    Q_OBJECT
public:
    BasisWidget(QWidget *parent = 0, QUndoStack *undoStack = 0, BSpline *spline = 0);
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

public slots:
    //resets basis functions to original state
    void reset();

    //toggles if segments are distinctively colored
    void toggleShowSegments();

protected:
    //converts screen coordinates to world coordinates
    void convertCoordinates(float xIn, float yIn, float &xOut, float &yOut);

    //returns the coordinates of a knot's control triangle
    void getControlTriangle(uint i, Vector2f &v1, Vector2f &v2, Vector2f &v3);

protected slots:
    //updates the dimensions of the projection matrix to fit basis functions
    void updateProjection();

protected:
    QUndoStack *m_undoStack;
    int m_commandId;    //id used so consecutive move commands are merged

    //spline whose basis functions will be drawn
    BSpline *m_spline;
    bool m_splineCreated;

    //knot selection and movement
    bool m_isKnotSelected;
    uint m_selectedIndex;
    uint m_lastKnotSize;

    //dimensions of the world space after projection
    float m_leftProj;
    float m_rightProj;
    float m_bottomProj;
    float m_topProj;

    bool m_showSegments;
};

#endif // BASISWIDGET_H
