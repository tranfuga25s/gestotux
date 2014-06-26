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
#include "formresumenctacte.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>
#include <QSqlError>
#include <QAction>
#include <QMenu>
#include <QDebug>

#include "eactcerrar.h"
#include "eactimprimir.h"
#include "eactpdf.h"
#include "mitemcuentacorriente.h"
#include "eregistroplugins.h"
#include "mcuentacorriente.h"

#include "MFactura.h"

FormResumenCtaCte::FormResumenCtaCte ( QWidget* parent, Qt::WFlags fl )
: EVentana ( parent, fl ), Ui::FormResumenCtaCteBase()
{
        setupUi ( this );
        this->setObjectName( "Resumen_cuenta-cte" );
        this->setWindowTitle( "Resumen de Cta Cte" );

        _filtro = ""; _numero_cuenta = "";

        // Relleno el combobox
        QSqlQuery cola( "SELECT cc.numero_cuenta, c.razon_social FROM ctacte AS cc, clientes AS c WHERE cc.id_cliente = c.id AND fecha_baja IS NULL" );
        while( cola.next() )
        {
                CBClienteCtaCte->addItem(     "#" + cola.record().value(0).toString() + " - " + cola.record().value(1).toString(),
                                             cola.record().value(0).toString() );
        }
        CBClienteCtaCte->setEditable( true );
        CBClienteCtaCte->setCurrentIndex( -1 );
        connect( CBClienteCtaCte, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioCtaCte( int ) ) );

        modeloItem = new MItemCuentaCorriente( TVItems, true );
        TVItems->setModel( modeloItem );
        TVItems->hideColumn( 0 );
        TVItems->hideColumn( 2 );
        TVItems->hideColumn( 7 );
        TVItems->hideColumn( 8 );
        TVItems->setSelectionMode( QAbstractItemView::SingleSelection );
        TVItems->setSelectionBehavior( QAbstractItemView::SelectRows );
        TVItems->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
        TVItems->horizontalHeader()->setResizeMode( 4, QHeaderView::Stretch );
        TVItems->horizontalHeader()->setResizeMode( 9, QHeaderView::Interactive );
        TVItems->setTextElideMode( Qt::ElideRight );
        //TVItems->setSortingEnabled( true );
        /// Menu contextual para cada operacion de la cuenta corriente
        connect( TVItems, SIGNAL( pressed( const QModelIndex & ) ), this, SLOT( menuContextual( const QModelIndex & ) ) );

        CBTipo->addItem( "Todos", 0 );
        CBTipo->addItem( "R"    , MItemCuentaCorriente::Recibo               );
        CBTipo->addItem( "F"    , MItemCuentaCorriente::Factura              );
        CBTipo->addItem( "NC"   , MItemCuentaCorriente::NotaCredito          );
        CBTipo->addItem( "ND"   , MItemCuentaCorriente::NotaDebito           );
        CBTipo->addItem( "CS"   , MItemCuentaCorriente::CobroServicio        );
        CBTipo->addItem( "RCS"  , MItemCuentaCorriente::RecargoCobroServicio );
        CBTipo->addItem( "AF"   , MItemCuentaCorriente::AnulacionFactura     );
        CBTipo->addItem( "AR"   , MItemCuentaCorriente::AnulacionRecibo      );
        CBTipo->addItem( "RM"   , MItemCuentaCorriente::Remito               );

        EActImprimir *ActImprimir = new EActImprimir( this );
        EActPdf *ActPdf = new EActPdf( this );

        QAction *ActSep = new QAction( this );
        ActSep->setSeparator( true );

        ActFiltrar = new QAction( this );
        ActFiltrar->setText( "Filtrar" );
        ActFiltrar->setStatusTip( "Muestra los filtros para mostrar solo ciertos datos" );
        ActFiltrar->setIcon( QIcon( ":/imagenes/filtro.png" ) );
        ActFiltrar->setCheckable( true );
        connect( ActFiltrar, SIGNAL( toggled( bool ) ), GBFiltro, SLOT( setVisible( bool ) ) );

        connect( GBFiltro, SIGNAL( toggled( bool ) ), this, SLOT( filtrar() ) );
        connect( CkBDesde, SIGNAL( toggled( bool ) ), this, SLOT( filtrar() ) );
        connect( CkBHasta, SIGNAL( toggled( bool ) ), this, SLOT( filtrar() ) );
        connect( DEDesde , SIGNAL( dateChanged( QDate ) ), this, SLOT( filtrar() ) );
        connect( DEHasta , SIGNAL( dateChanged( QDate ) ), this, SLOT( filtrar() ) );

        connect( CBTipo  , SIGNAL( currentIndexChanged(int) ), this, SLOT( filtrar() ) );

        GBFiltro->setVisible( ActFiltrar->isChecked() );
        DEDesde->setEnabled( CkBDesde->isChecked() );
        DEHasta->setEnabled( CkBHasta->isChecked() );

        DEDesde->setDate( QDate::currentDate() );
        DEHasta->setDate( QDate::currentDate() );

        this->addAction( ActImprimir );
        this->addAction( ActPdf );
        this->addAction( ActSep );
        this->addAction( ActFiltrar );
        //this->addAction( ActBuscar );
        this->addAction( new EActCerrar( this ) );

}

