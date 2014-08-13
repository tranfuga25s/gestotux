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
#include "vpresupuesto.h"

#include <QTableView>
#include <QModelIndex>
#include <QModelIndexList>
#include <QMessageBox>

#include "formmodificarpresupuesto.h"
#include "formagregarpresupuesto.h"
#include "EReporte.h"
#include "mpresupuesto.h"
#include "mvpresupuestos.h"
#include "vlistapresupuesto.h"
#include "mdescuentos.h"
#include "mproductostotales.h"

VPresupuesto::VPresupuesto(QWidget *parent)
 : EVLista(parent)
{
 setObjectName( "visorPresupuestos" );
 setWindowTitle( "Presupuestos Anteriores" );
 setWindowIcon( QIcon( ":/imagenes/anteriores.png" ) );
 modelo = new MVPresupuestos( this );
 vista->setModel( modelo );
 vista->hideColumn( 0 );
 vista->hideColumn( 5 );
 vista->setSortingEnabled( true );
 connect( vista, SIGNAL( doubleClicked( QModelIndex ) ), this, SLOT( modificar( QModelIndex ) ) );
 modelo->select();

 agregarFiltroBusqueda( "Destinatario", " `destinatario` LIKE '%%%1%' " );
 agregarFiltroBusqueda( "Fecha menor a", " DATE( `fecha` ) <= %1" );
 agregarFiltroBusqueda( "Fecha mayor a ", " DATE( `fecha` ) > %1" );
 habilitarBusqueda();

 ActVerContenido = new QAction( this );
 ActVerContenido->setText( "Ver items" );
 ActVerContenido->setStatusTip( "Muestra los items que componen el presupuesto" );
 ActVerContenido->setIcon( QIcon( ":/imagenes/presupuesto-items.png" ) );
 connect( ActVerContenido, SIGNAL( triggered() ), this, SLOT( verContenido() ) );

 ActAFactura = new QAction( this );
 ActAFactura->setText( "Facturar" );
 ActAFactura->setStatusTip( "Pasa el presupuesto seleccionado a factura" );
 connect( ActAFactura, SIGNAL( triggered() ), this, SLOT( aFactura() ) );

 addAction( ActAgregar );
 addAction( ActModificar );
 addAction( ActVerContenido );
 addAction( ActEliminar );
 addAction( ActImprimir );
 addAction( ActPdf );
 addAction( ActAFactura );
 addAction( ActBuscar );
 addAction( ActCerrar );
}

void VPresupuesto::agregar( bool /*autocompletar*/ )
{
 emit agregarVentana( new FormAgregarPresupuesto() );
 this->close();
}

#include "mclientes.h"
void VPresupuesto::imprimir()
{
  // Veo que ID quiere reimprimir.
  QModelIndexList lista = vista->selectionModel()->selectedRows();
  if( lista.isEmpty() ) {
      QMessageBox::information( this, "Error", "Por favor, seleccione uno o mas presupuestos para reimprimir", QMessageBox::Ok );
      return;
  }
  EReporte *rep = new EReporte( 0 );
  rep->presupuesto();
  ParameterList *parametros = new ParameterList();
  foreach( QModelIndex idx, lista ) {
      parametros->clear();
      parametros->append( Parameter( "id_presupuesto", modelo->data( modelo->index( idx.row(), 0 ), Qt::EditRole ).toInt() ) );
      int id_cliente = modelo->data( modelo->index( idx.row(), 0 ), Qt::EditRole ).toInt();
      if( id_cliente < 0 ) {
          parametros->append( Parameter( "cliente_existe", false ) );
          //lista.append( Parameter( "direccion", LEDireccion->text() ) );
      } else if( id_cliente >= 0 ){
          parametros->append( Parameter( "cliente_existe", true ) );
          //lista.append( Parameter( "direccion", LEDireccion->text() ) );
          if( id_cliente > 0 )
              parametros->append( Parameter( "direccion", MClientes::direccionEntera( id_cliente ) ) );
      }
      if( !rep->hacer( *parametros ) ) {
          qDebug( "Error la intentar imprimir el reporte" );
      }
  }
  delete parametros;
  parametros = 0;
  delete rep;
  rep = 0;
}

void VPresupuesto::aPdf()
{
    // Veo que ID quiere reimprimir.
    QModelIndexList lista = vista->selectionModel()->selectedRows();
    if( lista.isEmpty() ) {
        QMessageBox::information( this, "Error", "Por favor, seleccione uno o mas presupuestos para reimprimir", QMessageBox::Ok );
        return;
    }
    EReporte *rep = new EReporte( 0 );
    rep->presupuesto();
    ParameterList *parametros = new ParameterList();
    foreach( QModelIndex idx, lista ) {
        parametros->clear();
        int id_presupuesto = modelo->data( modelo->index( idx.row(), 0 ), Qt::EditRole ).toInt() ;
        parametros->append( Parameter( "id_presupuesto", id_presupuesto ) );
        int id_cliente = modelo->data( modelo->index( idx.row(), 0 ), Qt::EditRole ).toInt();
        if( id_cliente < 0 ) {
            parametros->append( Parameter( "cliente_existe", false ) );
            //lista.append( Parameter( "direccion", LEDireccion->text() ) );
        } else if( id_cliente >= 0 ){
            parametros->append( Parameter( "cliente_existe", true ) );
            //lista.append( Parameter( "direccion", LEDireccion->text() ) );
            if( id_cliente > 0 )
                parametros->append( Parameter( "direccion", MClientes::direccionEntera( id_cliente ) ) );
        }
        if( !rep->hacerPDF( *parametros, QString( "Presupuesto #%1" ).arg( id_presupuesto ) ) ) {
            qDebug( "Error la intentar imprimir el reporte" );
        }
    }
    delete parametros;
    parametros = 0;
    delete rep;
    rep = 0;
}

