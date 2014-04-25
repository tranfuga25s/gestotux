/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   *
 *   juiraze@yahoo.com.ar   *
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
#include "vremito.h"

#include <QIcon>
#include <QTableView>
#include <QMessageBox>
#include <QInputDialog>

#include "vitemremito.h"
#include "MVRemito.h"
#include "EReporte.h"
#include "eactcerrar.h"
#include "MRemito.h"
#include "eactimprimir.h"
#include "eactpdf.h"

VRemito::VRemito(QWidget *parent)
 : EVLista(parent)
{
    this->setObjectName( "ListaRemito" );
    this->setWindowTitle( "Lista de Remitos" );
    this->setWindowIcon( QIcon( ":/imagenes/remito.png" ) );

    this->modelo = new MVRemito( this->vista );
    qobject_cast<MVRemito *>(this->modelo)->verAnulados( false );
    this->vista->setModel( this->modelo );

    this->vista->hideColumn( 0 );
    this->vista->hideColumn( 6 );
    this->vista->setAlternatingRowColors( true );

    QAction *ActPagar = new QAction( this );
    ActPagar->setText( "Pagar" );
    ActPagar->setIcon( QIcon( ":/imagenes/pagaremito.png" ) );
    ActPagar->setStatusTip( "Genera un recibo por las facturas seleccionadas." );
    connect( ActPagar, SIGNAL( triggered() ), this, SLOT( pagar() ) );

    QAction *ActAnular = new QAction( this );
    ActAnular->setText( "Anular" );
    ActAnular->setIcon( QIcon( ":/imagenes/remito-anular.png" ) );
    ActAnular->setStatusTip( "Anula las facturas seleccionadas" );
    connect( ActAnular, SIGNAL( triggered() ), this, SLOT( anular() ) );

    QAction *ActVerAnuladas = new QAction( this );
    ActVerAnuladas->setText( "Ver anulados" );
    ActVerAnuladas->setStatusTip( "Muestra o oculta las facturas anuladas" );
    ActVerAnuladas->setIcon( QIcon( ":/imagenes/remitoveranulados.png" ) );
    ActVerAnuladas->setCheckable( true );
    ActVerAnuladas->setChecked( false );
    connect( ActVerAnuladas, SIGNAL( toggled( bool ) ), this, SLOT( cambioVerAnulados( bool ) ) );

    ActAgregar->setIcon( QIcon( ":/imagenes/remito-nuevo.png" ) );

    QAction *ActSep = new QAction( this );
    ActSep->setSeparator( true );

    ActVerItems = new QAction( this );
    ActVerItems->setText( "Ver items" );
    ActVerItems->setStatusTip( "Muestra el listado de items de la factura" );
    ActVerItems->setIcon( QIcon( ":/imagenes/remitoveritems.png" ) );
    connect( ActVerItems, SIGNAL( triggered() ), this, SLOT( verItems() ) );

    //agregarFiltroBusqueda( "Numero de Factura", " ``" );
    agregarFiltroBusqueda( "Numero de Cliente", " `id_cliente` = '%%%1%' " );
    agregarFiltroBusqueda( "Fecha", " `fecha` = '%%1%' ");
    habilitarBusqueda();

    disconnect( ActAgregar, SIGNAL( triggered() ), this, SLOT( agregar() ) );
    connect( ActAgregar, SIGNAL( triggered() ), this, SLOT( agregar() ) );

    this->addAction( ActAgregar );
    this->addAction( ActVerItems );
    this->addAction( ActPagar );
    this->addAction( ActAnular );
    this->addAction( new EActImprimir( this ) );
    this->addAction( new EActPdf( this ) );
    this->addAction( ActSep );
    this->addAction( ActVerAnuladas );
    this->addAction( ActBuscar );
    this->addAction( ActVerTodos );
    this->addAction( ActCerrar );

}

#include "formagregarremito.h"
void VRemito::agregar()
{ emit agregarVentana( new FormAgregarRemito() ); }

void VRemito::eliminar()
{ return; }

