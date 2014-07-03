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
#include "formagregarrecibo.h"
#include "emcliente.h"
#include "preferencias.h"
#include "eregistroplugins.h"
#include "../CtaCte/mcuentacorriente.h"
#include "eactcerrar.h"
#include "eactguardar.h"
#include "mpagos.h"

#include <QMessageBox>

FormAgregarRecibo::FormAgregarRecibo ( QWidget* parent, Qt::WFlags fl )
: EVentana( parent, fl ), Ui::FormReciboBase()
{
        setupUi( this );

        setObjectName( "agregar_recibo" );
        setWindowTitle( "Nuevo recibo" );
        setWindowIcon( QIcon( ":/imagenes/recibo-nuevo.png" ) );

        connect( CBCliente, SIGNAL( cambioIdCliente( int ) ), this, SLOT( cambioCliente( int ) ) );
        connect( CBCliente, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioCliente( int ) ) );
        connect( dSBPagado, SIGNAL( valueChanged( double ) ), this, SLOT( cambioPagado( double ) ) );

        this->addAction( new EActGuardar( this ) );
        this->addAction( new EActCerrar( this ) );

        // Seteo la fecha a la de hoy
        this->DEFecha->setDate( QDate::currentDate() );

        _hicomp = false;
        preferencias *p = preferencias::getInstancia();
        p->inicio();
        p->beginGroup( "carga" );
        if( p->value( "pluginInfo" ).toString() != "hicomp" ) {
            RBLuego->setChecked( false );
            RBLuego->setVisible( false );
        } else {
            _hicomp = true;
        }
        p->endGroup();
        p->inicio();
        p->beginGroup( "Preferencias" );
        p->beginGroup( "CtaCte" );
        _ctacte_habilitada = p->value( "habilitada" ).toBool();
        p->endGroup();
        p->endGroup();
        p = 0;

        dSBDeuda->setVisible( false );
        dSBTotal->setVisible( false );
        LSaldo->setVisible( false );
        LDeuda->setVisible( false );
}

#include "NumeroComprobante.h"
/*!
  \fn FormAgregarRecibo::setearModelo( MPagos *m )
  Setea el modelo que se utilizará para agregar los datos  que se actualize en la vista
  @param m Modelo de pagos ( clase #MPagos )
 */
void FormAgregarRecibo::setearModelo( MPagos *m )
{
    if( m!=0 ) {
        this->_modelo = m;
    } else {
        this->_modelo = new MPagos( this );
    }
    // Busco el ultimo numero de recibo
    NumeroComprobante numero = this->_modelo->proximoSerieNumeroRecibo();
    this->LNumero->setText( QString::fromUtf8( "Número de Recibo <b>" ).append( numero.aCadena() ).append( "</b>" ) );
}

/*!
 * \fn FormAgregarRecibo::setearDatos( const int id_cliente, const QString texto, const double cantidad )
 * Coloca en forma un recibo con los datos pasados como parametros
 * \param id_cliente Identificador del cliente al que queremos hacerle el recibo
 * \param texto Texto que va a tener el recibo
 * \param cantidad Cantidad que debe tener el recibo
 * \param fecha Fecha en que debe estar inicializado el recibo
 */
void FormAgregarRecibo::setearDatos( const int id_cliente, const QString texto, const double cantidad )
{
    // No uso fecha porque el recibo se emite con la fecha en que se paga
    if( this->_modelo )
    { this->setearModelo( 0 ); }
    this->TETexto->setText( texto );
    this->dSBPagado->setValue( cantidad );
    this->CBCliente->setearId( id_cliente );
}

/*!
    \fn FormAgregarRecibo::recalcularTotal()
    Recalcula el total debido por el cliente si posee deuda
 */
void FormAgregarRecibo::recalcularTotal()
{
    if( dSBDeuda->value() > 0 ) {
        if( RBLuego->isChecked() ) {
            dSBTotal->setValue( dSBDeuda->value() + dSBPagado->value() );
        } else {
            dSBTotal->setValue( dSBDeuda->value() - dSBPagado->value() );
        }
    } else {
        dSBTotal->setValue( dSBPagado->value() );
    }

}


/*!
    \fn FormAgregarRecibo::cambioCliente( int id_combo )
    Slot llamado cuando se cambia o seleccióna un nuevo cliente.
 */
