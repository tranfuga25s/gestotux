/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller & Daniel Sequeira		   *
 *   juiraze@yahoo.com.ar  - daniels@hotmail.com			   *
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
#ifndef ERESUMEN_H
#define ERESUMEN_H

#include <eventana.h>
class QCalendarWidget;
class QCheckBox;
class QComboBox;
class QSpinBox;
class QDateEdit;

/**
 *	\brief Clase inicial de resumenes
 *
 *	Clase que contiene los metodos para la generacion de los datos previos a los 3 tipos de resumenes
 *
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class EResumen : public EVentana
{
Q_OBJECT
public:
    enum tipo
    {
     diario = 0x00,
     mensual = 0x01,
     anual = 0x02,
     semanal = 0x03,
     entrefechas = 0x04
    };

    EResumen( QWidget *parent = 0, tipo que = diario );
    ~EResumen();

protected slots:
    void generarResumen();

private:
    tipo generar;
    QCalendarWidget *CWfecha;
    QCheckBox *ChBDetallado;
    QComboBox *CBMes;
    QSpinBox *seleccion, *SBAno;
    QDateEdit *DEInicio, *DEFin;
};

#endif
