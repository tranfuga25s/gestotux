#include "formprefclientes.h"

#include "mestadofiscal.h"

FormPrefClientes::FormPrefClientes(QWidget *parent) :
    EVentana( parent ), FormPrefClientesBase()
{
    setupUi(this);
    CBProvincia->setearCBPais( CBPais );
    MEstadoFiscal *mef = new MEstadoFiscal( CBEstadoFiscal );
    CBEstadoFiscal->setModel( mef );
    CBEstadoFiscal->setModelColumn(1);
    mef->select();
}

void FormPrefClientes::cargar()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Clientes" );
    CBProvincia->setearId( p->value( "provincia", 0 ).toInt() );
    CBPais->setearId( p->value( "pais", 0 ).toInt() );
    CBEstadoFiscal->setCurrentIndex( p->value( "estado-fiscal", 0 ).toInt() );
    p->endGroup();
    p->endGroup();
    p = 0;
}

void FormPrefClientes::guardar()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Clientes" );
    p->setValue( "provincia", CBProvincia->idActual() );
    p->setValue( "pais", CBPais->idActual() );
    p->setValue( "estado-fiscal", CBEstadoFiscal->currentIndex() );
    p->endGroup();
    p->endGroup();
    p = 0;
}

void FormPrefClientes::aplicar()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Clientes" );
    p->setValue( "provincia", CBProvincia->idActual() );
    p->setValue( "pais", CBPais->idActual() );
    p->setValue( "estado-fiscal", CBEstadoFiscal->currentIndex() );
    p->endGroup();
    p->endGroup();
    p = 0;
}

void FormPrefClientes::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}
