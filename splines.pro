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
    addpointcommand.cpp

HEADERS  += mainwindow.h \
    displaywidget.h \
    matrix.h \
    vector.h \
    glutils.h \
    pointsutil.h \
    addpointcommand.h

FORMS    +=

OTHER_FILES += \
    README
