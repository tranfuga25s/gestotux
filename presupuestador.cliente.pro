# Presupuestador - GB
TEMPLATE = subdirs
CONFIG += warn_on \
        qt \
        thread  \
	ordered

SUBDIRS = utiles \
          reporte/reporte.pro \
          src \
          utiles/estilos \
          plugins/presupuestador
NOMBRE_PROGRAMA = presupuestador
ARCHIVO_RC = presupuestador/icono.rc