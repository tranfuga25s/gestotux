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

#ifndef FORMPLUGINS_H
#define FORMPLUGINS_H

#include <QDialog>

namespace Ui {
    class FormPlugins;
}

/*!
 * @brief Clase para mostrar informaci�n sobre los plugins y sus versiones
 *
 * Formulario para mostrar en la ayuda, los distintos componentes cargados en el programa, sus versiones y sus ultimas actualizaciones.
 * @author Esteban Zeller <tranfuga25s@gmail.com>
 */
class FormPlugins : public QDialog
{
    Q_OBJECT

public:
    explicit FormPlugins(QWidget *parent = 0);
    ~FormPlugins();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FormPlugins *ui;
};

#endif // FORMPLUGINS_H
