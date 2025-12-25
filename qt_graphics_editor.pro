QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/configdialog.cpp \
    src/drawingarea.cpp \
    src/ellipse.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/rectangle.cpp \
    src/shape.cpp \
    src/shapefactory.cpp

HEADERS += \
    src/configdialog.h \
    src/drawingarea.h \
    src/ellipse.h \
    src/mainwindow.h \
    src/rectangle.h \
    src/shape.h \
    src/shapefactory.h

FORMS += \
    ui/configdialog.ui \
    ui/mainwindow.ui

TRANSLATIONS += \
    ts/qt_graphics_editor_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
