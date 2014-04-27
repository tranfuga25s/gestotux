#ifndef REPORTECOMPRAS_H
#define REPORTECOMPRAS_H

#include "einformeinterface.h"
#include <QtPlugin>

class ReporteCompras : public QObject, public EInformeInterface
{
    Q_OBJECT
    Q_INTERFACES( EInformeInterface )

public:
    double version() const;
    QString nombre() const;
    bool inicializar();
    void hacerMenu( QMenu *m );

protected slots:
    void resumenComprasTotales();
    void resumenComprasPorProveedor();
    void resumenComprasPorProducto();
    void seCierraGestotux();

signals:
    void agregarVentana( QWidget * );

private:
    QAction *ActResumenComprasTotales;
    QAction *ActResumenComprasPorProveedor;
    QAction *ActResumenComprasPorProducto;

};

#endif // REPORTECOMPRAS_H
