#ifndef BASISWIDGET_H
#define BASISWIDGET_H

#include <QGLWidget>

class BasisWidget : public QGLWidget {
    Q_OBJECT
public:
    BasisWidget(QWidget *parent = 0);
    ~BasisWidget();

protected:
    /*OpenGL events*/
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
};

#endif // BASISWIDGET_H
