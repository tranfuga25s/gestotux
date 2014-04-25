/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller & Daniel Sequeira		   *
 *   juiraze@yahoo.com.ar  - daniels@hotmail.com			   *
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
#include "formagregargasto.h"

#include <QPushButton>
#include <QComboBox>
#include <QCalendarWidget>
#include <QDate>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "mgasto.h"
#include "eactguardar.h"
#include "eactcerrar.h"
#include "eregistroplugins.h"
#include "preferencias.h"
#include "../caja/mcajas.h"
#include "mcategoriasgastos.h"

FormAgregarGasto::FormAgregarGasto( QWidget* parent )
: EVentana( parent ), Ui::FormAgregarGastoBase()
{
        setupUi( this );
        setObjectName( "AgregarGasto" );
        setWindowTitle( "Agregar nuevo gasto" );
        setWindowIcon( QIcon( ":/imagenes/gasto.jpg" ) );

        this->addAction( new EActGuardar( this ) );
        this->addAction( new EActCerrar( this ) );

        modeloCombo = new MCategoriasGastos( CBTipo );
        CBTipo->setModel( modeloCombo );
        CBTipo->setModelColumn( 1 );
        CBTipo->setInsertPolicy( QComboBox::NoInsert );
        modeloCombo->select();
        CBTipo->setCurrentIndex( -1 );

        CWFecha->setSelectedDate( QDate::currentDate() );
        CWFecha->setMaximumDate( QDate::currentDate() );

        modeloEdit = new QSqlQueryModel( this );
        modeloEdit->setQuery( "SELECT DISTINCT descripcion FROM gastos" );
        CBDescripcion->setModel( modeloEdit );
        CBDescripcion->setEditable( true );
        CBDescripcion->setCurrentIndex( -1 );

        if( ERegistroPlugins::getInstancia()->existePluginExterno( "caja" ) ) {
            CBCajas->setModel( new MCajas( this ) );
            CBCajas->setModelColumn( 1 );
            qobject_cast<QSqlTableModel *>(CBCajas->model())->select();
            CBCajas->setCurrentIndex( -1 );
        } else {
            CkBSacarCaja->setVisible( false );
            CBCajas->setVisible( false );
        }
}

#include "../caja/mmovimientoscaja.h"
/*!
    \fn FormAgregarGasto::guardar()
 */
void FormAgregarGasto::guardar()
{
 if( CBDescripcion->currentText().isEmpty() )
 {
  QMessageBox::warning( this, "Faltan Datos", "Por favor, ingrese una descripcion para el gasto" );
  return;
 }
 else if( CBTipo->currentIndex() < 0 )
 {
  QMessageBox::warning( this, "Faltan Datos", "Por favor, ingrese una categoria para el gasto" );
  return;
 }
 else if( dSBCosto->value() <= 0 )
 {
  QMessageBox::warning( this, "Faltan Datos", "Por favor, ingrese un costo para este gasto" );
  return;
 }
 MGasto *modeloGastos = new MGasto( this );
 if( modeloGastos->existe( CBDescripcion->currentText(), dSBCosto->value(), CBTipo->currentIndex() ) ) {
     QMessageBox::warning( this, "Datos duplicados", "El gasto ingresado ya existe en la base de datos" );
     return;
 }
 else
 {
  if( ERegistroPlugins::getInstancia()->existePluginExterno( "caja" ) ) {
      preferencias *p = preferencias::getInstancia();
      p->beginGroup( "Preferencias" );
      p->beginGroup( "Caja" );
      bool gastossinfondos = p->value( "gastos-sinfondos" ).toBool();
      p->endGroup();p->endGroup();p=0;
      if( gastossinfondos && this->CkBSacarCaja->isChecked() )
      {
          int id_caja = CBCajas->model()->data( CBCajas->model()->index( CBCajas->currentIndex(), 0 ), Qt::EditRole ).toInt();
          if( MCajas::saldo( id_caja ) < dSBCosto->value() ) {
              QMessageBox::warning( this, "Error", "El saldo en esta caja no es suficiente para ingresar este gasto. Modifique las preferencias para evitar este dialogo" );
              return;
          }
      }
  }
  if ( modeloGastos->agregarGasto( CBDescripcion->currentText(),
                                dSBCosto->value(),
                                CWFecha->selectedDate(),
                                CBTipo->model()->data( CBTipo->model()->index( CBTipo->currentIndex(), 0 ), Qt::EditRole ).toInt() ) == true )
  {
        if( ERegistroPlugins::getInstancia()->existePluginExterno( "caja" )  && CkBSacarCaja->isChecked() ) {
             MMovimientosCaja *m = new MMovimientosCaja();
             int id_caja = CBCajas->model()->data( CBCajas->model()->index( CBCajas->currentIndex(), 0 ), Qt::EditRole ).toInt();
             if( !m->agregarMovimiento( id_caja, CBDescripcion->currentText(), QString(), 0.0, dSBCosto->value() ) ) {
                 QMessageBox::information( this, "Error", "El gasto se guardo correctamente, pero no se pudo registrar la operacion en la cuenta de caja" );
             }
             // enlazo los datos
             int id_mov = m->ultimoIdInsertado();
             qDebug() << "Ultimo id movimiento: " << id_mov;
             if( !modeloGastos->setearIdMovimiento( id_mov, dSBCosto->value() ) ) {
                 QMessageBox::information( this, "Error", "El gasto se guardo correctamente pero no se pudo relacionar la entrada de caja con el gasto." );
             }
             modeloGastos->submitAll();
             m->submitAll();
             delete m;
         } else {

             QMessageBox::information( this, "Correcto", "El gasto se han agregado correctamente" );
         }
  }
  else
  {
      QMessageBox::information( this, "Error de base de datos", "Los datos del gasto no se han agregado correctamente" );
      qDebug( "Error de guardar" );
      qDebug() << "Detalles: tipo: " << modeloGastos->lastError().type()  << ", errno: " << modeloGastos->lastError().number() << ", descripcion: " << modeloGastos->lastError().text();
      qDebug() << "Detalles: cola:" << modeloGastos->query().lastQuery();
  }
  delete modeloGastos;
 }
 emit actualizarVista();
 this->close();
 return;
}
