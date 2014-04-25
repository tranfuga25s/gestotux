#ifndef VITEMREMITO_H
#define VITEMREMITO_H

#include "evlista.h"
#include "mproductostotales.h"

class VItemRemito : public EVLista
{
    Q_OBJECT
public:
    explicit VItemRemito(QWidget *parent = 0);
    void setearIdRemito( const int id_remito );
    
private:
    MProductosTotales *modelo;
};

#endif // VITEMFACTURA_H
