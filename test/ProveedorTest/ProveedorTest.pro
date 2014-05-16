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

LIBS += ../../bin/plugins/libproveedor.so

FORMS += ../../plugins/proveedor/FormProveedorBase.ui

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}