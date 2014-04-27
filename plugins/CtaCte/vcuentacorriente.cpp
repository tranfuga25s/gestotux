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
#include "vcuentacorriente.h"

#include "mcuentacorriente.h"
#include "dsino.h"
#include "EReporte.h"
#include "formresumenctacte.h"
#include "einputdialog.h"

#include <QTableView>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

VCuentaCorriente::VCuentaCorriente(QWidget *parent)
 : EVLista(parent)
{
 this->setObjectName( "visor_ctaCte" );
 this->setWindowTitle( "Cuentas Corrientes" );
 this->setWindowIcon( QIcon( ":/imagenes/ctacte.png" ) );

 crmodelo = new MCuentaCorriente( this );
 vista->setModel( crmodelo );
 vista->setItemDelegateForColumn( 6, new DSiNo( this->vista ) );
 vista->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
 vista->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
 vista->horizontalHeader()->setMinimumSectionSize( 60 );
 vista->setSortingEnabled( true );
 connect( crmodelo, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ), vista, SLOT( dataChanged( QModelIndex, QModelIndex ) ) );
 modelo = 0;
 rmodelo = crmodelo;

 ActResumen = new QAction( "Ver Resumen de Cuenta", this );
 connect( ActResumen, SIGNAL( triggered() ), this, SLOT( verResumen() ) );

 ActAgregar->setText( "Agregar Nueva CtaCte" );
 ActAgregar->setIcon( QIcon( ":/imagenes/ctacte_nueva.png" ) );

 QAction *ActSep = new QAction( this );
 ActSep->setSeparator(true );

 ActVerDeudoras = new QAction( this );
 ActVerDeudoras->setStatusTip( "Muestra solo las cuentas corrientes que tienen registrados movimientos mas altos que su limite." );
 ActVerDeudoras->setText( "Solo Sobrepasadas" );
 ActVerDeudoras->setCheckable( true );
 ActVerDeudoras->setIcon( QIcon( ":/imagenes/filtro.png" ) );
 connect( ActVerDeudoras, SIGNAL( toggled( bool ) ), this, SLOT( mostrarDeudoras( bool ) ) );

 ActModificarLimite = new QAction( "Modificar Limite", this );
 connect( ActModificarLimite, SIGNAL( triggered() ), this, SLOT( modificarLimite() ) );

 ActSuspender = new QAction( "Suspender", this );
 connect( ActSuspender, SIGNAL( triggered() ), this, SLOT( suspenderDesuspender() ) );

 ActVerSuspendidas = new QAction( this );
 ActVerSuspendidas->setStatusTip( "Muestra solo las cuentas corrientes quese encuentren suspendidas." );
 ActVerSuspendidas->setText( "Solo Suspendidas" );
 ActVerSuspendidas->setIcon( QIcon( ":/imagenes/filtro.png" ) );
 ActVerSuspendidas->setCheckable( true );
 connect( ActVerSuspendidas, SIGNAL( toggled( bool ) ), this, SLOT( verSuspendidas( bool ) ) );

 ActListadoDeudor = new QAction( this );
 ActListadoDeudor->setStatusTip( "Muestra un listado con todas las cuentas corrientes que tienen saldo deudor" );
 ActListadoDeudor->setText( "Listado Deudor" );
 ActListadoDeudor->setIcon( QIcon( ":/imagenes/impresora.png" ) );
 connect( ActListadoDeudor, SIGNAL( triggered() ), this, SLOT( listadoDeudor() ) );

 ActListadoDeudorPDF = new QAction( this );
 ActListadoDeudorPDF->setStatusTip( "Muestra un listado con todas las cuentas corrientes que tienen saldo deudor" );
 ActListadoDeudorPDF->setText( "Listado Deudor" );
 ActListadoDeudorPDF->setIcon( QIcon( ":/imagenes/acroread.png" ) );
 connect( ActListadoDeudorPDF, SIGNAL( triggered() ), this, SLOT( listadoDeudorPDF() ) );

 ActRecalcularSaldo = new QAction( this );
 ActRecalcularSaldo->setStatusTip( "Recalcula el saldo de una cuenta corriente manualmente" );
 ActRecalcularSaldo->setText( "Recalcular saldo" );
 connect( ActRecalcularSaldo, SIGNAL( triggered() ), this, SLOT( recalcularSaldo() ) );

