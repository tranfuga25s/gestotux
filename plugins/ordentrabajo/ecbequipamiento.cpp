#include "ecbequipamiento.h"

ECBEquipamiento::ECBEquipamiento( QWidget *parent ) :
ECBTabla(parent)
{
    this->setObjectName("CBEquipamiento");
    this->setearTabla( "equipamiento" );
    this->setearCampoId( "id_equipamiento" );
    this->setearCampoTexto( "descripcion" );
    this->setearCampoOrden( "descripcion" );
}
