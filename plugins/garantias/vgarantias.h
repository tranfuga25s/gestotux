#ifndef VGARANTIAS_H
#define VGARANTIAS_H

#include "evlista.h"
class MVGarantias;
class MGarantias;

class VGarantias : public EVLista
{
    Q_OBJECT
public:
    VGarantias( QWidget *parent = 0 );

protected slots:
    void agregar( bool autoeliminarid );
    void imprimir();
    void aPdf();
    void eliminar();
    void darBaja();
    void cambioBaja( bool estado );

private:
    MGarantias *modelo;
    MVGarantias *mgarantias;
    QAction *ActDarBaja;
    QAction *ActVerBaja;
    
};

#endif // VGARANTIAS_H
