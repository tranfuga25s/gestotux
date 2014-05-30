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
#include "formservicio.h"

#include <QDate>
#include <QMessageBox>
#include <QSqlError>
#include <QDataWidgetMapper>
#include <QDebug>

#include "eactcerrar.h"
#include "eactguardar.h"
#include "mrecargos.h"
#include "edrecargos.h"
#include "mservicios.h"
#include "eserviciosdelegate.h"

FormServicio::FormServicio ( MServicios *m, QWidget* parent, Qt::WFlags fl )
: EVentana( parent, fl ), Ui::FormServicioBase(), modelo(0)
{
        setupUi ( this );
        this->setWindowTitle( "Agregar nuevo servicio" );
        this->setWindowIcon( QIcon( ":/imagenes/add.png" ) );

        this->modelo = m;
        _modificando = false;
        _id_servicio = -1;

        // Pongo la fecha de alta en hoy
        DEFechaAlta->setDate( QDate::currentDate() );

        // Coloco los periodos
        CBPeriodo->insertItem( MServicios::Semanal      , "Semanal ( 7 dias )"   , MServicios::Semanal );
        CBPeriodo->insertItem( MServicios::Quincenal    , "Quincenal ( 15 dias )", MServicios::Quincenal );
        CBPeriodo->insertItem( MServicios::Mensual      ,  "Mensual"             , MServicios::Mensual );
        CBPeriodo->insertItem( MServicios::BiMensual    , "Bi-Mensual"           , MServicios::BiMensual );
        CBPeriodo->insertItem( MServicios::Trimestral   , "Trimestral"           , MServicios::Trimestral );
        CBPeriodo->insertItem( MServicios::Cuatrimestral, "Cuatrimestral"        , MServicios::Cuatrimestral );
        CBPeriodo->insertItem( MServicios::Seximestral  , "Semestral"            , MServicios::Seximestral );
        CBPeriodo->insertItem( MServicios::Anual        , "Anual"                , MServicios::Anual );

        // Dias en el mes que se hace el batch de calcular los nuevos importes 1->31 ( cuidado con los meses  28 y 30 )
        for( int i=1; i<=31; i++ )
        { CBInicioCobro->insertItem( i, QString::number( i ), QString::number( i ) ); }

        CBMetodoIncompleto->insertItem( MServicios::DiasFaltantes, "Division por dias y cobro de dias restantes", MServicios::DiasFaltantes );
        CBMetodoIncompleto->insertItem( MServicios::MesCompleto, "Periodo Completo", MServicios::MesCompleto );

        QAction *ActRecargos = new QAction( this );
        ActRecargos->setText( "Recargos" );
        connect( ActRecargos, SIGNAL( triggered() ), this, SLOT( agregarRecargo() ) );

        addAction( new EActGuardar( this ) );
        addAction( ActRecargos );
        addAction( new EActCerrar( this ) );

        connect( CkBBaja, SIGNAL( toggled( bool ) ), this, SLOT( cambiarBaja( bool ) ) );
        DEFechaBaja->setEnabled( CkBBaja->isChecked() );

}

/*!
    \fn FormServicio::guardar()
 */
