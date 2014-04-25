# TR Sistemas Informaticos Integrales - Esteban Zeller
TEMPLATE = subdirs
CONFIG += warn_on \
          qt \
          thread  \
          ordered

SUBDIRS = utiles/utiles.pro \
          reporte/reporte.pro \
          utiles/estilos/estilos.pro \
          plugins/CtaCte/CtaCteLib.pro \
          plugins/ventas/VentasLib.pro \
          plugins/remitos/RemitosLib.pro \
          plugins/productos/productos.pro \
          plugins/descuentos/descuentos.pro \
          plugins/remitos/remitos.pro \
          plugins/servicios/servicios.pro \
          plugins/pagos/pagos.pro \
          plugins/CtaCte/CtaCte.pro \
          plugins/caja/caja.pro \
          plugins/ventas/ventas.pro \
          plugins/presupuesto/presupuesto.pro \
          plugins/gastos/gastos.pro \
          plugins/compras/compras.pro \
          plugins/proveedor/proveedor.pro \
          plugins/informes/informa.pro \
          plugins/informes/informes.pro \
          plugins/cuotas/cuotas.pro \
          plugins/trsistemas/trsistemas.pro \
          src/src.pro

NOMBRE_PROGRAMA = ../../bin/trsis
ARCHIVO_RC = trsistemas/imagenes/icono.rc

unix {
  #system( generar_pri.sh $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}
win32 {
 #system( generar_pri.bat $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}
