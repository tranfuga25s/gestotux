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

#include "FormRetiroIngreso.h"
#include <QMessageBox>
#include "mmovimientoscaja.h"
#include "mcajas.h"

FormRetiroIngreso::FormRetiroIngreso( QWidget *parent, Modo modo ) :
    QDialog(parent) {
    setupUi(this);
    this->setObjectName( "ingresoretirocaja" );
    this->PBCancelar->setIcon( QIcon( ":/imagenes/cerrar.png" ) );
    if( modo == Retiro ) {
        this->setWindowTitle( "Retiro de caja" );
        this->setWindowIcon( QIcon( ":/imagenes/cajaretiro.png" ) );
        connect( PBAceptar, SIGNAL( clicked() ), this, SLOT( retiro() ) );
        this->GBTitulo->setTitle( "Retiro de dinero de caja" );
        this->PBAceptar->setText( "Retirar" );
        this->PBAceptar->setIcon( QIcon( ":/imagenes/retiro.png" ) );
    } else if( modo == Ingreso ) {
        this->setWindowTitle( "Ingreso a caja" );
        this->setWindowIcon( QIcon( ":/imagenes/cajaingreso.png" ) );
        connect( PBAceptar, SIGNAL( clicked() ), this, SLOT( ingreso() ) );
        this->GBTitulo->setTitle( "Ingreso de dinero a caja" );
        this->PBAceptar->setText( "Ingresar" );
        this->PBAceptar->setIcon( QIcon( ":/imagenes/ingreso.png" ) );
    }
    MCajas *modeloCaja = new MCajas( this->CBCaja );
    this->CBCaja->setModel( modeloCaja );
    this->CBCaja->setModelColumn( 1 );
    modeloCaja->select();
    if( modeloCaja->rowCount() == 1 ) {
        this->CBCaja->setCurrentIndex( 1 );
    } else {
        this->CBCaja->setCurrentIndex( -1 );
    }

}

void FormRetiroIngreso::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void FormRetiroIngreso::ingreso()
{
    if( !verificarParametros() ) { return; }
    // Ingreso como movimiento de caja el ingreso de dinero
    MMovimientosCaja *mmov = new MMovimientosCaja( this );
    int id_caja = CBCaja->model()->data( CBCaja->model()->index( CBCaja->currentIndex(), 0 ), Qt::EditRole ).toInt();
    QString razon = QString( "Ingreso de caja" );
    if( !this->LENota->text().isEmpty() ) {
        razon.append( "  " + this->LENota->text() );
    }
    if( mmov->agregarMovimiento( id_caja, razon, QString(), DSBCantidad->value() ) ) {
        QMessageBox::information( this, "Correcto", "El ingreso a caja se ha registrado correctamente" );
        delete mmov;
        this->close();
        return;
    } else {
        qWarning( "Error al intentar agregar el movimiento de caja, no se registro el ingreso." );
        delete mmov;
        return;
    }
}

void FormRetiroIngreso::retiro()
{
    if( !verificarParametros() ) { return; }
    // Verifico el saldo para la caja
    int id_caja = CBCaja->model()->data( CBCaja->model()->index( CBCaja->currentIndex(), 0 ), Qt::EditRole ).toInt();
    if( DSBCantidad->value() > MCajas::saldo( id_caja ) ) {
        QMessageBox::warning( this, "Error de saldo", "Esta intentando retirar una cantidad mayor a la existente en la caja seleccionada." );
        return;
    }
    MMovimientosCaja *mmov = new MMovimientosCaja( this );
    QString razon = QString( "Retiro de dinero de caja" );
    if( !this->LENota->text().isEmpty() ) {
        razon.append( "  " + this->LENota->text() );
    }
    if( mmov->agregarMovimiento( id_caja, razon, QString(), 0.0, DSBCantidad->value() ) ) {
        QMessageBox::information( this, "Correcto", "El retiro de caja se ha registrado correctamente" );
        delete mmov;
        this->close();
        return;
    } else {
        qWarning( "Error al intentar agregar el movimiento de caja, no se registro el retiro." );
        delete mmov;
        return;
    }

}

bool FormRetiroIngreso::verificarParametros()
{
    // Verifico el id de la caja
    if( this->CBCaja->currentIndex() < 0 ) {
        QMessageBox::warning( this, "Faltan datos", "Por favor seleccione una caja" );
        return false;
    }
    // verifico la cantidad
    if( this->DSBCantidad->value() <= 0 ) {
        QMessageBox::warning( this, "Fallo en los datos", "Por favor ingrese una cantidad valida." );
        return false;
    }
    return true;
}

void FormRetiroIngreso::setearCaja( const int id_caja ) {
  this->CBCaja->setCurrentIndex( id_caja );
}
