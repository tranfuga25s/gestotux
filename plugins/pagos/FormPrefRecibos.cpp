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

#include "FormPrefRecibos.h"
#include "mpagos.h"

FormPrefRecibos::FormPrefRecibos( QWidget *parent ) :
 QWidget( parent ), Ui::FormPrefRecibos() {

    setupUi( this );
    this->setAttribute( Qt::WA_DeleteOnClose );

    connect( PBRenumerar, SIGNAL( clicked() ), this, SLOT( renumerar() ) );

    // Coloco los numeros minimos
    NumeroComprobante numero = MPagos::proximoSerieNumeroRecibo();
    // Calculo el maximo según la cantidad de cifras
    SBSerie->setRange( numero.serie(), 99999  );
    SBNumero->setRange( numero.numero(), 99999 );
}

void FormPrefRecibos::changeEvent(QEvent *e)
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

#include <QMessageBox>
void FormPrefRecibos::renumerar()
{
  // Verifico que el numero sea mayor que el que esta actualmente
    QMessageBox::warning( this, "!Error!", "No implmentado todavia" );
}
