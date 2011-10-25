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
    basiswidget.cpp \
    orderaction.cpp \
    utils/glutils.cpp \
    utils/pointutils.cpp \
    commands/addpointcommand.cpp \
    commands/deletepointcommand.cpp \
    commands/clearcommand.cpp \
    commands/movepointcommand.cpp \
    commands/togglecommand.cpp \
    bspline.cpp \
    commands/moveknotcommand.cpp \
    commands/setordercommand.cpp \
    commands/resetknotscommand.cpp

HEADERS  += mainwindow.h \
    displaywidget.h \
    basiswidget.h \
    orderaction.h \
    utils/matrix.h \
    utils/vector.h \
    utils/glutils.h \
    utils/pointutils.h \
    commands/addpointcommand.h \
    commands/deletepointcommand.h \
    commands/clearcommand.h \
    commands/movepointcommand.h \
    commands/togglecommand.h \
    bspline.h \
    commands/moveknotcommand.h \
    commands/setordercommand.h \
    commands/resetknotscommand.h

FORMS    +=

OTHER_FILES += \
    README
