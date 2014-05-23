#include "formagregarproveedor.h"

#include <QMessageBox>
#include <QDebug>

FormAgregarProveedor::FormAgregarProveedor( MProveedor *m, QWidget *parent) :
    EVentana(parent), Ui::FormProveedorBase()
{
    setupUi(this);
    setObjectName( "agregarproveedor" );
    setWindowTitle( "Agregar Proveedor" );

    this->modelo = m;

    this->GBTitulo->setTitle( "Agregar nuevo proveedor" );

    addAction( new EActGuardar( this ) );
    addAction( new EActCerrar( this ) );
}

void FormAgregarProveedor::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include "ELECuitCuil.h"
void FormAgregarProveedor::guardar() {
    if( LERazonSocial->text().isEmpty() ) {
        QMessageBox::information( this, "Error", "Por favor, ingrese una razon social para el proveedor" );
        return;
    }
    // Verifico el Cuit
    if( !LECUIT->verificar() ) {
        QMessageBox::information( this, "Error", "Por favor, ingrese una razon CUIT/CUIL valido para el proveedor" );
        return;
    }
    if( MProveedor::existeProveedor( LERazonSocial->text() ) ) {
        QMessageBox::information( this, "Error", "El proveedor que esta intentando agregar ya existe." );
        return;
    }
    if( MProveedor::existeCuitCuil( LECUIT->text() ) ) {
        QMessageBox::information( this, "Error", "El proveedor que esta intentando agregar ya existe." );
        return;
    }
    QSqlRecord rec = this->modelo->record();
    rec.setValue( "nombre", LERazonSocial->text() );
    rec.setValue( "direccion", TEDireccion->toPlainText() );
    rec.setValue( "cuit_cuil", LECUIT->text() );
    rec.setValue( "telefono_linea", LETelFijo->text() );
    rec.setValue( "telefono_celular", LETelCel->text() );
    rec.setValue( "fax", LEFax->text() );
    rec.setValue( "email", LEEmail->text() );
    if( this->modelo->insertRecord( -1, rec ) ) {
        QMessageBox::information( this, "Correcto", QString( "El proveedor %1 se inserto correctamente" ).arg( this->modelo->query().lastInsertId().toInt() ) );
        this->close();
        return;
    } else {
        qDebug( "Error al insertar el registro" );
        qDebug() << modelo->lastError().text();
    }
}
