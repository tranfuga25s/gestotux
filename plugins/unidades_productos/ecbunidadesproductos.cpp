#include "ecbunidadesproductos.h"

ECBUnidadesProductos::ECBUnidadesProductos(QWidget *parent) :
    ECBTabla(parent)
{
    setearTabla( "unidades_productos" );
    setearCampoId( "id_unidad" );
    setearCampoTexto( "nombre" );
    setearCampoOrden( "nombre" );
    setearFiltro( " WHERE id_padre = 0 " );
}
