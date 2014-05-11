#ifndef RESUMENCOMPRASPROVEEDOR_H
#define RESUMENCOMPRASPROVEEDOR_H

#include <QSqlQueryModel>
#include <QMap>
#include "TiposPeriodos.h"

class ResumenComprasProveedor : public QSqlQueryModel
{
    Q_OBJECT
public:
    ResumenComprasProveedor( QObject *parent = 0 );
    void setearMetodoTemporal( DivisionTemporal metodo );

    QVariant data( const QModelIndex &idx, int role ) const;


public slots:
    void cambiarProveedor( int id_proveedor );

private:
    QString _filtro;
    void actualizarDatos();
    DivisionTemporal _metodo_temporal;

    QMap<QString,QString> consultas;
       
};

#endif // RESUMENCOMPRASPROVEEDOR_H
