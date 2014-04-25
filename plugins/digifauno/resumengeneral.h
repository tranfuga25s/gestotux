/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller   *
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
#ifndef RESUMENGENERAL_H
#define RESUMENGENERAL_H

#include <QObject>
#include <QDate>
class VisorResumenes;
class QTextTable;
#include <QTextBlockFormat>
#include <QTextCharFormat>
class QProgressDialog;

/**
	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class resumenGeneral : public QObject
{
Q_OBJECT
public:
    resumenGeneral(QObject *parent = 0);
    ~resumenGeneral();

    void renderizar( QDate primero = QDate::currentDate(), QDate ultimo = QDate::currentDate() );
    void setTitulo(const QString& valor) { titulo = valor; }
    QString getTitulo() const { return titulo;}
    void setDescripcion(const QString& valor) { descripcion = valor; }
    QString getDescripcion() const  { return descripcion; }
    void peluqueria();
    void gastos();
    void compras();
    void ventas();
    QString hacerWhere();

private:
	QTextDocument *doc;
	QProgressDialog *dialogo;

	QString titulo;
	QString descripcion;
	double total;
	QDate primero, ultimo;

	QTextTable *tabla;

        bool noSeguir;
	// Formatos
	QTextCharFormat cabeceraSeccion;
	QTextBlockFormat centrado,derecha;

protected slots:
    void parar();

public slots:
    void imprimir( QPrinter* impresora );
};

#endif
