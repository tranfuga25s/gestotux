/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   				   *
 *   juiraze@yahoo.com.ar   						   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "formnuevactacte.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRelationalTableModel>
#include "mcuentacorriente.h"
#include "preferencias.h"

FormNuevaCtaCte::FormNuevaCtaCte ( QWidget* parent, Qt::WFlags fl )
 : QDialog( parent, fl ), Ui::FormCtaCteBase()
{
 setupUi ( this );
 this->setWindowTitle( "Agregar nueva cuenta corriente" );
 this->setWindowIcon( QIcon( ":/imagenes/ctacte_nueva.png" ) );
 //this->setAttribute( Qt::WA_DeleteOnClose );

 DEFechaAlta->setDate( QDate::currentDate() );

  // lleno el combo de los clientes sin cuenta corriente
 CBCliente->setearFiltro( "WHERE id NOT IN ( SELECT id_cliente FROM ctacte ) AND id != 0" );
 connect( CBCliente, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioCliente( int ) ) );

 DEFechaBaja->setEnabled( CkBBaja->isChecked() );
 DSBLimiteMaximo->setEnabled( CkBBaja->isChecked() );

 DEFechaBaja->setDate( QDate::currentDate() );
}

void FormNuevaCtaCte::cambioCliente( int /*id_combo*/ )
{
    // Busco la forma de cuenta corriente y la convierto segun sean las preferencias
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "ctacte" );
    QString formato = p->value( "mascara-ctacte", "%1" ).toString();
    p->endGroup();p->endGroup();p=0;
    // Busco el id del cliente y lo paso por la mascara
    int id_cliente = CBCliente->idClienteActual();
    LENumeroCuenta->setText( QString( "#" ).append( formato ).arg( id_cliente ) );
}

/*!
    \fn FormNuevaCtaCte::accept()
 */
void FormNuevaCtaCte::accept()
{
 //verificar los datos
 if( CBCliente->currentIndex() == -1 )
 {
     QMessageBox::warning( this, "Error", "Por favor, seleccione un cliente para agregarle su cuenta corriente." );
     return;
 }
 if( LENumeroCuenta->text().isEmpty() )
 {
     QMessageBox::warning( this, "Error", "Por favor, ingrese un numero de cuenta." );
     return;
 }
 if( CkBBaja->isChecked() && DEFechaBaja->date().isValid() && DEFechaBaja->date() <= DEFechaAlta->date() )
 {
     QMessageBox::warning( this, "Error", "La fecha de baja de la cuenta corriente es anterior a la fecha de alta!" );
     return;
 }
 if( CkBLimiteMaximo->isChecked() && DSBLimiteMaximo->value() <= 0 )
 {
     QMessageBox::warning( this, "Error", "El limite de la cuenta es igual o menor que cero. Si desea deshabilitarlo, destilde la casilla que se encuentra al lado del titulo" );
     return;
 }

 // Datos teoricamente correctos
 int id_cliente = CBCliente->idClienteActual();
 if( id_cliente <= 0 ) {
     qWarning( "Error de id de cuenta corriente" );
     abort();
 }
 // Saco el # del numero de cuenta
 LENumeroCuenta->setText( LENumeroCuenta->text().remove( "#" ) );
 QDateTime fecha_baja = QDateTime();
 if( CkBBaja->isChecked() )
    fecha_baja = DEFechaBaja->dateTime();
 double limite = -1;
 if( CkBLimiteMaximo->isChecked() )
     limite = DSBLimiteMaximo->value();
 // Guardo la nueva cuenta corriente
 if( modelo->agregarCuentaCorriente(
             id_cliente,
             DEFechaAlta->dateTime(),
             fecha_baja,
             limite,
             LENumeroCuenta->text() ) ) {
     QMessageBox::information( this, "Correcto", "La cuenta corriente fue agregada correctamente" );
     modelo->select();
     this->close();
     //QDialog::accept();
  } else {
     QMessageBox::warning( this, "Error", "No se pudo agregar la cuenta coriente" );
     QDialog::reject();
  }
}


/*!
    \fn FormNuevaCtaCte::setModelo( MCuentaCorriente *m )
 */
void FormNuevaCtaCte::setModelo( MCuentaCorriente *m )
{
 if( m != 0 )
 { modelo = m; }
}
