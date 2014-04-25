#ifndef BALANCEGENERAL_H
#define BALANCEGENERAL_H

#include "einformeinterface.h"
#include <QtPlugin>

class BalanceGeneral : public QObject, public EInformeInterface
{
    Q_OBJECT
    Q_INTERFACES( EInformeInterface )

public:
    double version() const;
    QString nombre() const;
    bool inicializar();
    void hacerMenu( QMenu *m );
    
protected slots:
    void hacerResumen();
    void seCierraGestotux() {}

signals:
    void agregarVentana( QWidget * );

private:
    QAction *ActResumen;

};

#endif // BALANCEGENERAL_H
