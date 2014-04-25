#ifndef FORMLISTACOMPRA_H
#define FORMLISTACOMPRA_H

#include "evlista.h"
#include <QSqlQueryModel>
#include "mproductostotales.h"

class VListaCompra : public EVLista
{
    Q_OBJECT
public:
    explicit VListaCompra( QWidget *parent = 0 );
    void setearIdCompra( const int id_compra );

private:
    MProductosTotales *modelo;
};

#endif // FORMLISTACOMPRA_H
