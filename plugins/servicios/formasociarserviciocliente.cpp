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
#include "formasociarserviciocliente.h"

#include "emcliente.h"
#include "mservicios.h"
#include "mcuentacorriente.h"
#include "mclientes.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

FormAsociarServicioCliente::FormAsociarServicioCliente(QWidget* parent, tipoForm tipo, Qt::WFlags fl)
: QDialog( parent, fl ), Ui::Dialog()
{
        _tipo = tipo;
        setupUi(this);
        _id_cliente = _id_servicio = -1;
        _fecha = QDate::currentDate();
        CBCliente->setearAlertaNoExistentes( true );
        switch( tipo )
        {
                case Cliente:
                {
                        CBServicio->setVisible( false );
                        LServicio->setVisible( false );
                        CBCliente->setearFiltro( " WHERE id != 0" );
                        this->setWindowTitle( "Adherir cliente" );
                        break;
                }
                case Servicio:
                {
                        CBCliente->setVisible( false );
                        LCliente->setVisible( false );
                        this->setWindowTitle( "Adherir cliente a un servicio" );
                        break;
                }
                case SoloFecha:
                {
                        CBCliente->setVisible( false );
                        LCliente->setVisible( false );
                        CBServicio->setVisible( false );
                        LServicio->setVisible( false );
                        CBCliente->setearFiltro( " WHERE id != 0 " );
                        this->setWindowTitle( "Fecha de adhesion" );
                        break;
                }
                default:
                { break; }
        }
        DEFechaAlta->setDate( _fecha );
        DEFechaAlta->setMaximumDate( QDate::currentDate() );
        this->adjustSize();
        this->connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
        this->connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );
        _agregado = false;
}


/*!
 * \fn FormAsociarServicioCliente::reject()
 * Slot llamado cuando el usuario presiona el boton de cancelar
 */
void FormAsociarServicioCliente::reject()
{
  QDialog::reject();
}

/*!
 * \fn FormAsociarServicioCliente::accept()
 * Slot llamado cuando el usuario presiona el boton de aceptar
 */
void FormAsociarServicioCliente::accept()
{
 if( _agregado ) { return; }
 switch( this->_tipo )
 {
        case Cliente:
        {
                this->setIdCliente( CBCliente->idClienteActual() );
                break;
        }
        case Servicio:
        {
                this->setIdServicio( CBServicio->model()->data( CBServicio->model()->index( CBServicio->currentIndex(), 0 ) , Qt::EditRole ).toInt()  );
                break;
        }
        default:
        { break; }
 }
 _fecha = this->DEFechaAlta->date();
 // Autoagregado de clientes
 if( _id_cliente <= 0 && !CBCliente->currentText().isEmpty() ) {
    // Veo de trasladar los datos al cliente nuevo
    _id_cliente = MClientes::agregarClientePredeterminado( CBCliente->currentText() );
    if( _id_cliente == -1 ) {
        QMessageBox::information( this, "Error", "No se pudo agregar el cliente de manera automática. Por favor, agregelo manualmente e intente asociarlo nuevamente" );
        this->close();
        return;
    }
 }
 if( _id_cliente <= 0 || _id_servicio <= 0 || !_fecha.isValid() )
 {
     qDebug() << "Error de comprobación: id_cliente=" << _id_cliente << ", id_servicio=" << _id_servicio <<", fecha=" << _fecha.toString();
     qWarning() << "Existio un error de comprobancion de datos - no se adherirà el cliente";
     return;
 }
 if( _fecha < MServicios::getFechaAlta( _id_servicio ) ) {
     QMessageBox::information( this, "Error", "No se puede adherir un cliente en una fecha menor a la fecha de inicio del servicio." );
     return;
 }
 // Busco si el cliente tiene cuenta corriente
 if( MCuentaCorriente::obtenerNumeroCuentaCorriente( _id_cliente ) == QString::number( MCuentaCorriente::ErrorNumeroCuenta) ) {
     QMessageBox::information( this, "Creando nueva cuenta corriente", "El cliente no posee cuenta corriente, se le creara una automaticamente." );
     if( !MCuentaCorriente::agregarCuentaCorrientePredeterminada( _id_cliente, DEFechaAlta->dateTime() ) ) {
         QMessageBox::information( this, "Error", "Error al crear la cuenta corriente predeterminada" );
         return;
     }
 }
 MServicios *mservicios = new MServicios();
 if( mservicios->asociarCliente( _id_cliente, _id_servicio, DEFechaAlta->dateTime() ) )
 {
     // Verifico que se cobre lo que falta del período.
     if( CBInicial->isChecked() ) {
        if( !mservicios->calcularCobroAlta( _id_cliente, _id_servicio, DEFechaAlta->dateTime() ) ){
             QMessageBox::warning( this, "Error", "No se pudo hacer el cobro del periodo inical. Realicelo a mano." );
            _agregado = false;
        } else { _agregado = true; }
     } else {
        _agregado = true;
     }
     delete mservicios;
     if( _agregado ) {
         QMessageBox::information( this, "Correcto", QString::fromUtf8( "El cliente se adhirió al servicio correctamente" ) );
     }
     this->close();
 }
 else
 {
     delete mservicios;
     return;
 }
}


