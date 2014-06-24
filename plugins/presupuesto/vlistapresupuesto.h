#ifndef VLISTAPRESUPUESTO_H
#define VLISTAPRESUPUESTO_H

#include "MItemPresupuesto.h"
#include "evlista.h"
class MProductosTotales;

class VListaPresupuesto : public EVLista
{
    Q_OBJECT
public:
    VListaPresupuesto(QWidget *parent = 0);
    void setearIdPresupuesto( const int id_presupuesto );

private:
    MProductosTotales *modelo;
};

#endif // VLISTAPRESUPUESTO_H
