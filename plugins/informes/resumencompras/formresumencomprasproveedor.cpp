#include "formresumencomprasproveedor.h"

#include "resumencomprasproveedor.h"
#include "eactcerrar.h"

FormResumenComprasProveedor::FormResumenComprasProveedor(QWidget *parent) :
    EVentana(parent)
{
    setupUi(this);
    setObjectName("resumen_compras_proveedor");
    setWindowTitle("Resumen de compras por proveedor" );

    _modelo = new ResumenComprasProveedor( this );
    TVResumen->setModel( _modelo );
    TVResumen->hideColumn( 6 ); // Proveedor
    TVResumen->hideColumn( 7 ); // Contado

    connect( CBProveedor, SIGNAL( cambioIdProveedor( int ) ), _modelo, SLOT( cambiarProveedor( int ) ) );

    this->addAction( new EActCerrar( this ) );
}

void FormResumenComprasProveedor::changeEvent(QEvent *e)
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
