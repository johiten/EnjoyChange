#-------------------------------------------------
#
# Project created by QtCreator 2016-11-28T14:41:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EnjoyChange
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    include/edk.h \
    include/edkErrorCode.h \
    include/EmoStateDLL.h \
    include/glut.h

FORMS    += mainwindow.ui


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -ledk
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -ledkd

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -ledk_utils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -ledk_utilsd

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lglut32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lglut32d

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

DISTFILES += \
    lib/edk.lib \
    lib/edk_utils.lib \
    lib/glut32.lib \
    lib/edk.dll \
    lib/edk_utils.dll
