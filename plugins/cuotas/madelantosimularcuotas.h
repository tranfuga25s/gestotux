#ifndef MADELANTOSIMULARCUOTAS_H
#define MADELANTOSIMULARCUOTAS_H

#include "msimularcuotas.h"

class MAdelantoSimularCuotas : public MSimularCuotas
{
    Q_OBJECT
public:
    MAdelantoSimularCuotas( QObject *parent = 0 );
    ~MAdelantoSimularCuotas();

    void setearImporteAdelanto( double importe ) { _adelanto = importe; regenerar(); }
    double importeAdelanto() { return _adelanto; }

    void cargarCuotasPagadas( const int id_plan_cuota );

    void regenerar();

    QVariant data( const QModelIndex &index, int role ) const;
    
private:
    double _adelanto;
    QHash<int,double> *cuotas;
    QHash<int,double> *adelantos;
    QHash<int,bool> *pagados;
    QHash<int,bool> *modificables;

};

#endif // MADELANTOSIMULARCUOTAS_H
