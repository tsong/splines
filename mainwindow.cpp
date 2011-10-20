#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>

#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), displayWidget(0), undoStack(0)
{
    undoStack = new QUndoStack(this);
    displayWidget = new DisplayWidget(this, undoStack);
    basisWidget = new BasisWidget(this);
    createMenus();

    connect(basisWidget, SIGNAL(knotsChanged(vector<float>)), displayWidget, SLOT(setKnots(vector<float>)));

    //create splitter
    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(displayWidget);
    splitter->addWidget(basisWidget);
    splitter->setStretchFactor(0,6);
    splitter->setStretchFactor(1,3);


    this->setCentralWidget(splitter);


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

