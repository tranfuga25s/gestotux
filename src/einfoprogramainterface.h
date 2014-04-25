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
#ifndef EINFOPROGRAMAINTERFACE_H
#define EINFOPROGRAMAINTERFACE_H

#include <QtPlugin>
#include <QString>
#include <QIcon>
#include <QImage>

#include "../reporte/common/parameter.h"

/*!
 *  \brief Interfaz de informacion del programa
 *
 *   Clase que se utiliza para conocer que metodos va a tener un plugin de informacion
 *   @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class EInfoProgramaInterface
{
public:
    virtual ~EInfoProgramaInterface() {}
    virtual QString nombrePrograma() const = 0;
    virtual QIcon iconoPrograma() const = 0;
    virtual QImage imagenPrograma() const = 0;
    virtual QString directorioBackup() const = 0;
    virtual QString directorioActualizaciones() const = 0;
    virtual double version() const = 0;
    virtual QString empresa() const = 0;
    virtual QString companeros() = 0;
    virtual bool publicidad() = 0;
    virtual QString reporte( int tipo ) = 0;
    virtual void  reporteParametros( int tipo, QString &nombre, ParameterList &parametros ) = 0;
};

Q_DECLARE_INTERFACE( EInfoProgramaInterface,
                     "tranfuga.EInfoPrograma/1.4" )

#endif
