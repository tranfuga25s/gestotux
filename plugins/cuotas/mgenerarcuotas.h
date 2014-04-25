#ifndef MGENERARCUOTAS_H
#define MGENERARCUOTAS_H

#include <QAbstractTableModel>
#include <QPair>
#include <QHash>
#include "NumeroComprobante.h"

class MGenerarCuotas : public QAbstractTableModel
{
    Q_OBJECT
public:
    MGenerarCuotas( QObject *parent = 0 );
    ~MGenerarCuotas();
    QVariant data( const QModelIndex &idx, int role ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    int rowCount( const QModelIndex &parent = QModelIndex()  ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    
    bool calcularComprobantes();
    void setearMesBusqueda( const int mes ) { _mes_busqueda = mes; }
    int mesBusqueda() { return _mes_busqueda; }

signals:
    void cambioTotal( double );
    void cambioCantidad( int );

private:
    int _cant;
    int _mes_busqueda;
    double _total;
    QHash<int, int> *_numeros;
    QHash<int, int> *_planes;
    QHash<int, QString> *_clientes;
    QHash<int, int> *_clientes_id;
    QHash<int, QString> *_cuotas;
    QHash<int, double> *_importes;
    QHash<int, NumeroComprobante*> *_comprobantes;
    
};

#endif // MGENERARCUOTAS_H