/*!
    \fn FormResumenCtaCte::setNumeroCuenta( const QString &numero_cuenta )
        Setea todas las condiciones del formulario para mostrar los datos de la cuenta corriente seleccionada por el parametro
        @param numero_cuenta Numero de cuenta al que se desea ver el resumen
 */
void FormResumenCtaCte::setNumeroCuenta( const QString &numero_cuenta )
{
  _numero_cuenta = numero_cuenta;
 // Seteo el numero de cuenta
 CBClienteCtaCte->setCurrentIndex( CBClienteCtaCte->findText( numero_cuenta, Qt::MatchContains ) );
 //Busco los datos
 QSqlQuery cola( QString( "SELECT fecha_alta, saldo, limite FROM ctacte WHERE numero_cuenta = '%1'" ).arg( numero_cuenta ) );
 if( cola.next() )
 {
   LFechaAlta->setText( cola.record().value(0).toDate().toString( Qt::DefaultLocaleShortDate ) );
   LSaldoActual->setText( QString( "$ %L1" ).arg( cola.record().value(1).toDouble(), 10, 'f', 3 ) );
   LLimiteCredito->setText( QString( "$ %L1" ).arg( cola.record().value(2).toDouble(), 10, 'f', 3 ) );
   modeloItem->setFilter( QString( "id_ctacte = '%1'" ).arg( numero_cuenta ) );
   modeloItem->select();
 }
 else
 {
  LFechaAlta->setText( "" );
  LSaldoActual->setText( "" );
  LLimiteCredito->setText( "" );
  qWarning( "Error al obtener los datos de la cuenta corriente" );
  qDebug() << qPrintable( "Numero cuenta: " + numero_cuenta );
  qDebug() << qPrintable( cola.lastError().text() );
  qDebug() <<  qPrintable( cola.executedQuery() );
 }
}


/*!
    \fn FormResumenCtaCte::cambioCtaCte( int numero_cuenta )
        Slot llamado cada vez que se cambia el combobox de la cuenta corriente
        @param numero_cuenta Numero de cuenta que indica el combobox
 */
void FormResumenCtaCte::cambioCtaCte( int /*numero_cuenta*/ )
{
 setNumeroCuenta( CBClienteCtaCte->itemData( CBClienteCtaCte->currentIndex(), Qt::UserRole ).toString() );
}


#include "EReporte.h"
#include <QMessageBox>
/*!
    \fn FormResumenCtaCte::imprimir()
 */
void FormResumenCtaCte::imprimir()
{
    if( modeloItem->rowCount() <= 0 ) {
        QMessageBox::warning( this, "Error", "No hay ningun movimiento para la cuenta corriente. No se imprimira nada" );
        return;
    }
    // Busco la cuenta?
    ParameterList lista;
    lista.append( Parameter( "ctacte", CBClienteCtaCte->itemData( CBClienteCtaCte->currentIndex(), Qt::UserRole ).toString() ) );
    lista.append( Parameter( "filtro", this->_filtro ) );
    EReporte *rep = new EReporte( 0 );
    rep->especial( "ResumenCtaCte", lista );
    if( ! rep->hacer() ) {
        QMessageBox::warning( this, "Error", "No se pudo imprimir el resumen de cuenta corriente" );
    }
    delete rep;
    rep = 0;
}


/*!
    \fn FormResumenCtaCte::pdf()
 */
void FormResumenCtaCte::aPdf()
{
    if( modeloItem->rowCount() <= 0 ) {
        QMessageBox::warning( this, "Error", "No hay ningun movimiento para la cuenta corriente. No se imprimira nada" );
        return;
    }
    // Parametros para el nombre

    QString nombre = QString( "%1 - Resumen de cuenta corriente.pdf").arg( CBClienteCtaCte->currentText() );
    ParameterList lista;
    lista.append( Parameter( "ctacte", CBClienteCtaCte->itemData( CBClienteCtaCte->currentIndex(), Qt::UserRole ).toString() ) );
    lista.append( Parameter( "filtro", this->_filtro ) );
    EReporte *rep = new EReporte( 0 );
    rep->especial( "ResumenCtaCte", lista );
    if( ! rep->hacerPDF( lista, nombre ) ) {
        QMessageBox::warning( this, "Error", "No se pudo generar como pdf el resumen de cuenta corriente" );
    }
    delete rep;
    rep = 0;
}

