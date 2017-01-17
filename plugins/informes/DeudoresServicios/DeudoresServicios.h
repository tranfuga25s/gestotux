#ifndef REPORTESCUOTAS_H
#define REPORTESCUOTAS_H

#include "einformeinterface.h"
#include <QtPlugin>

class DeudoresServicios : public QObject, public EInformeInterface
{
    Q_OBJECT
    Q_INTERFACES( EInformeInterface )

public:
    double version() const;
    QString nombre() const;
    bool inicializar();
    void hacerMenu( QMenu *m );

protected slots:
    void resumenServiciosDeudaHistorica();
    void resumenServiciosPorServicio();
    void resumenServiciosCliente();
    void seCierraGestotux(){}

signals:
    void agregarVentana( QWidget * );

private:
    QAction *ActResumenServiciosDeudaHistorica;
    QAction *ActResumenServiciosCliente;
    QAction *ActResumenServiciosMes;

    bool existenServicios();
};

#endif // REPORTESCUOTAS_H
