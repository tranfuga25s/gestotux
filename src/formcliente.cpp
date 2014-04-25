/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller  				   *
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
#include "formcliente.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>

#include "eactcerrar.h"
#include "eactguardar.h"
#include "mclientes.h"
#include "mestadofiscal.h"
#include "preferencias.h"

FormCliente::FormCliente ( QWidget* parent, QSqlRelationalTableModel *modelo,  Qt::WFlags fl )
: EVentana( parent, fl ), Ui::FormClienteBase()
{
        setupUi ( this );
        setObjectName( "visorCliente" );
        setWindowTitle( "Visor de Cliente" );
        setWindowIcon( QIcon( ":/imagenes/clientes.png" ) );

        this->modelo = modelo;

        mapa = new QDataWidgetMapper( this );
        mapa->setOrientation( Qt::Horizontal );
        mapa->setModel( modelo );
        mapa->setSubmitPolicy( QDataWidgetMapper::ManualSubmit );

        // Coloco los mappeos
        mapa->addMapping( LENumCliente, modelo->fieldIndex( "id" ) );
        mapa->addMapping( LERazonSocial, modelo->fieldIndex( "razon_social"  ) );
        mapa->addMapping( LENombre, modelo->fieldIndex( "nombre" ) );
        mapa->addMapping( LEApellido, modelo->fieldIndex( "apellido" ) );

        mapa->addMapping( LECalle, modelo->fieldIndex( "calle" ) );
        mapa->addMapping( LENumero, modelo->fieldIndex( "numero" ) );
        mapa->addMapping( LEPiso, modelo->fieldIndex( "piso" ) );
        mapa->addMapping( LEDepto, modelo->fieldIndex( "depto" ) );
        mapa->addMapping( LECiudad, modelo->fieldIndex( "ciudad" ) );
        mapa->addMapping( LECodPostal, modelo->fieldIndex( "codigo_postal" ) );
        mapa->addMapping( CBProvincia, modelo->fieldIndex( "provincia" ) );
        mapa->addMapping( CBPaises, modelo->fieldIndex( "pais" ) );

        mapa->addMapping( LECUITCUIL, modelo->fieldIndex( "CUIT/CUIL" ) );
        mapa->addMapping( CBEstadoFiscal, modelo->fieldIndex( "id_estado_fiscal" ), "currentIndex" );

        mapa->addMapping( LETelFijo, modelo->fieldIndex( "tel_fijo" ) );
        mapa->addMapping( LETelCel, modelo->fieldIndex( "tel_celular" ) );
        mapa->addMapping( LEFax, modelo->fieldIndex( "fax" ) );

        mapa->addMapping( LEEmail, modelo->fieldIndex( "email" ) );
        mapa->addMapping( CkBComprobanteEmail, modelo->fieldIndex( "comprobante_email" ) );

        connect( LENombre       , SIGNAL( textChanged( const QString & ) ), this, SLOT( rehaceRazonSocial( const QString & ) ) );
        connect( LEApellido     , SIGNAL( textChanged( const QString & ) ), this, SLOT( rehaceRazonSocial( const QString & ) ) );

        // Cargo los estados fiscales
        _mestadofiscal = new MEstadoFiscal( CBEstadoFiscal );
        CBEstadoFiscal->setModel( _mestadofiscal );
        CBEstadoFiscal->setModelColumn( 1 );
        _mestadofiscal->select();
        CBEstadoFiscal->setCurrentIndex( -1 );

        //Acciones predefinidas
        EActGuardar *ActGuardar = new EActGuardar( this );
        this->addAction( ActGuardar );
        EActCerrar *ActCerrar = new EActCerrar( this );
        this->addAction( ActCerrar );

        CkBComprobanteEmail->setVisible( false );

        CBProvincia->setearCBPais( CBPaises );

        preferencias *p = preferencias::getInstancia();
        p->beginGroup( "Preferencias" );
        p->beginGroup( "Clientes" );
        CBPaises->setearId( p->value( "pais", -1 ).toInt() );
        CBProvincia->setearId( p->value( "provincia", -1 ).toInt() );
        CBEstadoFiscal->setCurrentIndex( p->value( "estado-fiscal", 4 ).toInt() );
        p->endGroup();
        p->endGroup();
        p=0;

}

/*!
    \fn FormCliente::guardar()
 */
