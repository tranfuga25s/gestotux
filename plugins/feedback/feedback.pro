TEMPLATE = lib

CONFIG += plugin \
          dll \
          help

TARGET = feedback

QT += xml \
      network

DESTDIR = ../../bin/plugins

INCLUDEPATH += ../../utiles \
               ../../src

LIBS += ../../bin/libutiles.a

PRE_TARGETDEPS += ../../bin/libutiles.a

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}

HEADERS += feedbackplugin.h \
           feedbacksender.h

SOURCES += feedbackplugin.cpp \
           feedbacksender.cpp
