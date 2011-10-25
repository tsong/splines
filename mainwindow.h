#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAIN_WINDOW_DEFAULT_WIDTH 800
#define MAIN_WINDOW_DEFAULT_HEIGHT 600

#include <QMainWindow>
#include <QUndoStack>
#include "displaywidget.h"
#include "basiswidget.h"
#include "bspline.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void createMenus();

protected slots:
    void setOrder(uint k);

protected:
    //surface which users interact with and draw the points
    DisplayWidget *m_displayWidget;

    //widget which displays and let users interact with the spline basis functions
    BasisWidget *m_basisWidget;

    QUndoStack *m_undoStack;
    BSpline m_spline;
};

#endif // MAINWINDOW_H
