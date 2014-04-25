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
#include "vservicios.h"

#include "mservicios.h"
#include "formasociarserviciocliente.h"
#include "formservicio.h"
#include "FormFacturarServicio.h"
#include "mperiodoservicio.h"
#include "vlistaperiodos.h"
#include "FormClientesAdheridos.h"
#include "formrecargos.h"

#include <QTableView>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

VServicios::VServicios(QWidget *parent)
 : EVLista(parent)
{
 this->setObjectName( "visorServicios" );
 this->setWindowTitle( "Servicios" );
 this->setWindowIcon( QIcon( ":/imagenes/servicios.png" ) );

 modelo = new MServicios( this );

 vista->setModel( modelo );
 vista->setAlternatingRowColors( true );
 modelo->setFilter( "fecha_baja IS NULL" );
 modelo->select();
 vista->hideColumn( 0 );
 vista->hideColumn( 2 );
 vista->hideColumn( modelo->fieldIndex( "fecha_baja" ) );
 vista->setSortingEnabled( true );
 vista->horizontalHeader()->setResizeMode( modelo->fieldIndex( "nombre" ), QHeaderView::ResizeToContents );

 connect( vista, SIGNAL( doubleClicked ( const QModelIndex & ) ), this, SLOT( modificar( const QModelIndex & ) ) );

 ActNuevoCliente = new QAction( this );
 ActNuevoCliente->setText( "Adherir Cliente" );
 ActNuevoCliente->setIcon( QIcon( ":/imagenes/adherir_cliente.png" ) );
 ActNuevoCliente->setStatusTip( "Adhiere un cliente al servicio seleccionado en la lista" );
 connect( ActNuevoCliente, SIGNAL( triggered() ), this, SLOT( darAltaServicioCliente() ) );

 ActVerClientes = new QAction( this );
 ActVerClientes->setText( "Ver Clientes adheridos" );
 ActVerClientes->setIcon( QIcon( ":/imagenes/clientes_adheridos.png" ) );
 ActVerClientes->setStatusTip( "Muestra la lista de clientes adheridos a este servicio" );
 connect( ActVerClientes, SIGNAL( triggered() ), this, SLOT( verClientesAdheridos() ) );

 ActGenerarFacturacion = new QAction( this );
 ActGenerarFacturacion->setText( "Generar Facturacion" );
 ActGenerarFacturacion->setIcon( QIcon( ":/imagenes/facturar_servicio.png" ) );
 ActGenerarFacturacion->setStatusTip( "Genera todas las facturas para el periodo que se desea cobrar automaticamente" );
 connect( ActGenerarFacturacion, SIGNAL( triggered() ), this, SLOT( generarFacturacion() ) );

 ActVerPeriodosFacturados = new QAction( this );
 ActVerPeriodosFacturados->setText( "Ver periodos" );
 ActVerPeriodosFacturados->setIcon( QIcon( ":/imagenes/periodos.png" ) );
 ActVerPeriodosFacturados->setStatusTip( "Permite obtener una lista de todos los periodos facturados del servicio seleccionado" );
 connect( ActVerPeriodosFacturados, SIGNAL( triggered() ), this, SLOT( verPeriodos() ) );

 ActRecargos = new QAction( this );
 ActRecargos->setText( "Administrar Recargos" );
 ActRecargos->setStatusTip( "Administra los recargos posibles para este servicio" );
 connect( ActRecargos, SIGNAL( triggered() ), this, SLOT( verRecargos() ) );

 ActDarDeBaja = new QAction( this );
 ActDarDeBaja->setText( "Dar de baja" );
 ActDarDeBaja->setIcon( QIcon( ":/imagenes/baja_servicio.png" ) );
 connect( ActDarDeBaja, SIGNAL( triggered() ), this, SLOT( darDeBaja() ) );

 QAction *ActSep = new QAction( this );
 ActSep->setSeparator( true );

 QAction *ActSep2 = new QAction( this );
 ActSep2->setSeparator( true );

 ActVerDadosBaja = new QAction( this );
 ActVerDadosBaja->setText( "Ver dados de baja" );
 ActVerDadosBaja->setCheckable( true );
 ActVerDadosBaja->setChecked( false );
 cambioVerBaja( ActVerDadosBaja->isChecked() );
 connect( ActVerDadosBaja, SIGNAL( toggled( bool ) ), this, SLOT( cambioVerBaja( bool ) ) );

 addAction( ActAgregar );
 addAction( ActModificar );
 //addAction( ActEliminar );
 addAction( ActSep );

 addAction( ActNuevoCliente );
 addAction( ActVerClientes );
 addAction( ActGenerarFacturacion );
 addAction( ActVerPeriodosFacturados );
 addAction( ActRecargos );
 addAction( ActDarDeBaja );
 addAction( ActSep2 );

 addAction( ActCerrar );
 addAction( ActVerDadosBaja );
}

/*!
 * \fn VServicios::agregar( bool autoeliminarid )
 * Slot llamado para agregar un servicio
 */