void VRemito::imprimir()
{
    // Busco todos los IDs a pagar
    QModelIndexList lista = this->vista->selectionModel()->selectedRows();
    if( lista.size() < 1 ) {
        QMessageBox::warning( this, "Seleccione un item",
                        "Por favor, seleccione al menos un item para imprimir.",
                        QMessageBox::Ok );
        return;
    }
    EReporte *rep = new EReporte( 0 );
    rep->remito();
    foreach( QModelIndex indice, lista ) {
        int id_remito = this->modelo->data( this->modelo->index( indice.row(), 0 ) ).toInt();
        ParameterList lista;
        lista.append( Parameter( "id_remito", id_remito ) );
        rep->hacer( lista, false, true );
    }
    delete rep;
    return;
}

void VRemito::aPdf()
{
    // Busco todos los IDs a pagar
    QModelIndexList lista = this->vista->selectionModel()->selectedRows();
    if( lista.size() < 1 ) {
        QMessageBox::warning( this, "Seleccione un item",
                        "Por favor, seleccione al menos un item para pasar a PDF.",
                        QMessageBox::Ok );
        return;
    }
    EReporte *rep = new EReporte( 0 );
    rep->remito();
    foreach( QModelIndex indice, lista ) {
        int id_remito = this->modelo->data( this->modelo->index( indice.row(), 0 ) ).toInt();
        QString numero = this->modelo->data( this->modelo->index( indice.row(), 1 ) ).toString();
        ParameterList lista;
        lista.append( Parameter( "id_remito", id_remito ) );
        rep->hacerPDF( lista, QString( "Remito#%1" ).arg( numero ) );
    }
    delete rep;
    return;
}

void VRemito::modificar()
{ return; }

/*!
 * \fn VRemito::anular()
 * Anula la factura seleccionada o solicita el numero de factura a anular
 */
void VRemito::anular()
{
    return;
    // Busco todos los IDs a anular
    if( this->modelo->rowCount() == 0 ) {
        // error, nada seleccionado....
        // Solicito el numero de factura
        bool ok = false;
        QString numero = QInputDialog::getText( this, "Ingrese numero", "Numero de Factura:", QLineEdit::Normal, QString(), &ok );
        if( ok ) {
            int id_remito = MRemito::idRemitoPorComprobante( numero );
            if( id_remito != -1 ) {
                ok = false;
                QString razon = QInputDialog::getText( this, "Ingrese razon", QString::fromUtf8( "Ingrese razon de anulación" ), QLineEdit::Normal, QString(), &ok );
                if( ok ) {
                    this->imprimirAnulacion( id_remito, razon, numero ); // Este metodo tiene el mfactura::anular factura
                }
            }
        }
        return;
    }
    QModelIndexList lista = this->vista->selectionModel()->selectedRows();
    if( lista.size() < 1 ) {
        QMessageBox::warning( this, "Seleccione un item",
                        "Por favor, seleccione al menos un item para anular",
                        QMessageBox::Ok );
        return;
    }
    foreach( QModelIndex indice, lista ) {
        bool ok = false;
        QString numero = this->modelo->data( this->modelo->index( indice.row(), 1 ) ).toString();
        QString razon = QInputDialog::getText( this, "Ingrese razon", QString::fromUtf8( "Ingrese razon de anulación" ), QLineEdit::Normal, QString(), &ok );
        int id_remito = this->modelo->data( this->modelo->index( indice.row(), 0 ) ).toInt();
        if( ok && !razon.isEmpty() ) {
            this->imprimirAnulacion( id_remito, razon, numero ); // Este metodo tiene el mfactura::anular factura
        }

    }
    this->modelo->select();
    return;
}

/*!
 * \fn VRemito::pagar()
 * Intenta generar un recibo con los datos de la factura seleccionada
 */
