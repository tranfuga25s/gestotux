#ifndef MORDENTRABAJO_H
#define MORDENTRABAJO_H

#include <QSqlRelationalTableModel>
#include "NumeroComprobante.h"
#include <QSqlRecord>
#include <QDateTime>

class MOrdenTrabajo : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    MOrdenTrabajo( QObject *parent = 0 );

    void relacionarDatos();

    QVariant data( const QModelIndex &item, int role ) const;

    static bool tieneDatosRelacionados( const int id_orden_trabajo );

    static NumeroComprobante numeroComprobanteProximo();
    static NumeroComprobante numeroComprobanteSegunId( const int id_orden );
    static int idSegunNumeroComprobante( NumeroComprobante num );

    int obtenerIdEquipamientoSegunId( const int id_orden );

    bool cambiarTecnico( const int id_orden_trabajo, const int id_tecnico );

    bool cargarDatos( const int id_orden );
    int idCliente() { return _data.value("cliente_id").toInt(); }
    int idTecnico() { return _data.value("id_tecnico").toInt(); }
    QString requerente() { return _data.value( "requerente" ).toString(); }
    QString ingresante() { return _data.value( "ingresante" ).toString(); }
    QDateTime fechaDevolucion() { return _data.value("fecha_devolucion").toDateTime(); }
    QDateTime fechaIngreso() { return _data.value("fecha_ingreso").toDateTime(); }

    int agregarOrdenTrabajo( const int id_cliente, const int id_equipamiento,
                             const int id_tecnico, QString requerente,
                             QString ingresante, QDateTime fecha_ingreso,
                             QDateTime fecha_devolucion, QDateTime fecha_vencimiento,
                             QString causa_ingreso );

private:
    QSqlRecord _data;
    int _id_orden_trabajo;
};

#endif // MORDENTRABAJO_H