/*!
    \fn FormResumenCtaCte::menuContextual( const QModelIndex &indice )
 */
void FormResumenCtaCte::menuContextual( const QModelIndex &indice )
{
 if( QApplication::mouseButtons() != Qt::RightButton )
 { return; }
   // Calculo la posicion en que esta el item
   QPoint posicion;
   posicion.setX( TVItems->columnViewportPosition( indice.column() ) );
   posicion.setY( TVItems->rowViewportPosition( indice.row() ) );
   QMenu *_menuContextual = new QMenu( this );
   // Pido el menu contextual a la clase que esta manejando la vista acutlamente

 // Veo que tipo de item es
 QModelIndex temp = indice.model()->index( indice.row(), 3 );
 switch( temp.data( Qt::EditRole ).toInt() )
 {
        case MItemCuentaCorriente::Factura:
        {
                if( ERegistroPlugins::getInstancia()->existePluginExterno( "pagos" ) )
                {
                        /// @TODO: Verificar si no esta pagada ya
                        QAction *ActCrearRecibo = new QAction( this );
                        ActCrearRecibo->setText( "Pagar esta factura..." );
                        connect( ActCrearRecibo, SIGNAL( triggered() ), this, SLOT( pagarFactura() ) );
                        _menuContextual->addAction( ActCrearRecibo );
                }

                QAction *ActVerFactura = new QAction( this );
                ActVerFactura->setText( "Ver esta factura" );
                connect( ActVerFactura, SIGNAL( triggered() ), this, SLOT( verFactura() ) );
                _menuContextual->addAction( ActVerFactura );
                break;
        }
        case MItemCuentaCorriente::Recibo:
        {
                if( ERegistroPlugins::getInstancia()->existePluginExterno( "pagos" ) )
                {
                        QAction *ActVerRecibo = new QAction( this );
                        ActVerRecibo->setText( "Ver Recibo..." );
                        connect( ActVerRecibo, SIGNAL( triggered() ), this, SLOT( verRecibo() ) );
                        _menuContextual->addAction( ActVerRecibo );

                }
                break;
        }
        case MItemCuentaCorriente::Remito:
        {
                 if( ERegistroPlugins::getInstancia()->existePluginExterno( "remitos" ) ) {
                     QAction *ActVerRemito = new QAction( this );
                     ActVerRemito->setText( "Ver Remito..." );
                     connect( ActVerRemito, SIGNAL( triggered() ), this, SLOT( verRemito() ) );
                     _menuContextual->addAction( ActVerRemito );
                 }
        }
        case MItemCuentaCorriente::CobroServicio:
        case MItemCuentaCorriente::RecargoCobroServicio:
        default:
        { break; }
 }
 _menuContextual->addSeparator();

 if( ERegistroPlugins::getInstancia()->existePluginExterno( "pagos" ) )
 {
        QAction *ActPagarTodo = new QAction( this );
        ActPagarTodo->setText( "Pagar todo..." );
        connect( ActPagarTodo, SIGNAL( triggered() ), this, SLOT( pagarTodo() ) );
        _menuContextual->addAction( ActPagarTodo );
 }

 QAction *ActImprimirResumen = new QAction( this );
 ActImprimirResumen->setText( "Imprimir resumen de cuenta" );
 connect( ActImprimirResumen, SIGNAL( triggered() ), this, SLOT( imprimir() ) );
 _menuContextual->addAction( ActImprimirResumen );

 _menuContextual->addSeparator();
 _menuContextual->addAction( new EActCerrar( this ) );
 _menuContextual->popup( this->mapToGlobal( posicion ) );
}

#include "formagregarrecibo.h"
/*!
    \fn FormResumenCtaCte::pagarTodo()
 */
