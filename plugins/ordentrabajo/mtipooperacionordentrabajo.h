#ifndef MTIPOORDENTRABAJO_H
#define MTIPOORDENTRABAJO_H

#include <QSqlTableModel>

class MTipoOperacionOrdenTrabajo : public QSqlTableModel
{
    Q_OBJECT

public:

    enum TipoOperacionOrdenTrabajo {
        CambioTecnico = 1, /** Indica que la orden cambio de tecnico responsable */
        CambioCliente = 2, /**  Indica que la orden cambio de cliente */
        CambioEquipamiento = 3, /** Indica que la orden sufrió el cambio de equipamiento */
        CierreOrden = 4, /** Indica que la orden fue cerrada */
        DevolucionCliente = 5, /** Indica que la orden fue terminada por devolucion al cliente */
        Cancelacion = 6, /** Indica que la orden fue cancelada */
        Facturacion = 7, /** Indica que el evento es una facturacion */
        IngresoAlTaller = 8 /** Indica que el evento es el ingreso al taller */
    };

    MTipoOperacionOrdenTrabajo(QObject *parent = 0);
    
    static bool existe( const QString nombre );
    bool tieneDatosRelacionados( const int id_tipo );

    bool agregarTipo( const QString nombre );
    bool modificar( const int row,  const QString nombre );
    
};

#endif // MTIPOORDENTRABAJO_H
