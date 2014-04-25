#include "ecbtipooperacionordentrabajo.h"

ECBTipoOperacionOrdenTrabajo::ECBTipoOperacionOrdenTrabajo(QWidget *parent) :
    ECBTabla(parent)
{
    setearTabla( "tipo_operacion_orden_trabajo" );
    setearCampoId( "id_tipo_operacion_orden_trabajo" );
    setearCampoTexto( "nombre" );
    setearCampoOrden( "nombre" );
}
