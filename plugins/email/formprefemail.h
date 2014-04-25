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

#ifndef FORMPREFEMAIL_H
#define FORMPREFEMAIL_H

#include "eventana.h"
#include "formprefhijo.h"
#include "ui_FormPrefEmailBase.h"
#include "eservidoremail.h"
#include <QDataWidgetMapper>

/*!
 * @brief Formulario de preferencias del plugin de email
 *
 * Formulario de preferencias del plugin de email que muestra la lista de servidores, permite ABM y setear el servidor predefinido.
 * @author Esteban Zeller <tranfuga25s@gmail.com>
 */
class FormPrefEmail : public EVentana, public FormPrefHijo, private Ui::FormPrefEmailBase
{
  Q_OBJECT

public:
    FormPrefEmail( QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~FormPrefEmail();


public slots:
    void guardar();
    void aplicar();
    void cargar();

private:
    EServidorEmail *servidores;
    QDataWidgetMapper *mapeador;

private slots:
    void agregar();
    void eliminar();
};

#endif