void VServicios::agregar( bool /*autoeliminarid*/ )
{
  FormServicio *f = new FormServicio( qobject_cast<MServicios *>( this->modelo ) );
  connect( f, SIGNAL( actualizarVista() ), this, SLOT( actualizarVista() ) );
  emit agregarVentana( f );
}

/*!
 * \fn VServicios::modificar( const QModelIndex &idx )
 * Slot llamado para modificar los datos de un servicio
 * \param idx Indice a modificar
 */
void VServicios::modificar( const QModelIndex &idx )
{
    // modifico el indice actual ( existe uno actual? )
    if( !idx.isValid() ) {
        qDebug( "indice invalido" );
        return;
    }
    // Obtengo el id actual
    int id_servicio = idx.model()->data( idx.model()->index( idx.row(), 0 ), Qt::EditRole ).toInt();
    if( id_servicio <= 0 ) {
        qWarning( "El identificador del servicio encontrado es invalido" );
        return;
    }
    FormServicio *f = new FormServicio( qobject_cast<MServicios *>(this->modelo ) );
    f->setearId( id_servicio, idx.model()->index( idx.row(), 0 ) );
    connect( f, SIGNAL( actualizarVista() ), this, SLOT( actualizarVista() ) );
    emit agregarVentana( f );
}

/*!
 * \fn VServicios::modificar()
 * Slot llamado para modificar el item seleccionado
 */
void VServicios::modificar()
{
    if( this->vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::warning( this, "Error", "Por favor, seleccione un servicio para modificar" );
        return;
    }
    this->modificar( this->vista->selectionModel()->selectedRows().first() );
}


/*!
 * \fn VServicios::eliminar()
 * Implementación de slot de eliminar con verificacion de si se puede o no
 */
void VServicios::eliminar()
{
    if( this->vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::warning( this, "Error", "Seleccione un servicio para intentar eliminarlo" );
        return;
    }
    QModelIndex idx = this->vista->selectionModel()->selectedRows().first();
    int id_servicio = this->vista->model()->data( this->vista->model()->index( idx.row(), 0 ) ).toInt();
    if( ! MServicios::verificarSiPuedeEliminar( id_servicio ) ) {
        QMessageBox::warning( this, "Error", "Existen datos asociados con este servicio que esta intentando eliminar. No se podrá eliminar." );
        return;
    }
    if( this->vista->model()->removeRow( idx.row() ) ) {
        QMessageBox::information( this, "Correcto", "Servicio eliminado correctamente" );
        return;
    } else {
        qDebug( "Error al eliminar el servicio" );
        qDebug() << qobject_cast<QSqlTableModel *>( this->vista->model() )->lastError().text();
        return;
    }
}


/*!
 * \fn VServicios::menuContextual( const QModelIndex &indice, QMenu *menu )
 * Slot llamado para generar el menú contextual del servicio
 * \param indice Indice desde donde se solicita el menú
 * \param menu Menu contextual
 */
void VServicios::menuContextual( const QModelIndex &indice, QMenu *menu )
{
 // Agrego las acciones que quiero que aparezcan en el menu
 menu->addAction( ActAgregar );
 menu->addAction( ActNuevoCliente );
 menu->addAction( ActVerClientes );
 menu->addSeparator();
 menu->addAction( ActGenerarFacturacion );
 menu->addAction( ActRecargos );
 menu->addAction( ActVerPeriodosFacturados );
 indiceMenu = indice;
 return;
}


/*!
 * \fn VServicios::darAltaServicioCliente()
 * Slot llamado para dar de alta un cliente a un servicio
 */
void VServicios::darAltaServicioCliente()
{
 // El item actual seleccionado es en el cual se pidio el menu
 if( vista->selectionModel()->selectedRows().count() <= 0 ) {
   QMessageBox::warning( this, "Faltan Datos", "Por favor, seleccione un servicio para asociarle un cliente" );
   return;
 }

 // Obtengo el numero de servicio de la vista...
 int id_servicio = vista->model()->data( vista->model()->index( vista->currentIndex().row(), 0 ), Qt::EditRole ).toInt();

 if( MServicios::dadoDeBaja( id_servicio ) ) {
     QMessageBox::information( this, "Incorrecto", "No se puede adherir un cliente al servicio ya que fue dado de baja" );
     return;
 }
 FormAsociarServicioCliente *f = new FormAsociarServicioCliente( this, FormAsociarServicioCliente::Cliente );
 f->setIdServicio( id_servicio );
 connect( f, SIGNAL( editarCliente( int ) ), this, SIGNAL( editarCliente( int ) ) );
 f->exec();
}


/*!
 * \fn VServicios::verClientesAdheridos()
 * LLama al formulario que muestra la lista de clientes que estan adheridos al servicio
 */
