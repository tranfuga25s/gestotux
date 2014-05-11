#include "formprefcompras.h"

#include "preferencias.h"

FormPrefCompras::FormPrefCompras(QWidget *parent) :
    FormPrefComprasBase(), FormPrefHijo()
{
    this->setParent( parent );
    setupUi( this );
    this->setWindowTitle( "Compras" );
    this->setWindowIcon( QIcon( ":/imagenes/compras.png" ) );
}

/*!
 * \brief FormPrefCompras::aplicar
 */
void FormPrefCompras::aplicar()
{
}

/*!
 * \brief FormPrefCompras::cargar
 */
void FormPrefCompras::cargar()
{
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Compras" );

    CkBAutoAgregarProductos->setChecked( p->value( "auto-agregar-productos", false ).toBool() );

    p->endGroup();
    p->endGroup();
    p = 0;
}

/*!
 * \brief FormPrefCompras::guardar
 */
void FormPrefCompras::guardar()
{
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Cuotas" );

    p->setValue( "auto-agregar-productos", CkBAutoAgregarProductos->isChecked() );

    p->endGroup();
    p->endGroup();
    p = 0;
}
