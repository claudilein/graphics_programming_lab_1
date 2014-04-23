#-------------------------------------------------
#
# Project created by QtCreator 2014-04-15T20:33:48
#
#-------------------------------------------------

QT       += core gui opengl

# linking errors on other machines?
LIBS     += -lGLU

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hellocube
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h

RESOURCES += \
    hellocube.qrc