/* SELECT ctacte."numero_cuenta",
 *        relTblAl_1.razon_social,
 *        ctacte."fecha_alta",
 *        ctacte."fecha_baja",
 *        ctacte."saldo",
 *        ctacte."limite",
 *        ctacte."suspendida"
 * FROM ctacte, clientes relTblAl_1
 * WHERE (ctacte."id_cliente" = relTblAl_1.id)
 * ORDER BY ctacte."numero_cuenta" DESC*/


 this->agregarFiltroBusqueda( "Cualquiera", "relTblAl_1.\"razon_social\"  LIKE \"%%%1%\" OR ctacte.\"numero_cuenta\" = %1" );
 this->agregarFiltroBusqueda( "Razon Social", "relTblAl_1.\"razon_social\"  LIKE \"%%%1%\"" );
 this->agregarFiltroBusqueda( "Razon social y habilitadas", "relTblAl_1.\"razon_social\"  LIKE \"%%%1%\" AND ctacte.\"suspendida\" = 1" );
 this->agregarFiltroBusqueda( "Saldo mayor a ", "ctacte.\"saldo\" > %1" );
 this->agregarFiltroBusqueda( "Saldo menor a", "ctacte.\"saldo\" < %1" );
 habilitarBusqueda();

 QAction *ActSep1 = new QAction( this );
 ActSep1->setSeparator(true);
 QAction *ActSep2 = new QAction( this );
 ActSep2->setSeparator(true);

 addAction( ActAgregar );
 addAction( ActModificarLimite );
 addAction( ActSuspender );
 addAction( ActSep1 );
 addAction( ActVerTodos );
 addAction( ActResumen );
 addAction( ActListadoDeudor );
 addAction( ActListadoDeudorPDF );
 addAction( ActSep );
 addAction( ActVerDeudoras );
 addAction( ActVerSuspendidas );
 addAction( ActBuscar );
 addAction( ActSep2 );
 addAction( ActCerrar );
}

#include "formnuevactacte.h"
/*!
    \fn VCuentaCorriente::agregar( bool autoeliminarid )
    Abre el formulario para agregar una nueva cuenta corriente, verificando que existan clientes para agregar a las cuentas corrientes.
 */
void VCuentaCorriente::agregar( bool /*autoeliminarid*/ )
{
 // Verifico si existe algún cliente para agregar que no sea el consumidor final
 QSqlQuery cola;
 if( cola.exec( "SELECT COUNT(id) FROM clientes WHERE id NOT IN ( SELECT id_cliente FROM ctacte ) AND id != 0" ) ) {
     if( cola.next() ) {
         if( cola.record().value(0).toInt() <= 0 ) {
             QMessageBox::information( this, "No hay clientes", "No existen clientes a los que se les pueda agregar cuenta corriente" );
             return;
         }
     } else {
         qDebug() << "Error de next en la cola de averiguación de cantidad de clientes para la ctacte.";
         qDebug() << cola.lastError().text();
         qDebug() << cola.lastQuery();
     }
 } else {
     qWarning() << "Error de exec en la cola de averiguación de cantidad de clientes para la ctacte.";
     qDebug() << cola.lastError().text();
     qDebug() << cola.lastQuery();
 }
 FormNuevaCtaCte d;
 d.setModelo( crmodelo );
 d.exec();
}


/*!
    \fn VCuentaCorriente::menuContextual( const QModelIndex &indice, QMenu *menu )
    Muestra el menú contextual de cada item.
 */
void VCuentaCorriente::menuContextual( const QModelIndex &indice, QMenu *menu )
{
 menu->addAction( ActAgregar );
 menu->addSeparator();
 if( !indice.model()->data( indice.model()->index( indice.row(), crmodelo->fieldIndex( "fecha_baja" ) ) ).toDate().isValid() )
 {
        QAction *ActDarBaja = new QAction( "Dar de Baja", this );
        connect( ActDarBaja, SIGNAL( triggered() ), this, SLOT( darBaja() ) );
        menu->addAction( ActDarBaja );
 }
 if( !indice.model()->data( indice.model()->index( indice.row(), crmodelo->fieldIndex( "suspendida" ) ) ).toBool() ) {
     QAction *ActSuspender2 = new QAction( "Sacar suspension", this );
     connect( ActSuspender2, SIGNAL( triggered() ), this, SLOT( suspenderDesuspender() ) );
     menu->addAction( ActSuspender2 );
 } else {
     menu->addAction( ActSuspender );
 }

 menu->addAction( ActModificarLimite );
 menu->addAction( ActResumen );
 menu->addAction( ActRecalcularSaldo );
}


