#include "ecbmodeloequipamiento.h"

ECBModeloEquipamiento::ECBModeloEquipamiento( QWidget *parent ) :
ECBTabla(parent)
{
    this->setObjectName("CBModelosEquipamiento");
    this->setearTabla( "equipamiento" );
    this->setearCampoId( "DISTINCT modelo" );
    this->setearCampoTexto( "modelo" );
    this->setearCampoOrden( "modelo" );
}
