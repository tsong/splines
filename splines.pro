#-------------------------------------------------
#
# Project created by QtCreator 2011-09-26T22:45:42
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = splines
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    displaywidget.cpp \
    glutils.cpp \
    pointsutil.cpp \
    addpointcommand.cpp \
    deletepointcommand.cpp \
    clearcommand.cpp \
    movepointcommand.cpp \
    basiswidget.cpp \
    orderaction.cpp \
    togglecommand.cpp

HEADERS  += mainwindow.h \
    displaywidget.h \
    matrix.h \
    vector.h \
    glutils.h \
    pointsutil.h \
    addpointcommand.h \
    deletepointcommand.h \
    clearcommand.h \
    movepointcommand.h \
    basiswidget.h \
    orderaction.h \
    togglecommand.h

FORMS    +=

OTHER_FILES += \
    README
