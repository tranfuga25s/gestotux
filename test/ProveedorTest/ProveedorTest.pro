#-------------------------------------------------
# Testeo para proveedores
#-------------------------------------------------

QT       += sql testlib

TARGET = tst_proveedortest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_proveedortest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../plugins/proveedor \
               ../../utiles

LIBS += -L../../bin/plugins \
        -lproveedor

FORMS += ../../plugins/proveedor/FormProveedorBase.ui

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}
