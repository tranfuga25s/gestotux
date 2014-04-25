TEMPLATE = lib
TARGET = l4ustyle2
CONFIG += plugin \
          dll
Qt += gui \
      core
INCLUDEPATH += .
VERSION = 0.1

HEADERS += l4ustyle2.h misc.h
SOURCES += l4ustyle2.cpp misc.cpp

DESTDIR = ../../../bin/styles

linux-g++ {
    UI_DIR = ui
    MOC_DIR = moc
    OBJECTS_DIR = obj
}
win32 {
    MOC_DIR = win/moc
    UI_DIR = win/ui
    OBJECTS_DIR = win/objeto
}