void FormCliente::guardar()
{
 // Chequeo los datos que no pueden ser nulos
 if( LERazonSocial->text().isNull() || LERazonSocial->text().isEmpty() )
 {
  QMessageBox::warning( this, "Faltan Datos", "Por favor ingrese minimamente una razon social para el cliente" );
  return;
 }
 // Si existe un cuit-cuil verifico que de la suma

 if( !LECUITCUIL->text().isEmpty() ) {
     if( !LECUITCUIL->verificar() ) {
         QMessageBox::warning( this, "Error Cuit/Cuil", QString::fromUtf8("La verificación del cuit/cuil del cliente es incorrecta. Verifique el numero ingresado" ) );
         return;
     }
 }
 if( CBEstadoFiscal->currentIndex() == -1 ) {
     QMessageBox::warning( this, "Error estado fiscal", "Por favor, seleccione un estado fiscal para el cliente." );
     return;
 }
 //
 if( CBPaises->currentIndex() == -1 ) {
     QMessageBox::warning( this, "Error de pais", "Por favor, ingrese un pais para el cliente" );
     return;
 }
 if( CBProvincia->currentIndex() == -1 ) {
     QMessageBox::warning( this, "Error de provincia", "Por favor, ingrese una provincia para el cliente" );
     return;
 }
 if( !this->_agregando ) {
     // Veo si hay modificaciones
     if( mapa->submit() ) {
        QMessageBox::information( this, "Correcto", "Datos guardados correctamente" );
        this->close();
        return;
     } else {
        qWarning( "No se pudieron guardar los datos" );
        qDebug() << "Error, no se pudo hacer submit del mapa";
        qDebug() << this->modelo->lastError().text();
     }
 } else {
     // Verifico si existe ya
     if( MClientes::existe( LERazonSocial->text(), LENombre->text() ) ) {
         QMessageBox::warning( this, "Error", "Ya existe un cliente que coincide con la razon social y/o el nombre suministrados" );
         return;
     }
     QSqlTableModel::EditStrategy _anterior = modelo->editStrategy();
     modelo->setEditStrategy( QSqlTableModel::OnManualSubmit );
     QSqlRecord rc = modelo->record();
     rc.setValue( "razon_social", LERazonSocial->text() );
     ///////////////////////////////////////////////////////////////////////
     if( LENombre->text().isEmpty() )
     { rc.setNull( "nombre" ); }
     else
     { rc.setValue( "nombre", LENombre->text() ); }
     ///////////////////////////////////////////////////////////////////////
     if( LEApellido->text().isEmpty() )
     { rc.setNull( "apellido" ); }
     else
     { rc.setValue( "apellido", LEApellido->text() ); }
     ////////////////////////////////////////////////////////////////////////
     if( LECalle->text().isEmpty() )
     { rc.setNull( "calle" ); }
     else
     { rc.setValue( "calle", LECalle->text() ); }
     ////////////////////////////////////////////////////////////////////////
     if( LENumero->text().isEmpty() )
     { rc.setNull( "numero" ); }
     else
     { rc.setValue( "numero", LENumero->text() ); }
     ////////////////////////////////////////////////////////////////////////
     if( LEPiso->text().isEmpty() )
     { rc.setNull( "piso" ); }
     else
     { rc.setValue( "piso", LEPiso->text() ); }
     ////////////////////////////////////////////////////////////////////////
     if( LEDepto->text().isEmpty() )
     { rc.setNull( "depto" ); }
     else
     { rc.setValue( "depto", LEDepto->text() ); }
     ////////////////////////////////////////////////////////////////////////
     if( LECiudad->text().isEmpty() )
     { rc.setNull( "ciudad" ); }
     else
     { rc.setValue( "ciudad", LECiudad->text() ); }
     ////////////////////////////////////////////////////////////////////////
     if( LECodPostal->text().isEmpty() )
     { rc.setNull( "codigo_postal" ); }
     else
     { rc.setValue( "codigo_postal", LECodPostal->text() ); }
     ////////////////////////////////////////////////////////////////////////
     if( CBProvincia->currentIndex() > 0 )
     { rc.setNull( "provincia" ); }
     else
     { rc.setValue( "provincia", CBProvincia->currentText() ); }
     ////////////////////////////////////////////////////////////////////////
     if( CBPaises->currentIndex() > 0  )
     { rc.setNull( "pais" ); }
     else
     { rc.setValue( "pais", CBPaises->currentText() ); }
     ////////////////////////////////////////////////////////////////////////
     if( LETelFijo->text().isEmpty() )
     { rc.setNull( "tel_fijo" ); }
     else
     { rc.setValue( "tel_fijo", LETelFijo->text() ); }
     ////////////////////////////////////////////////////////////////////////
     if( LETelCel->text().isEmpty() )
     { rc.setNull( "tel_celular" ); }
     else
     { rc.setValue( "tel_celular", LETelCel->text() ); }
     ////////////////////////////////////////////////////////////////////////
     if( LEFax->text().isEmpty() )
     { rc.setNull( "fax" ); }
     else
     { rc.setValue( "fax", LEFax->text() ); }
     ////////////////////////////////////////////////////////////////////////
     if( LEEmail->text().isEmpty() )
     { rc.setNull( "email" ); }
     else
     { rc.setValue( "email", LEEmail->text() ); }
     ////////////////////////////////////////////////////////////////////////
     rc.setValue( "comprobante_email", CkBComprobanteEmail->isChecked() );
     ////////////////////////////////////////////////////////////////////////
     if( LECUITCUIL->text().isEmpty() )
     { rc.setNull("CUIT/CUIL"); }
     else
     { rc.setValue( "CUIT/CUIL", LECUITCUIL->text() ); }
     ////////////////////////////////////////////////////////////////////////
     if( CBEstadoFiscal->currentIndex() == -1 ) {
         rc.setNull( "id_estado_fiscal" );
     } else {
         rc.setValue( "id_estado_fiscal", CBEstadoFiscal->model()->data( CBEstadoFiscal->model()->index( CBEstadoFiscal->currentIndex(), 0 ), Qt::EditRole ).toInt() );
     }
     ////////////////////////////////////////////////////////////////////////
     // Inserto los datos
     if( modelo->insertRecord( -1, rc ) )
     {
      if( modelo->submitAll() )
      {
       QMessageBox::information( this, "Correcto", QString( "El cliente %1 se guardo correctamente" ).arg( modelo->query().lastInsertId().toInt() ) );
       modelo->setEditStrategy( _anterior );
       close();
       return;
      }
      else
      {
       qWarning( "Error al hacer submit del modelo de datos de cliente" );
       qWarning() << modelo->lastError().text();
       qWarning() << modelo->query().lastQuery();
      }
     }
     else
     {
       qWarning( "Error al insertar el registro de datos de cliente" );
       qWarning() << modelo->lastError().text();
       qWarning() << modelo->query().lastQuery();
     }
     modelo->setEditStrategy( _anterior );
 } // Fin agregando
}

