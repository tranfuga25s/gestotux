#ifndef REPORTESCUOTAS_H
#define REPORTESCUOTAS_H

#include "einformeinterface.h"
#include <QtPlugin>

class ReportesCuotas : public QObject, public EInformeInterface
{
    Q_OBJECT
    Q_INTERFACES( EInformeInterface )

public:
    double version() const;
    QString nombre() const;
    bool inicializar();
    void hacerMenu( QMenu *m );

protected slots:
    void resumenCuotasDeudaHistorica();
    void resumenCuotasMes();
    void resumenCuotasCliente();
    void seCierraGestotux(){}

signals:
    void agregarVentana( QWidget * );

private:
    QAction *ActResumenCuotasDeudaHistorica;
    QAction *ActResumenCuotasCliente;
    QAction *ActResumenCuotasMes;

};

#endif // REPORTESCUOTAS_H
