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

#ifndef FORMFILTRO_H
#define FORMFILTRO_H

#include <QDialog>
#include "ui_FormFiltrosBase.h"

class FormFiltro : public QDialog, private Ui::FormFiltrosBase
{
  Q_OBJECT

public:
  FormFiltro(QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~FormFiltro();
    bool tieneCategoria() const;
    bool tieneEstablecimiento() const;
    bool tieneFechaEspecifica() const;
    bool tieneTipo() const;
    bool tieneRangoFecha() const;
    QDate FechaEspecifica() const;
    int categoria() const;
    int establecimiento() const;
    QPair<QDate, QDate> rangoFechas() const;
    int tipo() const;
};

#endif

