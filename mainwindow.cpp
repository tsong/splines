#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), displayWidget(0), undoStack(0)
{
    undoStack = new QUndoStack(this);
    displayWidget = new DisplayWidget(parent);
    this->setCentralWidget(displayWidget);

    createMenus();

    resize(MAIN_WINDOW_DEFAULT_WIDTH, MAIN_WINDOW_DEFAULT_HEIGHT);
}

MainWindow::~MainWindow()
{
    delete displayWidget;
}

void MainWindow::createMenus() {
    //file menu
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *exitAction = new QAction("E&xit",this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu->addAction(exitAction);

    //edit menu
    QMenu *editMenu = menuBar()->addMenu("&Edit");
    QAction *clearAction = new QAction("&Clear", this);
    clearAction->setShortcut(QKeySequence("Ctrl+C"));
    connect(clearAction, SIGNAL(triggered()), displayWidget, SLOT(clear()));
    editMenu->addAction(clearAction);

    QAction *undoAction = undoStack->createUndoAction(this);
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    editMenu->addAction(undoAction);

    QAction *redoAction = undoStack->createRedoAction(this);
    redoAction->setShortcut(QKeySequence("Shift+Ctrl+Z"));
    editMenu->addAction(redoAction);
}

