#include "formprefgarantias.h"

#include "preferencias.h"

FormPrefGarantias::FormPrefGarantias(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    this->setWindowTitle( QString::fromUtf8( "Garantías" ) );
    this->setWindowIcon( QIcon( ":/imagenes/garantia.png" ) );
}

void FormPrefGarantias::changeEvent(QEvent *e)
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

void FormPrefGarantias::cargar()
{
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Garantias" );
    SBDuracion->setValue( p->value( "duracion_garantia", 6 ).toInt() );
    p->endGroup();
    p->endGroup();
    p=0;
}

void FormPrefGarantias::guardar()
{
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Garantias" );
    p->setValue( "duracion_garancia", SBDuracion->value() );
    p->endGroup();
    p->endGroup();
    p=0;
}
