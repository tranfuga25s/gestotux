#ifndef RESUMENCOMPRASPROVEEDOR_H
#define RESUMENCOMPRASPROVEEDOR_H

#include <QSqlTableModel>
#include "TiposPeriodos.h"

class ResumenComprasProveedor : public QSqlTableModel
{
    Q_OBJECT
public:
    ResumenComprasProveedor( QObject *parent = 0 );
    void setearMetodoTemporal( DivisionTemporal metodo );

    QVariant data(const QModelIndex &idx, int role) const;

private:
    void actualizarDatos();
    DivisionTemporal _metodo_temporal;
       
};

#endif // RESUMENCOMPRASPROVEEDOR_H
