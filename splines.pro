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
    basiswidget.cpp \
    orderaction.cpp \
    commands/addpointcommand.cpp \
    commands/deletepointcommand.cpp \
    commands/clearcommand.cpp \
    commands/movepointcommand.cpp \
    commands/togglecommand.cpp

HEADERS  += mainwindow.h \
    displaywidget.h \
    matrix.h \
    vector.h \
    glutils.h \
    pointsutil.h \
    basiswidget.h \
    orderaction.h \
    commands/addpointcommand.h \
    commands/deletepointcommand.h \
    commands/clearcommand.h \
    commands/movepointcommand.h \
    commands/togglecommand.h

FORMS    +=

OTHER_FILES += \
    README
