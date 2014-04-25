TEMPLATE = lib

CONFIG += plugin \
          dll \
          help \
          excpetions \
          rtti

TARGET = garantias

QT += xml \
      network \
      svg

HEADERS += garantiasplugin.h \
           vgarantias.h \
           mgarantias.h \
           dagregargarantia.h \
           formprefgarantias.h \
           egarantiasvg.h \
           mvgarantiassvg.h \
           mvgarantias.h \
           formvencimientogarantias.h
SOURCES += garantiasplugin.cpp \
           vgarantias.cpp \
           mgarantias.cpp \
           dagregargarantia.cpp \
           formprefgarantias.cpp \
           egarantiasvg.cpp \
           mvgarantiassvg.cpp \
           mvgarantias.cpp \
           formvencimientogarantias.cpp

DESTDIR = ../../bin/plugins

RESOURCES += garantias.qrc

INCLUDEPATH += ../../utiles \
               ../../src \
               ../../reporte \
               ../ordentrabajo \
               ../ventas

PRE_TARGETDEPS += ../../bin/libutiles.a

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

OTHER_FILES += garantias.QMYSQL.sql \
               garantias.QSQLITE.sql \
               imagenes/garantias.svg

FORMS += dagregargarantia.ui \
         formprefgarantias.ui \
         formvencimientogarantias.ui

QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte \
        -lordentrabajo


unix {
    LIBS += -gcov
}
