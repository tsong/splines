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
    //file menu
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *exitAction = new QAction("E&xit",this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu->addAction(exitAction);

    //edit menu
    QMenu *editMenu = menuBar()->addMenu("&Edit");
    QAction *clearAction = new QAction("&Clear", this);
    clearAction->setShortcut(QKeySequence("Ctrl+C"));
    connect(clearAction, SIGNAL(triggered()), m_displayWidget, SLOT(clear()));
    editMenu->addAction(clearAction);

    QAction *undoAction = m_undoStack->createUndoAction(this);
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    editMenu->addAction(undoAction);

    QAction *redoAction = m_undoStack->createRedoAction(this);
    redoAction->setShortcut(QKeySequence("Shift+Ctrl+Z"));
    editMenu->addAction(redoAction);

    //view menu
    QMenu *showMenu = menuBar()->addMenu("&Show");
    QAction *showPointsAction = new QAction("&Show Control Points", this);
    showPointsAction->setShortcut(QKeySequence("Shift+Ctrl+P"));
    showPointsAction->setCheckable(true);
    showPointsAction->setChecked(true);
    connect(showPointsAction, SIGNAL(triggered()), m_displayWidget, SLOT(toggleShowControlPoints()));
    showMenu->addAction(showPointsAction);

    QAction *showLinesAction = new QAction("&Show Control Lines", this);
    showLinesAction->setShortcut(QKeySequence("Shift+Ctrl+L"));
    showLinesAction->setCheckable(true);
    showLinesAction->setChecked(true);
    connect(showLinesAction, SIGNAL(triggered()), m_displayWidget, SLOT(toggleShowControlLines()));
    showMenu->addAction(showLinesAction);

    //order menu
    QMenu *orderMenu = menuBar()->addMenu("&Order");
    for (uint i = 2; i < 7; i++) {
        OrderAction *orderAction = new OrderAction(i, QString("&%1").arg(i), this);
        connect(orderAction, SIGNAL(triggered(uint)), this, SLOT(setOrder(uint)));
        orderAction->setShortcut(QString("Ctrl+%1").arg(i));
        orderMenu->addAction(orderAction);
    }
}

void MainWindow::setOrder(uint k) {
    m_undoStack->push(new SetOrderCommand(k, m_spline));
}