void VRemito::pagar()
{
    // Busco todos los IDs a pagar
    QModelIndexList lista = this->vista->selectionModel()->selectedRows();
    if( lista.size() < 1 ) {
        QMessageBox::warning( this, "Seleccione un item",
                        "Por favor, seleccione al menos un item para pagar.",
                        QMessageBox::Ok );
        return;
    }
    // Busco los datos de las facturas
    double total = 0;
    QString texto_recibo = "Pago de los remito";
    if( lista.size() > 1 ) { texto_recibo.append( "s" ); }
    texto_recibo.append( "  " );
    foreach( QModelIndex indice, lista ) {
        total += this->modelo->data( this->modelo->index( indice.row(), 5 ) ).toDouble();
        texto_recibo.append( "#" );
        texto_recibo.append( this->modelo->data( this->modelo->index( indice.row(), 1 ) ).toString() );
        texto_recibo.append( ",  " );
    }
    // Busco el numero de cliente
    int id_cliente = this->modelo->data( this->modelo->index( lista.first().row(), 2 ) ).toInt();
    emit emitirRecibo( id_cliente, QDate::currentDate(), texto_recibo, total );
    return;
}

/*!
 * \fn VRemito::cambioVerAnulados( bool parametros )
 * Muestra o oculta del listado de parametros
 * \param parametro Parametro si esta o no habilitado el boton.
 */
void VRemito::cambioVerAnulados( bool parametro )
{ qobject_cast<MVRemito *>(this->modelo)->verAnulados( parametro ); }


/*!
 * \fn VRemito::verItems()
 * Muestra el listado de items de una factura
 */
void VRemito::verItems()
{
    // Busco todos los IDs a pagar
    int id_remito = -1;
    QModelIndexList lista = this->vista->selectionModel()->selectedRows();
    if( lista.size() < 1 ) {
        bool ok = false;
        QString numero = QInputDialog::getText( this, QString::fromUtf8("Ingrese número"), QString::fromUtf8("Número de Remito:"), QLineEdit::Normal, QString(), &ok );
        if( ok ) {
            id_remito = MRemito::idRemitoPorComprobante( numero );
        }
    } else {
        QModelIndex indice = lista.first();
        id_remito = indice.model()->data( indice.model()->index( indice.row(), 0 ), Qt::EditRole ).toInt();
    }
    VItemRemito *f = new VItemRemito();
    f->setearIdRemito( id_remito );
    emit agregarVentana( f );
}

/*!
 * \fn VRemito::imprimirAnulacion( const int id_factura, const QString razon, const QString numero )
 * Anula la factura e imprime el reporte correspondiente
 * \param id_factura ID de la factura a anular
 * \param razon Razon por la cual se anula
 * \param numero Identificador de comprobante que se pasa para el cuadro de dialogo
 */
void VRemito::imprimirAnulacion( const int id_factura, const QString razon, const QString numero )
{
    if( MRemito::anularRemito( id_factura, razon, QDateTime::currentDateTime() ) ) {
        int ret = QMessageBox::question( this, "Correcto", QString::fromUtf8( "El remito %1 ha sido anulada correctamente. <br /> ¿Desea imprimir la anulaciòn?" ).arg( numero ), QMessageBox::Yes, QMessageBox::No );
        if( ret == QMessageBox::Yes ) {
            QMessageBox::information( this, "Esperando", QString::fromUtf8( "Por favor, ingrese el remito en la impresora para imprimir la anulaciòn. <br /> Presione OK para enviar a imprimir la anulacion" ), QMessageBox::Ok );
            EReporte *rep = new EReporte( this );
            rep->anulacionFactura();
            ParameterList lista;
            lista.append( Parameter( "razon", razon ) );
            lista.append( Parameter( "fechahora", QDateTime::currentDateTime().toString( "dd/MM/yyyy") ) );
            if( !rep->hacer( lista, true ) ) {
                QMessageBox::information( this, QString::fromUtf8("Error de impresión"), QString::fromUtf8( "Error al imprimir la anulación. Ingresela a mano." ), QMessageBox::Ok );
            }
        }
    } else {
        QMessageBox::warning( this, "Error", "Hubo un error la intentar anular el remito. No se anulo" );
    }
}
