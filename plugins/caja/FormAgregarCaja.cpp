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

#include "FormAgregarCaja.h"
#include "ui_FormAgregarCajaBase.h"

#include "mcajas.h"

#include <QMessageBox>

FormAgregarCaja::FormAgregarCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormAgregarCaja)
{
    ui->setupUi(this);
    this->setWindowTitle( "Agregar nueva caja" );

    ui->DEFechaAlta->setDate( QDate::currentDate() );
    ui->DEFechaAlta->setMaximumDate( QDate::currentDate() );
}

FormAgregarCaja::~FormAgregarCaja()
{
    delete ui;
}

void FormAgregarCaja::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



/*!
 * @fn FormAgregarCaja::accept()
 * Funcion llamada al apretar el boton de aceptar
 */
void FormAgregarCaja::accept()
{
    if( ui->LENombre->text().isEmpty() ) {
        QMessageBox::warning( this, "Error de datos", "El nombre de la caja no puede estar vacio" );
    }
    // Guardo los datos
    MCajas *m = new MCajas( this );
    if( m->existeCaja( ui->LENombre->text() ) ) {
        QMessageBox::warning( this, "Error", "La caja que intenta ingresar ya existe" );
        return;
    }
    if( m->agregarCaja( ui->LENombre->text(), ui->DEFechaAlta->date(), ui->dSBSaldoInicial->value() ) ) {
        QMessageBox::information( this, "Correcto", "Caja agregada correctamente" );
    } else {
        // Error al intentar agregar la caja
        QMessageBox::critical( this, "Error", "No se pudo agregar la caja. Verifique el log" );
    }
    this->close();
}
