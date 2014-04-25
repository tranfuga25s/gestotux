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

#include "FormEstadoCaja.h"
#include "ui_FormEstadoCajaBase.h"

#include "mcajas.h"

FormEstadoCaja::FormEstadoCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormEstadoCaja)
{
    ui->setupUi(this);
    this->setWindowTitle( "Estado de caja" );
    this->setAttribute( Qt::WA_DeleteOnClose );

    modelo = new MCajas( ui->CBCaja );
    ui->CBCaja->setModel( modelo );
    ui->CBCaja->setModelColumn( 1 );
    connect( ui->CBCaja, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioCaja( int ) ) );
    qobject_cast<QSqlTableModel *>(ui->CBCaja->model())->select();

}

FormEstadoCaja::~FormEstadoCaja()
{
    delete ui;
    if( modelo != 0 )
        delete modelo;
}

void FormEstadoCaja::changeEvent(QEvent *e)
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
 * @fn FormEstadoCaja::cambioCaja( int num )
 * Slot llamado cada vez que se cambia la caja en el combobox
 */
void FormEstadoCaja::cambioCaja( int num )
{
    int num_caja = ui->CBCaja->model()->data( ui->CBCaja->model()->index( num, 0 ), Qt::DisplayRole ).toInt();
    ui->dSBSaldo->setValue( MCajas::saldo( num_caja ) );
}
