#ifndef ECBCATEGORIASPRODUCTOS_H
#define ECBCATEGORIASPRODUCTOS_H

#include "ecbtabla.h"

class ECBCategoriasProductos : public ECBTabla
{
    Q_OBJECT
public:
    ECBCategoriasProductos( QWidget *parent = 0 ) : ECBTabla( parent, "categoria_producto", "id", "nombre", "nombre" ) {}
    
};

#endif // ECBCATEGORIASPRODUCTOS_H
