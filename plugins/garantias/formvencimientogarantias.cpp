#include "formvencimientogarantias.h"

#include <QDate>

#include "eactcerrar.h"

FormVencimientoGarantias::FormVencimientoGarantias(QWidget *parent) :
EVentana(parent)
{
    setupUi(this);
    this->setWindowTitle( "Vencimientos de Garantias" );
    this->setWindowIcon( QIcon( ":/imagenes/garantia_vencimiento.png" ) );

    mgarantias = new MVGarantias( this );

    TVVencimientos->setModel( mgarantias );

    connect( DEFecha, SIGNAL( dateChanged( QDate ) ), this, SLOT( cambioFecha( QDate ) ) );
    connect( CkBBaja, SIGNAL( toggled( bool ) ), this, SLOT( cambioBaja( bool ) ) );

    DEFecha->setDate( QDate::currentDate() );

    ActCerrar = new EActCerrar( this );

    addAction( ActCerrar );

}

/*!
 * \brief FormVencimientoGarantias::cambioFecha
 * \param fecha
 */
void FormVencimientoGarantias::cambioFecha( QDate fecha )
{
    QString filtro;
    filtro.append( " fecha_fin <= %1" ).arg( fecha.toString( "Y-m-d" ) );
    if( CkBBaja->isChecked() ) {
        filtro.append( " AND fecha_baja IS NOT NULL " );
    } else {
        filtro.append( " AND fecha_baja IS NULL" );
    }
    mgarantias->setFilter( filtro );
    mgarantias->select();
}

/*!
 * \brief FormVencimientoGarantias::cambioBaja
 * \param estado
 */
void FormVencimientoGarantias::cambioBaja( bool )
{
    cambioFecha( DEFecha->date() );
}

/*!
 * \brief FormVencimientoGarantias::changeEvent
 * \param e
 */
void FormVencimientoGarantias::changeEvent(QEvent *e)
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