void FormServicio::guardar()
{
 //Verificacion de si los campos son vacios
 if( LENombre->text().isEmpty() )
 {
     QMessageBox::information( this, "Error", "Por favor, ingrese un nombre para el servicio" );
     return;
 }
 if( !DEFechaAlta->date().isValid() )
 {
     QMessageBox::information( this, "Error", "Por favor, ingrese una fecha de alta valida para el inicio del servicio" );
     return;
 }
 if( dSBPrecioBase->value() <= 0 )
 {
     QMessageBox::information( this, "Error", "Por favor, ingrese un valor del servicio valido" );
     return;
 }
 if( _modificando ) {
     if( CkBBaja->isChecked() ) {
         _mapa->addMapping( DEFechaBaja, modelo->fieldIndex( "fecha_baja" ) );
     }
     if( _mapa->submit() ) {
         if( modelo->submitAll() ) {
            QMessageBox::information( this, "Correcto", "El servicio fue modificado correctamente" );
            /// @TODO: Version 0.6 -> Veo la modificacion del precio para ofrecer sistema de aviso en la proxima factura
            this->close();
            return;
         } else {
             qDebug() << "Error, no se pudo hacer el submitAll del modelo";
             qDebug() << this->modelo->lastError().text();
             qWarning() << "Hubo un error al guardar los datos en la base de datos";
         }
     } else {
         qDebug() << "Error, no se pudo hacer submit del mapa";
         qDebug() << this->modelo->lastError().text();
     }
 } else {
         if( modelo->existe( LENombre->text() ) ) {
             QMessageBox::warning( this, "Error", "Ya existe un servicio con el mismo nombre dado de alta" );
             return;
         }
         if( modelo->agregarServicio( LENombre->text(),
                                      TEDescripcion->toPlainText(),
                                      DEFechaAlta->date(),
                                      dSBPrecioBase->value(),
                                      CBPeriodo->itemData( CBPeriodo->currentIndex() ).toInt(),
                                      CBInicioCobro->itemData( CBInicioCobro->currentIndex() ).toInt(),
                                      CBMetodoIncompleto->itemData( CBMetodoIncompleto->currentIndex() ).toInt()
           ) )
         {
             QMessageBox::information( this, "Correcto", "El servicio fue dado de alta correctamente" );
             emit actualizarVista();
             this->close();
             return;
         } else {
             QMessageBox::information( this, "Incorrecto", "El servicio <b>NO</b> pudo ser dado de alta" );
             qDebug() << "Error de modelo: " << modelo->lastError().text();
             return;
         }
 }
}


/*!
    \fn FormServicio::cambiarBaja( bool estado )
        Slot llamado para habilitar y deshabilitar el selector de fecha de cuando fue dado de baja
 */
void FormServicio::cambiarBaja( bool estado )
{  DEFechaBaja->setEnabled( estado ); }


#include "formrecargos.h"
/*!
    \fn FormServicio::agregarRecargo()
    Abre la ventana para agregar un recargo
 */
void FormServicio::agregarRecargo()
{
    if ( _modificando ) {
        FormRecargos *f = new FormRecargos( this );
        f->setearId( this->_id_servicio );
        emit agregarVentana( f );
        return;
    } else {
        QMessageBox::information( this, "No se puede", "No se puede agregar un recargo ya que todavía no ha sido dado de alta el servicio. \n Esto es necesario para mantener la consistencia de los datos" );
        return;
    }

}

/*!
 * \fn FormServicio::setearId( const int id_servicio, const QModelIndex indice )
 * Coloca el formulario en modo de edicion y carga los datos para que sean modificados.
 * \param id_servicio Identificador del servicio a cargar
 * \param indice Indice de la vista para pasar al mapa
 */
void FormServicio::setearId( const int id_servicio, const QModelIndex indice )
{
    if( !indice.isValid() ) {
        qWarning( "El indice pasado es invalido!" );
        abort();
    }
    this->modelo->setEditStrategy( QSqlTableModel::OnManualSubmit );
    this->_modificando = true;

    _mapa = new QDataWidgetMapper( this );
    _mapa->setOrientation( Qt::Horizontal );
    _mapa->setModel( this->modelo );
    _mapa->setSubmitPolicy( QDataWidgetMapper::ManualSubmit );
    _mapa->setItemDelegate( new EServiciosDelegate( _mapa ) ); /// ESTO PUEDE SER PROBLEMATICO SI NO ESTA TOTALMENTE IMPLEMENTADO

    _mapa->addMapping( LENombre          , modelo->fieldIndex( "nombre" )           );
    _mapa->addMapping( TEDescripcion     , modelo->fieldIndex( "descripcion" )      );
    _mapa->addMapping( DEFechaAlta       , modelo->fieldIndex( "fecha_alta" )       );
    _mapa->addMapping( dSBPrecioBase     , modelo->fieldIndex( "precio_base" )      );
    _mapa->addMapping( CBPeriodo         , modelo->fieldIndex( "periodo" )          );
    _mapa->addMapping( CBMetodoIncompleto, modelo->fieldIndex( "forma_incompleto" ) );
    _mapa->addMapping( CBInicioCobro     , modelo->fieldIndex( "dia_cobro" )        );

    if( this->modelo->data( this->modelo->index( indice.row(), this->modelo->fieldIndex( "fecha_baja" ) ), Qt::EditRole ).toDate().isValid() ) {
        _mapa->addMapping( DEFechaBaja, modelo->fieldIndex( "fecha_baja" ) );
        CkBBaja->setChecked( true );
    }

    // Busco el indice
    this->_id_servicio = id_servicio;
    this->_mapa->setCurrentModelIndex( indice );

}
