#ifndef BASISWIDGET_H
#define BASISWIDGET_H

#include <QGLWidget>
#include <vector>

using namespace std;

class BasisWidget : public QGLWidget {
    Q_OBJECT
public:
    BasisWidget(QWidget *parent = 0);
    ~BasisWidget();

    void setOrder(uint order);
    void setKnots(vector<float> knots);
    void addKnot(float knot);
    bool removeKnot(uint position);
    bool moveKnot(uint position, float newKnot);

protected:
    /*OpenGL events*/
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void updateProjection();

protected:
    vector<float> m_knots;
    uint m_order;
};

#endif // BASISWIDGET_H
