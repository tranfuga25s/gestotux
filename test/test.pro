TEMPLATE = subdirs

SUBDIRS +=  ProductosTest \
            PeriodoServicioTest \
            #BajaClienteServicioTest \
            PreferenciasTest \
            #EquipamientosTest \
            CancelacionPeriodoServicio

OTHER_FILES += sql/dependences.ini \
               sql/QSQLITE/productos.sql \
               sql/QSQLITE/servicios.sql \
               sql/QSQLITE/categorias_productos.sql \
               sql/QSQLITE/periodo_servicio.sql \
               sql/QSQLITE/cobro_servicio_periodo.sql