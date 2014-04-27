#include "formresumencomprasproveedor.h"

#include "resumencomprasproveedor.h"

FormResumenComprasProveedor::FormResumenComprasProveedor(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    setObjectName("resumen_compras_proveedor");
    setWindowTitle("Resumen de compras por proveedor" );

    _modelo = new ResumenComprasProveedor( this );
    TVResumen->setModel( _modelo );
    _modelo->select();

    connect( CBProveedor, SIGNAL( cambioIdProveedor( int ) ), _modelo, SLOT( cambiarProveedor( int ) ) );

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
