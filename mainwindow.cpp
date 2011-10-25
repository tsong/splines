#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>

#include "orderaction.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), displayWidget(0), undoStack(0)
{
    undoStack = new QUndoStack(this);
    displayWidget = new DisplayWidget(this, undoStack);
    basisWidget = new BasisWidget(this);
    createMenus();

    connect(displayWidget, SIGNAL(pointsChanged(const vector<Vector2f>&)), basisWidget, SLOT(createKnots(const vector<Vector2f>&)));
    connect(basisWidget, SIGNAL(knotsChanged(const vector<float>&)), displayWidget, SLOT(setKnots(const vector<float>&)));

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

    //view menu
    QMenu *showMenu = menuBar()->addMenu("&Show");
    QAction *showPointsAction = new QAction("&Show Control Points", this);
    showPointsAction->setShortcut(QKeySequence("Shift+Ctrl+P"));
    showPointsAction->setCheckable(true);
    showPointsAction->setChecked(true);
    connect(showPointsAction, SIGNAL(triggered()), displayWidget, SLOT(toggleShowControlPoints()));
    showMenu->addAction(showPointsAction);

    QAction *showLinesAction = new QAction("&Show Control Lines", this);
    showLinesAction->setShortcut(QKeySequence("Shift+Ctrl+L"));
    showLinesAction->setCheckable(true);
    showLinesAction->setChecked(true);
    connect(showLinesAction, SIGNAL(triggered()), displayWidget, SLOT(toggleShowControlLines()));
    showMenu->addAction(showLinesAction);

    //order menu
    QMenu *orderMenu = menuBar()->addMenu("&Order");
    for (uint i = 2; i < 7; i++) {
        OrderAction *orderAction = new OrderAction(i, QString("&%1").arg(i), this);
        connect(orderAction, SIGNAL(triggered(uint)), displayWidget, SLOT(setOrder(uint)));
        connect(orderAction, SIGNAL(triggered(uint)), basisWidget, SLOT(setOrder(uint)));
        orderAction->setShortcut(QString("Ctrl+%1").arg(i));
        orderMenu->addAction(orderAction);
    }
}