void FormAgregarRecibo::cambioCliente( int id_combo )
{
 // Si es un cliente existente veo si tiene saldo y si puede hacer recibos a pagar luego
 if( _ctacte_habilitada && ERegistroPlugins::getInstancia()->existePluginExterno( "ctacte" ) )
 {
  id_combo = this->CBCliente->idClienteActual();
  if( id_combo == 0 ) {
     // El numero indica Consumidor Final
     //qDebug( "FormAgregarRecibo::cambioCliente::Se eligio consumidor final" );
  } else {
    QString numero_cuenta =  MCuentaCorriente::obtenerNumeroCuentaCorriente( this->CBCliente->idClienteActual() );
    if( numero_cuenta == QString::number( MCuentaCorriente::ErrorBuscarLimite )
     || numero_cuenta == QString::number( MCuentaCorriente::ErrorNumeroCuenta )
     || numero_cuenta == QString::number( MCuentaCorriente::ErrorClienteInvalido ) )
    {
        // El número de cuenta no es correcto
    }
    else
    {
        dSBDeuda->setValue( MCuentaCorriente::saldo( numero_cuenta ) );
        dSBDeuda->setVisible( true );
        LSaldo->setVisible( true );
        dSBTotal->setVisible( true );
        LDeuda->setVisible( true );
        recalcularTotal();
        if( _hicomp )
        { RBLuego->setVisible( true ); }
        return;
    }
  }
 }
 dSBDeuda->setVisible( false );
 LDeuda->setVisible( false );
 dSBTotal->setVisible( false );
 LSaldo->setVisible( false );
 if( _hicomp )
 { RBLuego->setVisible( false ); }
}


/*!
    \fn FormAgregarRecibo::cambioPagado( double valor )
    Slot llamado cuando se cambia la cantidad ingresada en pagado
 */
void FormAgregarRecibo::cambioPagado( double /*valor*/ )
{ recalcularTotal(); }

#include "EReporte.h"
/*!
  \fn FormAgregarRecibo::guardar()
 */
void FormAgregarRecibo::guardar()
{
    // Si fue llamado sin modelo adyacente
    if( this->_modelo ) {
        this->setearModelo( 0 );
    }
    // Verificaciónes iniciales
    if( this->CBCliente->currentIndex() == -1 ) {
        QMessageBox::warning( this, "Faltan datos", "Por favor verifique que un cliente este elegido" );
        return;
    }
    if( this->TETexto->toPlainText().isEmpty() ) {
        QMessageBox::warning( this, "Faltan datos", "Por favor verifique que el texto del recibo no este vacio" );
        return;
    }
    if( (!this->RBContado->isChecked()) && (!this->RBotro->isChecked()) && (!this->RBLuego->isChecked()) ) {
        QMessageBox::warning( this, "Faltan datos", "Por favor verifique que haya elegido una forma de pago." );
        return;
    }
    if( this->dSBPagado->value() <= 0.0 ) {
        QMessageBox::warning( this, "Faltan datos", "Por favor verifique que haya ingresado una cantidad a pagar" );
        return;
    }
    int id_cliente = this->CBCliente->idClienteActual();
    QDate fecha = this->DEFecha->date();
    QString contenido = this->TETexto->toPlainText();
    double total = this->dSBPagado->value();
    bool pagado = false;
    bool contado = false;
    if( this->RBContado->isChecked() ) {
        pagado = true;
        contado = true;
    } else if( this->RBotro->isChecked() ) {
        pagado = true;
    } // Si esta como luego, pongo los dos parametros como estan false, false
    // Genero la transacción
    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).transaction();
    this->_modelo->setEditStrategy( QSqlTableModel::OnManualSubmit );
    int num_recibo = this->_modelo->agregarRecibo( id_cliente, fecha, contenido, total, pagado, contado );
    if( num_recibo == -1 ) {
        this->_modelo->revertAll();
        QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
        QMessageBox::warning( this, "Error", QString( "Ocurrio un error al intentar guardar el recibo" ) );
        return;
    }
    this->_modelo->submitAll();
    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).commit();
    // Imprimir el recibo
    QMessageBox::information( this, "Correcto", QString::fromUtf8( "El recibo nº %1 se guardo correctamente y se ha enviado a imprimir automaticamente" ).arg( this->_modelo->buscarNumeroComprobantePorId( num_recibo ).aCadena() ) );
    // Imprimo el recibo
    ParameterList lista;
    lista.append( "id_recibo", num_recibo );
    EReporte *rep = new EReporte( this );
    rep->recibo();
    if( !rep->hacer( lista ) ) {
        rep->mostrarError( this );
        QMessageBox::information( this, "Error", QString( "No se pudo encontrar la definicion del informe. Contactese con el administrador." ) );
    }
    this->close();
    this->_modelo = 0;
    return;
}