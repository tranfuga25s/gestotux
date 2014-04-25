TEMPLATE = lib

CONFIG += plugin \
          dll \
          help

TARGET = backupremoto

QT += xml \
      network

HEADERS += backupremotoplugin.h \
           formprefbackupremoto.h \
    json.h \
    eenviobackup.h \
    ebackupremoto.h \
    mtempbackups.h

SOURCES += backupremotoplugin.cpp \
           formprefbackupremoto.cpp \
    json.cpp \
    eenviobackup.cpp \
    ebackupremoto.cpp \
    mtempbackups.cpp

DESTDIR = ../../bin/plugins

#RESOURCES += backupremoto.qrc

INCLUDEPATH += ../../utiles \
               ../../src

LIBS += ../../bin/libutiles.a

PRE_TARGETDEPS += ../../bin/libutiles.a

FORMS += formprefbackupremoto.ui \
    formBackupRemoto.ui

RESOURCES += backupremoto.qrc

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

unix {
    LIBS += -gcov
}
