#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>

#include "orderaction.h"
#include "commands/setordercommand.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_displayWidget(0), m_undoStack(0)
{
    m_undoStack = new QUndoStack(this);
    m_displayWidget = new DisplayWidget(this, m_undoStack, &m_spline);
    m_basisWidget = new BasisWidget(this, m_undoStack, &m_spline);
    createMenus();

    //create splitter
    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(m_displayWidget);
    splitter->addWidget(m_basisWidget);
    splitter->setStretchFactor(0,6);
    splitter->setStretchFactor(1,3);

    this->setCentralWidget(splitter);


    resize(MAIN_WINDOW_DEFAULT_WIDTH, MAIN_WINDOW_DEFAULT_HEIGHT);
}

MainWindow::~MainWindow()
{
    delete m_displayWidget;
}

void MainWindow::createMenus() {
    //file menu and exit menu item
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *exitAction = new QAction("E&xit",this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu->addAction(exitAction);

    //edit menu
    QMenu *editMenu = menuBar()->addMenu("&Edit");

    //clear points menu item
    QAction *clearAction = new QAction("&Clear", this);
    clearAction->setShortcut(QKeySequence("Ctrl+C"));
    connect(clearAction, SIGNAL(triggered()), m_displayWidget, SLOT(clear()));
    editMenu->addAction(clearAction);

    //reset basis functions menu item
    QAction *resetBasisAction = new QAction("Reset &Knots", this);
    resetBasisAction->setShortcut(QKeySequence("Ctrl+K"));
    connect(resetBasisAction, SIGNAL(triggered()), m_basisWidget, SLOT(reset()));
    editMenu->addAction(resetBasisAction);

    //undo menu item
    QAction *undoAction = m_undoStack->createUndoAction(this);
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    editMenu->addAction(undoAction);

    //redo menu item
    QAction *redoAction = m_undoStack->createRedoAction(this);
    redoAction->setShortcut(QKeySequence("Shift+Ctrl+Z"));
    editMenu->addAction(redoAction);

    //view menu
    QMenu *showMenu = menuBar()->addMenu("&Show");

    //show control points menu item
    QAction *showPointsAction = new QAction("Toggle Control &Points", this);
    showPointsAction->setShortcut(QKeySequence("Shift+Ctrl+P"));
    //showPointsAction->setCheckable(true);
    //showPointsAction->setChecked(true);
    connect(showPointsAction, SIGNAL(triggered()), m_displayWidget, SLOT(toggleShowControlPoints()));
    showMenu->addAction(showPointsAction);

    //show control lines menu item
    QAction *showLinesAction = new QAction("Toggle Control &Lines", this);
    showLinesAction->setShortcut(QKeySequence("Shift+Ctrl+L"));
    //showLinesAction->setCheckable(true);
    //showLinesAction->setChecked(true);
    connect(showLinesAction, SIGNAL(triggered()), m_displayWidget, SLOT(toggleShowControlLines()));
    showMenu->addAction(showLinesAction);

    //show spline segments menu item
    QAction *showSplineAction = new QAction("Toggle &Spline Segments", this);
    showSplineAction->setShortcut(QKeySequence("Shift+Ctrl+S"));
    //showSegmentsAction->setCheckable(true);
    connect(showSplineAction, SIGNAL(triggered()), m_displayWidget, SLOT(toggleShowSegments()));
    showMenu->addAction(showSplineAction);

    //show basis segments menu item
    QAction *showBasisAction = new QAction("Toggle &Basis Segments", this);
    showBasisAction->setShortcut(QKeySequence("Shift+Ctrl+B"));
    //showSegmentsAction->setCheckable(true);
    connect(showBasisAction, SIGNAL(triggered()), m_basisWidget, SLOT(toggleShowSegments()));
    showMenu->addAction(showBasisAction);

    //order menu
    QMenu *orderMenu = menuBar()->addMenu("&Order");
    //QActionGroup *actionGroup = new QActionGroup(this); //only one menu item selected at a time
    for (uint i = 2; i < 7; i++) {
        //menu item for the current order
        OrderAction *orderAction = new OrderAction(i, QString("&%1").arg(i), this);
        connect(orderAction, SIGNAL(triggered(uint)), this, SLOT(setOrder(uint)));
        orderAction->setShortcut(QString("Ctrl+%1").arg(i));
        //orderAction->setCheckable(true);
        //orderAction->setActionGroup(actionGroup);
        orderMenu->addAction(orderAction);

        //set default menu item
        /*if (i == 3)
            orderAction->setChecked(true);*/
    }
}

void MainWindow::setOrder(uint k) {
    m_undoStack->push(new SetOrderCommand(k, m_spline));
}

