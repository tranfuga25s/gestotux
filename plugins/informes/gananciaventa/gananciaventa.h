#ifndef GANANCIAVENTA_H
#define GANANCIAVENTA_H

#include "einformeinterface.h"
#include <QtPlugin>

class GananciaVenta : public QObject, public EInformeInterface
{
    Q_OBJECT
    Q_INTERFACES( EInformeInterface )

public:
    double version() const;
    QString nombre() const;
    bool inicializar();
    void hacerMenu( QMenu *m );
    
protected slots:
    void hacerResumenTotal();
    void hacerResumenFechas();
    void hacerResumenDia();
    void seCierraGestotux() {}

signals:
    void agregarVentana( QWidget * );

private:
    QAction *ActResumenTotal;
    QAction *ActResumenFechas;
    QAction *ActResumenDia;

};

class MFactura {
public:
    static QDate obtenerFechaMinimaVenta();
    static QDate fechaUltimaVenta();
};

class MCompra {
public:
    static QDate obtenerFechaMinimaCompra();
    static QDate obtenerFechaMaximaCompra();
};

#endif // BALANCEGENERAL_H
