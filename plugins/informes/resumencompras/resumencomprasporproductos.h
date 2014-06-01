#ifndef RESUMENCOMPRASPORPRODUCTOS_H
#define RESUMENCOMPRASPORPRODUCTOS_H

#include <QSqlQueryModel>
#include <QMap>
#include "TiposPeriodos.h"

class ResumenComprasPorProductos : public QSqlQueryModel
{
    Q_OBJECT
public:
    ResumenComprasPorProductos(QObject *parent = 0);
    
    void setearMetodoTemporal( DivisionTemporal metodo );
    void setearIdProducto( const int id_producto );

    QVariant data(const QModelIndex &idx, int role) const;

private:
    void actualizarDatos();
    DivisionTemporal _metodo_temporal;
    QMap<QString,QString> consultas;
    int _id_producto;
    
};

#endif // RESUMENCOMPRASPORPRODUCTOS_H
