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

#ifndef FORMCLIENTE_H
#define FORMCLIENTE_H

#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include "ui_FormClienteBase.h"
#include "eventana.h"
class MEstadoFiscal;

/*!
 * \brief Formulario de edicion de clientes
 * @author Esteban Zeller <tranfuga25s@gmail.com>
 */
class FormCliente : public EVentana, private Ui::FormClienteBase
{
  Q_OBJECT
    public:
        FormCliente ( QWidget* parent = 0, QSqlRelationalTableModel *modelo = 0,  Qt::WFlags fl = 0 );
        void setearCliente( QModelIndex &indice );
        void setearCliente( int id_cliente );
        void agregar();

    signals:
        void agregarVentana( QWidget * );

    public slots:
        void guardar();

    private slots:
        void rehaceRazonSocial( const QString &texto );

    private:
        bool verificarCuitCuil( QString texto );
        QSqlRelationalTableModel *modelo;
        MEstadoFiscal *_mestadofiscal;
        QDataWidgetMapper *mapa;
        bool _agregando;
};

#endif

