SUBDIRS += utiles \
           reporte/reporte.pro \
           plugins/default/default.pro \
           plugins/plugins.pro \
           utiles/estilos/estilos.pro \
           src/src.pro

TEMPLATE = subdirs

CONFIG += warn_on \
          qt \
          thread

DESTDIR = .

OTHER_FILES = tareas-pendientes.txt \
              README.md \
              .travis.yml
unix {
  SUBDIRS += test
  LIBS += -gcov
  QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
  QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
  #system( ./generar_pri.sh eliminar )
  #system( ./generar_doc_usuario.sh )
}
