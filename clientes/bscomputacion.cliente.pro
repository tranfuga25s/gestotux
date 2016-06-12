# BS Computacion - Daniel Sequeira
TEMPLATE = subdirs
CONFIG += warn_on \
          qt \
          thread  \
          ordered

SUBDIRS = utiles/utiles.pro \
          reporte/reporte.pro \
          utiles/estilos/estilos.pro \
          plugins/bscomputacion/bscomputacion.pro \
          plugins/productos/productos.pro \
          plugins/servicios/servicios.pro \
          plugins/pagos/pagos.pro \
          plugins/CtaCte/CtaCte.pro \
          plugins/caja/caja.pro \
          plugins/ventas/ventas.pro \
          plugins/cuotas/cuotas.pro \
          plugins/presupuesto/presupuesto.pro \
          plugins/gastos/gastos.pro \
          plugins/compras/compras.pro \
          plugins/proveedor/proveedor.pro \
          plugins/feedback/feedback.pro \
          plugins/remitos/remitos.pro \
          plugins/ordentrabajo/ordentrabajo.pro \
          src/src.pro

NOMBRE_PROGRAMA = ../../bin/BsComputacion
ARCHIVO_RC = BsComputacion/icono.rc

unix {
  system( generar_pri.sh $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}
win32 {
  system( generar_pri.bat $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}
