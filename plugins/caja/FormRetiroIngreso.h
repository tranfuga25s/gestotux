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

#ifndef FORMRETIROINGRESO_H
#define FORMRETIROINGRESO_H

#include "ui_FormRetiroIngresoBase.h"
#include <QDialog>

/*!
 * @brief Formulario para ingresar o retirar dinero de una caja
 */
class FormRetiroIngreso : public QDialog, private Ui::FormRetiroIngreso
{
    Q_OBJECT
public:
    /*!
     * @enum Modo
     * Permite definir la accion a llevar a cabo al realizar la consutccion del formulario
     */
    enum Modo {
        Retiro = 0,
        Ingreso = 1
    };

    explicit FormRetiroIngreso( QWidget *parent = 0, Modo modo = Retiro );
    void setearCaja( const int id_caja );

protected slots:
    void ingreso();
    void retiro();

private:
    bool verificarParametros();

protected:
    void changeEvent(QEvent *e);
};

#endif // FORMRETIROINGRESO_H