void VServicios::verClientesAdheridos()
{
 if( vista->selectionModel()->selectedRows().count() <= 0 ) {
   QMessageBox::warning( this, "Faltan Datos", "Por favor, seleccione un servicio para ver sus clientes adheridos" );
   return;
 }
 // Obtengo el numero de servicio de la vista...
 int id_servicio = vista->model()->data( vista->model()->index( vista->currentIndex().row(), 0 ), Qt::EditRole ).toInt();
 FormClientesAdheridos *f = new FormClientesAdheridos( this );
 f->setServicioInicial( id_servicio );
 emit agregarVentana( f );
}


/*!
 * \fn VServicios::verRecargos()
 * Llama al formulario que muestra los recargos por servicio
 */
void VServicios::verRecargos()
{
 if( vista->selectionModel()->selectedRows().count() <= 0 ) {
    QMessageBox::warning( this, "Faltan Datos", "Por favor, seleccione un servicio para administrar sus recargos" );
    return;
 }
 // Obtengo el numero de servicio de la vista...
 int id_servicio = modelo->data( modelo->index( vista->currentIndex().row(), 0 ) ).toInt();
 // Muestro el formulario de recargos con el id seteado
 FormRecargos *f = new FormRecargos();
 f->setearId( id_servicio );
 emit agregarVentana( f );
}

/*!
 * \fn VServicios::generarFacturacion()
 * Abre la ventana para generar la facturacion
 */
void VServicios::generarFacturacion()
{
 if( vista->selectionModel()->selectedRows().count() <= 0 ) {
   QMessageBox::warning( this, "Faltan Datos", "Por favor, seleccione un servicio para generar su facturacion" );
   return;
 }
 // Obtengo el numero de servicio de la vista...
 int id_servicio = modelo->data( modelo->index( vista->currentIndex().row(), 0 ) ).toInt();
 if( MServicios::dadoDeBaja( id_servicio ) ) {
     QMessageBox::information( this, "Incorrecto", "No se puede facturar el servicio ya que fue dado de baja" );
     return;
 }
 // Muestro el formulario de recargos con el id seteado
 FormFacturarServicio *f  = new FormFacturarServicio();
 f->setearServicio( id_servicio );
 emit agregarVentana( f );
}

/*!
 * \fn VServicios::darDeBaja()
 * Da de baja el servicio seleccionado
 */
void VServicios::darDeBaja()
{
    if( vista->selectionModel()->selectedRows().count() <= 0 ) {
      QMessageBox::warning( this, "Faltan Datos", "Por favor, seleccione un servicio para darlo de baja" );
      return;
    }

    QMessageBox::StandardButton ret;
    ret = QMessageBox::question( this, QString::fromUtf8("¿Seguro?" ), QString::fromUtf8( "¿Está seguro que desea dar de baja el servicio seleccionado?" ), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if ( ret != QMessageBox::Yes )
        return;

    int id_servicio = modelo->data( modelo->index( vista->currentIndex().row(), 0 ) ).toInt();
    if( qobject_cast<MServicios *>(modelo)->darDeBaja( id_servicio, vista->currentIndex() ) ) {
        QMessageBox::information( this, "Correcto", QString::fromUtf8( "Servicio dado de baja correctamente. \n No se podrá facturar ni asociar clientes a el de ahora en adelante." ) );
        return;
    } else {
        QMessageBox::warning( this, "Incorrecto", "No se dio de baja el servicio" );
        return;
    }
}

/*!
 * \fn VServicios::verPeriodos()
 * Slot llamado para ver los periodos facturados
 */
void VServicios::verPeriodos()
{
    // Busco el servicio que esta seleccionado
    if( vista->selectionModel()->selectedRows().count() <= 0 ) {
      QMessageBox::warning( this, "Faltan Datos", "Por favor, seleccione un servicio para ver sus periodos facturados." );
      return;
    }
    int id_servicio = modelo->data( modelo->index( vista->currentIndex().row(), 0 ) ).toInt();
    if( !MPeriodoServicio::existeFacturacion( id_servicio )  ) {
        QMessageBox::warning( this, "No hay facturacion", QString::fromUtf8( "No se ha generado ninguna facturación para este servicio." ) );
        return;
    }
    emit agregarVentana( new VListaPeriodos( id_servicio ) );
}

/*!
 * \fn VServicios::actualizarVista()
 * Actualiza el listado luego de las modificaciones
 */
void VServicios::actualizarVista()
{  modelo->select();  }

/*!
 * \fn VServicios::cambioVerBaja( bool estado )
 * Slot llamado cuando se cambia la acción de ver dados de baja
 * \param estado Estado de la accion
 */
void VServicios::cambioVerBaja( bool estado )
{
    if( estado ) {
        modelo->setFilter( " fecha_baja IS NOT NULL " );
        vista->showColumn( modelo->fieldIndex( "fecha_baja" ) );
        modelo->select();
    } else {
        modelo->setFilter( " fecha_baja IS NULL " );
        vista->hideColumn( modelo->fieldIndex( "fecha_baja" ) );
        modelo->select();
    }
}
