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

OTHER_FILES = tareas-pendientes.txt

unix {
  SUBDIRS += test
  system( generar_pri.sh eliminar )
  system( generar_doc_usuario.sh )
}
