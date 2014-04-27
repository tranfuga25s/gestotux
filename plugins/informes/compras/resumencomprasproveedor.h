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

    QVariant data( const QModelIndex &idx, int role ) const;


public slots:
    void cambioProveedor( int id_proveedor );

private:
    QString _filtro;
    void actualizarDatos();
    DivisionTemporal _metodo_temporal;
       
};

#endif // RESUMENCOMPRASPROVEEDOR_H