/*!
    \fn VPresupuesto::menuContextual( const QModelIndex &indice, QMenu *menu )
    Muestra el menu contextual con las acciones que pueden ser realizadas sobre un item de presupuesto.
    \param indice Indice del cual ver el item
    \param menu Menu contextual que será mostrado
 */
void VPresupuesto::menuContextual( const QModelIndex &indice, QMenu *menu )
{
 // Agrego las acciones que quiero que aparezcan en el menu
 menu->addAction( ActVerContenido );
 menu->addAction( ActImprimir );
 menu->addAction( ActPdf );
 menu->addAction( ActAFactura );
 menu->addSeparator();
 menu->addAction( ActAgregar );
 indiceMenu = indice;
 return;
}

/*!
 * \fn VPresupuesto::verContenido()
 * Muestra la lista de contenido del presupuesto
 */
void VPresupuesto::verContenido()
{
    // Veo que ID quiere reimprimir.
    QModelIndexList lista = vista->selectionModel()->selectedRows();
    if( lista.isEmpty() ) {
        QMessageBox::information( this, "Error", "Por favor, seleccione uno o mas presupuestos para reimprimir", QMessageBox::Ok );
        return;
    }
    QModelIndex indice = lista.first();
    int id_presupuesto = indice.model()->data( indice.model()->index( indice.row(), 0 ), Qt::EditRole ).toInt();
    VListaPresupuesto *f = new VListaPresupuesto();
    f->setearIdPresupuesto( id_presupuesto );
    agregarVentana( f );
}

/*!
 * \fn VPresupuesto::modificar()
 * Modifica un presupuesto
 */
void VPresupuesto::modificar( QModelIndex indice )
{
    // Veo que ID quiere reimprimir.
    if( !indice.isValid() ) {
        QModelIndexList lista = vista->selectionModel()->selectedRows();
        if( lista.isEmpty() ) {
            QMessageBox::information( this, "Error", "Por favor, seleccione un presupuesto para modificar", QMessageBox::Ok );
            return;
        }
        indice = lista.first();
    }
    FormModificarPresupuesto *f = new FormModificarPresupuesto();
    agregarVentana( f );
    f->setearIdPresupuesto( indice.model()->index( indice.row(), 0 ) );
}
/*!
 * \fn VPresupuesto::eliminar()
 * Elimina el/los presupuestos seleccionados
 */
void VPresupuesto::eliminar()
{
    QModelIndexList lista = vista->selectionModel()->selectedRows();
    if( lista.isEmpty() ) {
        QMessageBox::information( this, "Error", "Por favor, seleccione un presupuesto para eliminar", QMessageBox::Ok );
        return;
    }
    // confirmacion del usuario
    if( QMessageBox::question( this, "¿Seguro?", QString::fromUtf8( "¿Está seguro que desea eliminar %1 presupuesto(s)?").arg( lista.size() ), QMessageBox::Ok, QMessageBox::Cancel ) != QMessageBox::Ok ) {
        return;
    }
    foreach( QModelIndex idx, lista ) {
        // Obtengo el ID
        int idp = idx.model()->data( idx.model()->index( idx.row(), 0 ), Qt::EditRole ).toInt();
        if( !MPresupuesto::eliminarPresupuesto( idp ) ) {
            QMessageBox::warning( this, "Error", QString( "No se pudo eliminar el presupuesto %1" ).arg( idp ) );
        }
    }
    modelo->select();
}

/*!
 * \fn VPresupuesto::aFactura()
 * Genera una factura con el primer item seleccionado
 */
void VPresupuesto::aFactura()
{
    QModelIndexList lista = vista->selectionModel()->selectedRows();
    if( lista.isEmpty() ) {
        QMessageBox::information( this, "Error", "Por favor, seleccione un presupuesto para facturarlos", QMessageBox::Ok );
        return;
    }
    // Obtengo los datos
    QModelIndex idx = lista.first();
    int id_presupuesto = idx.model()->data( idx.model()->index( idx.row(), 0 ), Qt::EditRole ).toInt();
    int id_cliente = MPresupuesto::obtenerIdCliente( id_presupuesto );
    QDate fecha = MPresupuesto::obtenerFecha( id_presupuesto );
    MProductosTotales *mp = new MProductosTotales();
    mp->calcularTotales( true );
    mp->setearTipoPrecioBuscar( MProductosTotales::Venta );
    // Cargo todos los datos de los items
    MItemPresupuesto *m = new MItemPresupuesto();
    m->setearId( id_presupuesto );
    m->select();
    for( int i=0; i<m->rowCount(); i++ ) {
        mp->agregarItem(
                         m->data( m->index( i, 2 ), Qt::EditRole ).toDouble(),
                         m->data( m->index( i, 3 ), Qt::EditRole ).toString(),
                         m->data( m->index( i, 4 ), Qt::EditRole ).toDouble()
                    );
    }
    delete m;
    // Cargo los datos si es necesario.
    MDescuentos *md = new MDescuentos();
    md->setearIdComprobante( id_presupuesto );
    md->setearTipo( MDescuentos::Presupuesto );
    if( md->existenDatos() ) {
        md->seleccionarDatos();
        for( int i=0; i<md->rowCount(); i++ ) {
            mp->agregarDescuento(
                                     md->data( md->index( i, 1 ), Qt::DisplayRole ).toString(),
                                      md->data( md->index( i, 2 ), Qt::EditRole ).toDouble()
                    );
        }
    }
    delete md;

    emit emitirFactura( id_cliente, fecha, mp );
}