/*!
    \fn VCuentaCorriente::modificarLimite()
    Modifica el limite de la cuenta corriente seleccionada.
 */
void VCuentaCorriente::modificarLimite()
{
 // Busco el item
 if( vista->selectionModel()->selectedRows().isEmpty() ) {
     QMessageBox::warning( this, "Error", QString::fromUtf8( "Por favor, seleccione una cuenta corriente para cambiarle el límite." ) );
     return;
 }
 QModelIndex indice = vista->selectionModel()->selectedRows().first();
 double limite_anterior = crmodelo->data( crmodelo->index( indice.row(), crmodelo->fieldIndex( "limite" ) ), Qt::EditRole ).toDouble();
 QString id_ctacte = crmodelo->data( crmodelo->index( indice.row(), crmodelo->fieldIndex( "numero_cuenta" ) ), Qt::EditRole ).toString();
 bool ok = false;
 double limite_nuevo = EInputDialog::getImporte( this, "Ingrese el limite maximo", QString::fromUtf8("Límite máximo de credito:"), limite_anterior, 0.0, 1000000.0, 3, &ok );
 if( ok )
 {
  //Verifico que no sean el mismo
  if( limite_anterior == limite_nuevo )
  { return; }
  if( crmodelo->setData( rmodelo->index( indice.row(), crmodelo->fieldIndex( "limite" ) ), limite_nuevo ) ) {
            crmodelo->modificarLimite( id_ctacte, limite_nuevo );
            crmodelo->submit();
            QMessageBox::information( this, "Correcto", QString::fromUtf8( "El nuevo límite ha sido colocado correctamente." ) );
            return;
        } else {
            QMessageBox::warning( this, "Incorrecto", QString::fromUtf8( "El nuevo límite NO ha sido colocado correctamente." ) );
            return;
        }
 }
}


/*!
    \fn VCuentaCorriente::darBaja()
    Da de baja la cuenta corriente seleccionada.
 */
void VCuentaCorriente::darBaja()
{
 // busco el item
 QModelIndex indice = vista->selectionModel()->selectedRows().first();
 int ret = QMessageBox::question( this, QString::fromUtf8( "¿Esta seguro?" ), "Esta seguro que desea deshabilitar esta cuenta corriente?", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel );
 if( ret == QMessageBox::Ok )
 {
  crmodelo->setData( crmodelo->index( indice.row(), crmodelo->fieldIndex( "fecha_baja" ) ), QDate::currentDate(), Qt::EditRole );
  crmodelo->submitAll();
 }
}


/*!
    \fn VCuentaCorriente::verResumen()
    Muestra el resumen de cuenta de la cuenta corriente seleccionada.
 */
void VCuentaCorriente::verResumen()
{
 if( vista->selectionModel()->selectedRows().isEmpty() )
 {
     QMessageBox::warning( this, "Error", "Elija una cuenta corriente para ver su resumen", QMessageBox::Ok );
     return;
 }
 QModelIndex indice = vista->selectionModel()->selectedRows().first();
 //Obtengo el numero de cuenta
 QString numero_cuenta = indice.model()->data( indice.model()->index( indice.row(), crmodelo->fieldIndex( "numero_cuenta" ) ), Qt::EditRole ).toString();
 FormResumenCtaCte *form = new FormResumenCtaCte( this );
 form->setNumeroCuenta( numero_cuenta );
 connect( form, SIGNAL( emitirRecibo( int, QString ,double ) ), this, SIGNAL( emitirRecibo( int, QString, double ) ) );
 connect( form, SIGNAL( mostrarRecibo( int ) ), this, SIGNAL( mostrarRecibo( int ) ) );
 connect( form, SIGNAL( mostrarFactura( int ) ), this, SIGNAL( mostrarFactura( int ) ) );
 connect( form, SIGNAL( mostrarRemito( int ) ), this, SIGNAL( mostrarRemito( int ) ) );
 emit agregarVentana( form );
}

/*!
 * \fn VCuentaCorriente::mostrarDeudoras( bool estado )
 * Setea para que se vean solo las cuentas deudoras o no.
 */
