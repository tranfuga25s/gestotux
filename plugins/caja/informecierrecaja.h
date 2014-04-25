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

#ifndef INFORMECIERRECAJA_H
#define INFORMECIERRECAJA_H

#include <QObject>
#include <QDateTime>
class QPrinter;
class QTextDocument;
class MCajas;
class MMovimientosCaja;

class InformeCierreCaja : public QObject
{
    Q_OBJECT
public:
    explicit InformeCierreCaja(QObject *parent = 0);
    void hacerResumen( int id_caja = -1, bool ultimo = false, int id_cierre = -1 );

public slots:
    void imprimir( QPrinter *impresora );

private:
    QTextDocument *documento;
    QString _estilo;

};

#endif // INFORMECIERRECAJA_H