/*!
    \fn FormAsociarServicioCliente::idCliente()
        Devuelve el id del cliente
        @return int - id cliente en tabla clientes
 */
int FormAsociarServicioCliente::idCliente()
{ return _id_cliente; }


/*!
    \fn FormAsociarServicioCliente::idServicio()
        Devuelve el id del servicio
        @return int - id del servicio en tabla servicios
 */
int FormAsociarServicioCliente::idServicio()
{ return _id_servicio; }


/*!
    \fn FormAsociarServicioCliente::fecha()
        Devuelve la fecha de asociacion
        @return QDate - Fecha de asociacion
 */
QDate FormAsociarServicioCliente::fecha()
{ return _fecha; }


/*!
    \fn FormAsociarServicioCliente::setFecha( QDate fecha )
        Coloca la fecha de asociación
        @param fecha QDate con la fecha de asociación
 */
void FormAsociarServicioCliente::setFecha( QDate fecha )
{ this->_fecha = fecha; }


/*!
    \fn FormAsociarServicioCliente::setIdCliente( int id_cliente )
        Coloca el id de cliente al cual se quiere asociar
        @param id_cliente int - Id de tabla clientes
 */
void FormAsociarServicioCliente::setIdCliente( int id_cliente )
{
 if( id_cliente > 0 )
 {
  _id_cliente = id_cliente;
 }
}


/*!
    \fn FormAsociarServicioCliente::setIdServicio( int id_servicio )
        Coloca el id del servicio al cual se quiere asociar un cliente
        @param id_servicio int - id de tabla servicio
 */
void FormAsociarServicioCliente::setIdServicio( int id_servicio )
{
  if( id_servicio > 0 ) {
     _id_servicio = id_servicio;
  }
}


/*!
    \fn FormAsociarServicioCliente::exec()
 */
int FormAsociarServicioCliente::exec()
{
 switch( this->_tipo )
 {
    case Cliente:
    {
        // Tengo que eliminar todos los clientes que ya estan adheridos a este servicio
        CBCliente->setearFiltro( QString( "WHERE id NOT IN ( SELECT id_cliente FROM servicios_clientes WHERE id_servicio = %1 AND fecha_baja IS NULL ) AND id != 0 " ).arg( _id_servicio ) );
        break;
    }
    case Servicio:
    {
        // Tengo que eliminar todos los servicios a los que el cliente ya esta adherido
        QSqlQueryModel *modelo = new QSqlQueryModel( CBServicio );
        modelo->setQuery( QString( "SELECT id_servicio, nombre FROM servicios WHERE id_servicio NOT IN ( SELECT id_servicio FROM servicios_clientes WHERE id_cliente = %1 AND fecha_baja NOT NULL ) ORDER BY razon_social" ).arg( _id_cliente ) );
        CBServicio->setModel( modelo );
        CBServicio->setModelColumn( 1 );
    }
    default:
    {
            break;
    }
 }
 return QDialog::exec();
}