void VCuentaCorriente::mostrarDeudoras( bool estado ) {
    this->crmodelo->filtrarSoloDeudoras( estado );
    this->crmodelo->select();
}

/*!
 * \fn VCuentaCorriente::suspenderDesuspender()
 * Slot lamado par suspender o desuspender una cuenta
 */
void VCuentaCorriente::suspenderDesuspender()
{
    // Busco el item
    if( vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "Por favor, seleccione una cuenta corriente para suspender/sacar de suspensión." ) );
        return;
    }
    QModelIndex indice = vista->selectionModel()->selectedRows().first();
    bool anterior = crmodelo->data( crmodelo->index( indice.row(), crmodelo->fieldIndex( "suspendida" ) ), Qt::EditRole ).toBool();
    int ret = -1;
    if( !anterior ) {
        ret = QMessageBox::question( this, QString::fromUtf8("¿Esta seguro?"), QString::fromUtf8("¿Está seguro que desea suspender esta cuenta corriente?. \n El cliente que posee esta cuenta corriente no podrá realizar mas operaciónes a cuenta corriente."), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel );
    } else {
        ret = QMessageBox::question( this, QString::fromUtf8("¿Esta seguro?"), QString::fromUtf8("¿Está seguro que desea sacar la suspensión de esta cuenta corriente?. \n Esto habilitará a el cliente que posee esta cuenta corriente a realizar operaciónes a cuenta corriente."), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel );
    }
    if( ret == QMessageBox::Ok )
    {
     //Verifico que no sean el mismo
     if( crmodelo->setData( crmodelo->index( indice.row(), crmodelo->fieldIndex( "suspendida" ) ), !anterior ) ) {
               if( !anterior )
                   QMessageBox::information( this, "Correcto", QString::fromUtf8( "La cuenta ha sido suspendida correctamente." ) );
               else
                   QMessageBox::information( this, "Correcto", QString::fromUtf8( "La cuenta ha sido sacada de suspencion correctamente." ) );
               crmodelo->submit();
               return;
           } else {
               QMessageBox::warning( this, "Incorrecto", QString::fromUtf8( "No se pudo modificar la suspensión de la cuenta corriente." ) );
               return;
           }
    }
}

/*!
 * \fn VCuentaCorriente::verSuspendidas( bool estado )
 * Muestra solo las cuentas suspendidas
 */
void VCuentaCorriente::verSuspendidas( bool estado )
{
    this->crmodelo->filtrarSoloSuspendidas( estado );
    this->crmodelo->select();
}

/*!
 * \fn VCuentaCorriente::listadoDeudor()
 * Imprime el listado de cuentas corrientes deudoras
 */
void VCuentaCorriente::listadoDeudor()
{
    EReporte *rep = new EReporte( 0 );
    rep->especial( "ListadoCtaCteSaldo", ParameterList() );
    rep->hacer();
    delete rep;
}

/*!
 * \fn VCuentaCorriente::listadoDeudorPDF()
 * Pasa a PDF el listado de cuentas corrientes deudoras
 */
void VCuentaCorriente::listadoDeudorPDF()
{
    EReporte *rep = new EReporte( 0 );
    rep->especial( "ListadoCtaCteSaldo", ParameterList() );
    rep->hacerPDF( ParameterList(), QString( "Listado de Cuenta Corriente con saldo al %1" ).arg( QDate::currentDate().toString( Qt::SystemLocaleShortDate ) ) );
    delete rep;
}

/*!
 * \brief VCuentaCorriente::recalcularSaldo
 */
void VCuentaCorriente::recalcularSaldo()
{
    // Busco el item
    if( vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "Por favor, seleccione una cuenta corriente para suspender/sacar de suspensión." ) );
        return;
    }
    QModelIndex indice = vista->selectionModel()->selectedRows().first();
    QString numero_cuenta = indice.model()->data( indice.model()->index( indice.row(), crmodelo->fieldIndex( "numero_cuenta" ) ), Qt::EditRole ).toString();
    if( crmodelo->recalcularSaldo( numero_cuenta ) ) {
        QMessageBox::information( this, "Correcto", "El saldo fue recalculado correctamente" );
    } else {
        QMessageBox::warning( this, "Incorrecto", "No se pudo actualizar el saldo de la cuenta" );
    }
}
