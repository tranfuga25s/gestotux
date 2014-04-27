#include "formresumencomprasproveedor.h"

#include "resumencomprasproveedor.h"
#include "eactcerrar.h"

FormResumenComprasProveedor::FormResumenComprasProveedor(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    setObjectName("resumen_compras_proveedor");
    setWindowTitle("Resumen de compras por proveedor" );

    _modelo = new ResumenComprasProveedor( this );
    TVResumen->setModel( _modelo );
    _modelo->select();
    TVResumen->hideColumn( _modelo->fieldIndex( "id_proveedor" ) );
    TVResumen->hideColumn( _modelo->fieldIndex( "contado" ) );

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
