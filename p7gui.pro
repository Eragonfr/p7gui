#-------------------------------------------------
#
# Project created by QtCreator 2017-01-25T22:42:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = p7gui
TEMPLATE = app

CONFIG += link_pkgconfig
PKGCONFIG += libp7

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/Connection.cpp \
    src/FileSystemTree.cpp \
    src/CalculatorFileTree.cpp \
    src/async/lsFilesAsync.cpp \
    src/async/initAsync.cpp \
    src/async/optimizeAsync.cpp \
    src/async/reqFileAsync.cpp \
    src/async/p7Async.cpp \
    src/async/sendFileAsync.cpp \
    src/async/delFileAsync.cpp \
    src/async/copyFileAsync.cpp \
    src/TaskNotifierWidget.cpp \
    src/AsyncPool.cpp \
    src/FileInfo.cpp

HEADERS  += src/MainWindow.h \
    src/Connection.h \
    src/FileSystemTree.h \
    src/CalculatorFileTree.h \
    src/async/lsFilesAsync.h \
    src/async/initAsync.h \
    src/async/optimizeAsync.h \
    src/async/reqFileAsync.h \
    src/async/p7Async.h \
    src/async/sendFileAsync.h \
    src/async/delFileAsync.h \
    src/async/copyFileAsync.h \
    src/TaskNotifierWidget.h \
    src/AsyncPool.h \
    src/FileInfo.h

FORMS    += src/MainWindow.ui \
    src/TaskNotifierWidget.ui

target.path = /usr/bin
INSTALLS += target
