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
#ifndef IRENDERIZADOR_H
#define IRENDERIZADOR_H

#include <QObject>
class QTextDocument;
class QProgressDialog;
class QTextTable;
class QTextCursor;
#include <QDate>
#include <QPrinter>

/**
	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class IRenderizador: public QObject
{
Q_OBJECT
public:
    IRenderizador( QObject *parent = 0);
    ~IRenderizador();
    void setEstablecimiento( QString nombre );
    void setEstablecimiento( int idDB );
    void hacerInforme();
    QString cargarEstilo();


private:
	QTextDocument *_doc;
	QProgressDialog *d;
	QTextTable *tabla;
	QDate _fecha;
	QString _estilo;
	QTextCursor *cursor;
	int _idEstablecimiento;

    QString cargarCabecera();
public slots:
    void imprimir( QPrinter *impresora );
};

#endif
