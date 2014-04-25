#include "balancegeneral.h"
#include "EReporte.h"

#include <QDate>

double BalanceGeneral::version() const
{ return 1.0; }

QString BalanceGeneral::nombre() const
{ return "BalanceGeneral"; }

bool BalanceGeneral::inicializar()
{
    ActResumen = new QAction( this );
    ActResumen->setText( "Balance general" );
    ActResumen->setToolTip( "Genera un balance general de todo el sistema" );
    connect( ActResumen, SIGNAL( triggered() ), this, SLOT( hacerResumen() ) );

    return true;
}

void BalanceGeneral::hacerMenu( QMenu *m )
{
    m->addAction( ActResumen );
}

void BalanceGeneral::hacerResumen()
{
    EReporte *rep = new EReporte( 0 );
    ParameterList lista;
    rep->especial( "BalanceGeneral", lista );
    rep->hacerPDF( lista, QString( "Balance general al %1" ).arg( QDate::currentDate().toString( "dd-MM-yyyy" ) ) );
    delete rep;
}

Q_EXPORT_PLUGIN2( balance, BalanceGeneral )