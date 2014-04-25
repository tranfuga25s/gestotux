#include "ecbtecnicos.h"

ECBTecnicos::ECBTecnicos( QWidget *parent ) :
ECBTabla( parent )
{
    this->setObjectName("CBTecnicos");
    this->setearTabla( "tecnico" );
    this->setearCampoId( "id_tecnico" );
    this->setearCampoTexto( "razon_social" );
    this->setearCampoOrden( "razon_social" );
    this->setearFiltro( " WHERE habilitado = \"true\" " );
}
