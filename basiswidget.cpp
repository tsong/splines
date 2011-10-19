#include "basiswidget.h"
#include <QDebug>

BasisWidget::BasisWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

BasisWidget::~BasisWidget() {}


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

void BasisWidget::resizeGL(int width, int height) {
    //change viewport to new dimensions
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);

    //use an orthographic projection in the same dimensions as the draw surface
    glOrtho(0, width, height, 0, -1, 0);

    glMatrixMode(GL_MODELVIEW);
}

void BasisWidget::paintGL() {
    //clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}
