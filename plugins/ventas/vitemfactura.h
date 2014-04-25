#ifndef VITEMFACTURA_H
#define VITEMFACTURA_H

#include "evlista.h"
#include "mproductostotales.h"

class VItemFactura : public EVLista
{
    Q_OBJECT
public:
    explicit VItemFactura(QWidget *parent = 0);
    void setearIdFactura( const int id_factura );
    
private:
    MProductosTotales *modelo;
};

#endif // VITEMFACTURA_H