/*!
  \fn FormCliente::agregar()
    Setea el formulario para agregar un registro
 */
void FormCliente::agregar() {
    this->_agregando = true;
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Clientes" );
    this->CBEstadoFiscal->setCurrentIndex( p->value( "estado-fiscal", 0 ).toInt() );
    this->CBPaises->setearId( p->value( "pais", 0 ).toInt() );
    this->CBProvincia->setearId( p->value( "provincia", 0 ).toInt() );
    p->endGroup();
    p->endGroup();
    p = 0;
}

/*!
    \fn FormCliente::setearCliente( QModelIndex &indice )
 */
void FormCliente::setearCliente( QModelIndex &indice )
{
    // Cargo todos los datos del cliente.
    mapa->setCurrentIndex( indice.row() );
    _agregando = false;
}

void FormCliente::setearCliente( int id_cliente )
{
    qWarning() << "No implementado!";
    this->close();
    return;
}


/*!
    \fn FormCliente::rehaceRazonSocial( const QString &texto )
 */
void FormCliente::rehaceRazonSocial( const QString &texto )
{
 // Este slot siempre se llama por diferencia de 1 caracter o texto vacio.
 if( texto.isEmpty() )
 { return; }
 QString anterior = texto;
 anterior.remove( texto.length()-1, 1 );
 if( LERazonSocial->text().isEmpty() )
 {
    LERazonSocial->setText( LEApellido->text() + ", " + LENombre->text() );
 } else {
     if( LERazonSocial->text().contains( anterior, Qt::CaseInsensitive )  ) {
         LERazonSocial->setText( LEApellido->text() + ", " + LENombre->text() );
     }
 }
 /// \todo Arreglar esto
 return;
}


bool FormCliente::verificarCuitCuil( QString texto ) {
    // Separo el codigo de verificación
    QStringList partes = texto.split( "-" );
    // 1 -  primera cifra
    // 2 -  numero de doc o ref
    // 3 -  verificacion
    int verf = partes.at( 2 ).toInt();
    QString seg = partes.at( 0 );
    seg.append( partes.at( 1 ) );
    int a[10] = { 5, 4, 3, 2, 7, 6, 5, 4, 3, 2 };
    int res = 0;
    for( int i = 0; i<seg.size(); i++ ) {
        res += seg.at(i).digitValue() * a[i];
    }
    int resto = res % 11;
    if( resto == 0 ) {
        if( verf == resto ) {
            return true;
        } else {
            return false;
        }
    } else  if( resto == 1 ) {
        if( verf == 9 ) {
            return true;
        } else {
            return false;
        }
    } else  if( ( 11 - resto ) == verf ) {
        return true;
    } else {
        return false;
    }
}
