#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAIN_WINDOW_DEFAULT_WIDTH 640
#define MAIN_WINDOW_DEFAULT_HEIGHT 480

#include <QMainWindow>
#include "displaywidget.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void createMenus();

protected:
    //surface which users interact with and draw the points
    DisplayWidget *displayWidget;
};

#endif // MAINWINDOW_H
