#ifndef MSIMULARCUOTAS_H
#define MSIMULARCUOTAS_H

#include <QAbstractTableModel>
#include <QDate>

#include "mplancuota.h"

class MSimularCuotas : public QAbstractTableModel
{
    Q_OBJECT

public:
    MSimularCuotas( QObject *parent = 0 );
    ~MSimularCuotas();

    void setCuotas( int c );
    void setImporte( double i ) { importe = i; }
    void setPeriodo( MPlanCuota::Periodo p ) { periodo = p; }
    void setInteres( double i ) { interes = i; }
    void setFechaInicio( QDate f ) { fecha_inicio = f; }
    void setPagoInicial( double d ) { pinicial = d; }

    void regenerar();

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex & index ) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

protected:
    double importe;
    double interes;
    double pinicial;
    int cantidad;
    MPlanCuota::Periodo periodo;
    QDate fecha_inicio;

    QHash<int,double> *sumatoria;
    QHash<int,double> *faltante;

    bool valido() const;

};

#endif // MSIMULARCUOTAS_H
