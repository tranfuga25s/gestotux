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
#include "formprefemail.h"

#include <QSqlError>
#include <QDebug>

FormPrefEmail::FormPrefEmail(QWidget* parent, Qt::WFlags fl)
: EVentana( parent, fl ), Ui::FormPrefEmailBase()
{
        setupUi(this);
        servidores = new EServidorEmail( LVServidores );

        LVServidores->setModel( servidores );
        LVServidores->setModelColumn( servidores->fieldIndex( "nombre" ) );

        mapeador = new QDataWidgetMapper( this );
        mapeador->setModel( servidores );
        mapeador->addMapping( LENombre, servidores->fieldIndex( "nombre" ) );
        mapeador->addMapping( LEDireccion, servidores->fieldIndex( "direccion" ) );
        mapeador->addMapping( SBPuerto, servidores->fieldIndex( "puerto" ) );
        mapeador->addMapping( LEUsuario, servidores->fieldIndex( "usuario" ) );
        mapeador->addMapping( LEPass, servidores->fieldIndex( "pass" ) );
        mapeador->addMapping( CkBPredeterminado, servidores->fieldIndex( "predeterminado" ) );
        mapeador->addMapping( LEFrom, servidores->fieldIndex( "de" ) );
        mapeador->addMapping( LEDireccionOrigen, servidores->fieldIndex( "direccion_origen" ) );
        mapeador->addMapping( CkBEcriptacion, servidores->fieldIndex( "encriptacion" ) );
        mapeador->setSubmitPolicy( QDataWidgetMapper::ManualSubmit );

        connect(LVServidores->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
             mapeador, SLOT(setCurrentModelIndex(QModelIndex)));

        mapeador->toFirst();

        connect( PBAgregar, SIGNAL( clicked() ), this, SLOT( agregar() ) );
        connect( PBEliminar, SIGNAL( clicked() ), this, SLOT( eliminar() ) );

        PBAgregar->setIcon( QIcon( ":/imagenes/add.png" ) );
        PBEliminar->setIcon( QIcon( ":/imagenes/stop.png" ) );

        this->setWindowTitle( "Servidor Em@il" );
        this->setWindowIcon( QIcon( ":/imagenes/servidor_email.png" ) );
}

FormPrefEmail::~FormPrefEmail()
{
}


/*!
    \fn FormPrefEmail::guardar()
 */
void FormPrefEmail::guardar()
{
 if( !mapeador->submit() )
 {
     if( qobject_cast<EServidorEmail *>(mapeador->model())->lastError().isValid() ) {
        qWarning() << "Error de submit:" << qobject_cast<EServidorEmail *>(mapeador->model())->lastError().text();
     }
 }
}


/*!
    \fn FormPrefEmail::aplicar()
 */
void FormPrefEmail::aplicar()
{
 if( !mapeador->submit() )
 {
     if( qobject_cast<EServidorEmail *>(mapeador->model())->lastError().isValid() ) {
        qWarning() << "Error de submit: " << qobject_cast<EServidorEmail *>(mapeador->model())->lastError().text();
     }
 }
}


/*!
    \fn FormPrefEmail::cargar()
 */
void FormPrefEmail::cargar()
{
 servidores->select();
}

#include <QSqlRecord>
/*!
    \fn FormPrefEmail::agregar()
 */
void FormPrefEmail::agregar()
{
 QSqlRecord registro = servidores->record();
 registro.setValue( "nombre", "Nuevo Servidor" );
 registro.setValue( "direccion", "direccion.com" );
 registro.setValue( "usuario", "usuario" );
 registro.setValue( "pass", "123456" );
 registro.setValue( "puerto", 21 );
 registro.setValue( "de", "usuario@direccion.com" );
 servidores->insertRecord( -1, registro );
}

#include <QMessageBox>
/*!
    \fn FormPrefEmail::eliminar()
 */
void FormPrefEmail::eliminar()
{
 if( LVServidores->selectionModel()->selectedIndexes().count() <= 0 )
 {
  QMessageBox::information( this, "Error", "Por favor, seleccione el servidor que desea eliminar." );
  return;
 }
//Hacer dialogo de confirmacion..
 int ret;
 ret = QMessageBox::warning( this, "Esta seguro?",
                   QString( "Esta seguro de eliminar %1 servidor(es)?").arg( LVServidores->selectionModel()->selectedIndexes().count() ),
                   "Si", "No" );
 if ( ret == 0 )
 {
        QModelIndexList indices = LVServidores->selectionModel()->selectedIndexes();
        QModelIndex indice;
        foreach( indice, indices )
        {
                if( indice.isValid() )
                {
                        servidores->removeRow( indice.row() );
                }
        }
 }
 return;
}