void FormResumenCtaCte::pagarTodo()
{
    if( ERegistroPlugins::getInstancia()->existePluginExterno( "pagos" ) ) {
        // Genero un recibo x el saldo deudor
        double saldo = MCuentaCorriente::saldo( _numero_cuenta );
        if( saldo <= 0.0 ) {
            QMessageBox::information( this, "Error", "El cliente no posee saldo que se deba pagar." );
            return;
        }
        // Busco que elementos son los que se pagan
        QString texto = "Pago total del saldo restante en cuenta corriente.";
        int id_cliente = MCuentaCorriente::idClientePorCtaCte( _numero_cuenta );
        if( id_cliente == -1 ) {
            QMessageBox::warning( this, "Error", QString::fromUtf8( "No se pudo buscar un dato para hacer el recibo. No se realizarÃ¡ nada" ) );
            return;
        }
        emit emitirRecibo( id_cliente, texto, saldo );
    } else {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "No se puede emitir un recibo ya que no se encuentra habilitado el plugin para tal función.\n Contacte su administrador de sistema." ) );
        return;
    }
    return;
}

/*!
    \fn FormResumenCtaCte::verRecibo()
 */
void FormResumenCtaCte::verRecibo()
{
    foreach( QModelIndex idx, TVItems->selectionModel()->selectedRows() ) {
        int id_recibo = modeloItem->data( modeloItem->index( idx.row(), 2 ), Qt::EditRole ).toInt();
        emit mostrarRecibo( id_recibo );
    }
    return;
}

/*!
 * \brief FormResumenCtaCte::verRemito
 */
void FormResumenCtaCte::verRemito()
{
    foreach( QModelIndex idx, TVItems->selectionModel()->selectedRows() ) {
        int id_recibo = modeloItem->data( modeloItem->index( idx.row(), 2 ), Qt::EditRole ).toInt();
        emit mostrarRemito( id_recibo );
    }
    return;
}

/*!
    \fn FormResumenCtaCte::verFactura()
 */
void FormResumenCtaCte::verFactura()
{
    if( ERegistroPlugins::getInstancia()->existePluginExterno( "ventas" ) ) {
        int id_pos = TVItems->selectionModel()->selectedRows().first().row();
        int id_factura = modeloItem->data( modeloItem->index( id_pos, 2 ), Qt::EditRole ).toInt();
        emit mostrarFactura( id_factura );
    }
}


/*!
    \fn FormResumenCtaCte::pagarFactura()
 */
void FormResumenCtaCte::pagarFactura()
{
    if( ERegistroPlugins::getInstancia()->existePluginExterno( "ventas" ) ) {
        // Veo que numero de factura es
        int id_pos = TVItems->selectionModel()->selectedRows().first().row();
        int id_factura = modeloItem->data( modeloItem->index( id_pos, 2 ), Qt::EditRole ).toInt();
        // Verifico que la factura no este pagada
        if( MPagos::verificarFacturaPagada( MFactura::obtenerComprobante( id_factura ).aCadena() ) ) {
            if( QMessageBox::information( this,
                                      QString::fromUtf8( "Ya pagada" ),
                                      QString::fromUtf8( "La factura parece ya estar pagada. Desea emitir un pago igualmente?" ),
                                      QMessageBox::Yes,
                                      QMessageBox::No ) == QMessageBox::No ) {
                return;
            }
        }
        // Busco los detalles de la factura
        QString texto_recibo = QString( "Pago de la factura %1" ).arg( MFactura::obtenerComprobante( id_factura ).aCadena() );
        double total = MFactura::obtenerTotal( id_factura );
        int id_cliente = MCuentaCorriente::idClientePorCtaCte( _numero_cuenta );
        // Genero un nuevo recibo con el total de la factura y en el detalle que paga la factura
        emit emitirRecibo( id_cliente, texto_recibo, total );
    }
}

/*!
 * \fn FormResumenCtaCte::filtrar()
 * Slot que permite hacer filtrado de los datos
 */
void FormResumenCtaCte::filtrar()
{
    if( GBFiltro->isChecked() ) {
        QString filtro;
        if( CkBDesde->isChecked() ) {
            // Comprobato que funciona con mysql y sqlite.
            filtro.append( QString( " AND fecha >= DATE( '%1' ) " ).arg( DEDesde->date().toString( "yyyy-MM-dd" ) ) );
        }
        if( CkBHasta->isChecked() ) {
            filtro.append( QString(" AND fecha <= DATE( '%1' ) " ).arg( DEHasta->date().toString( "yyyy-MM-dd" ) ) );
        }
        if( CBTipo->currentIndex() != 0 ) {
            filtro.append( QString( " AND tipo_op = %1 " ).arg( CBTipo->currentIndex() ) );
        }
        modeloItem->setFilter( QString( "id_ctacte = %1" ).arg( _numero_cuenta ).append( filtro ) );
        this->_filtro = filtro;
    } else {
        modeloItem->setFilter( QString( "id_ctacte = '%1'" ).arg( _numero_cuenta ) );
    }
    modeloItem->select();
}
