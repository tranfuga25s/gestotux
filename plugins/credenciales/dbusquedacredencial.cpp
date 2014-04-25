#include "dbusquedacredencial.h"
#include <QDialogButtonBox>
#include <QPushButton>

DBusquedaCredencial::DBusquedaCredencial(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setWindowTitle( "Buscar credencial" );
    this->buttonBox->button( QDialogButtonBox::Ok )->setText( "Buscar" );
}

void DBusquedaCredencial::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}


void DBusquedaCredencial::accept() {
    QString filtro; bool necesita_and = false;
    if( SBNumero->value() != 0 ) {
        filtro.append( QString( "id_credencial = %1" ).arg( SBNumero->value() ) );
        necesita_and = true;
    }
    if( !LENombre->text().isEmpty() ) {
        if( necesita_and ) { filtro.append( " AND " ); }
        filtro.append( QString( " nombre LIKE \"%1\"" ).arg( LENombre->text() ).toLocal8Bit() );
        necesita_and = true;
    }
    if( !LEMedio->text().isEmpty() ) {
        if( necesita_and ) { filtro.append( " AND " ); }
        filtro.append( QString( " medio  LIKE \"%1\"" ).arg( LEMedio->text() ) );
        necesita_and = true;
    }
    // Agregar Accesos
    qDebug( QString( " cola: %1" ).arg( filtro ).toLocal8Bit() );
    this->modelo->setFilter( filtro );
    this->modelo->select();
}

void DBusquedaCredencial::reject() {
    this->modelo->setFilter( "" );
    this->modelo->select();
    QDialog::reject();
}

void DBusquedaCredencial::setearModelo(MCredenciales *m)
{ if( m != 0 ) {  this->modelo = m; } }
