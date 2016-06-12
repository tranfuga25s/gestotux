# Inmobiliaria Barbier
TEMPLATE = subdirs
CONFIG += warn_on \
          qt \
          thread  \
          ordered

SUBDIRS = ../utiles/utiles.pro \
          ../reporte/reporte.pro \
          ../utiles/estilos/estilos.pro \
          ../plugins/caja/caja.pro \
          ../plugins/ventas/ventas.pro \
          ../plugins/CtaCte/CtaCte.pro \
          ../plugins/pagos/pagos.pro \
          ../plugins/feedback/feedback.pro \
          ../src/src.pro \
          Barbier/barbier.pro

NOMBRE_PROGRAMA = ../../../bin/Barbier
ARCHIVO_RC = Barbier/icono.rc