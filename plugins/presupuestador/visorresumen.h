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
#ifndef VISORRESUMEN_H
#define VISORRESUMEN_H

#include <QTextEdit>
#include <QString>
#include <QDate>
#include <QTextCharFormat>
#include <QTextBlockFormat>
class QTextCursor;

/**
	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class visorResumen : public QTextEdit
{
Q_OBJECT
public:
    visorResumen(QWidget *parent = 0);
    ~visorResumen();
    void renderizarCabecera();
    void renderizarDetalle();
    void renderizarDestinatario();
    QString nombre();
    void set_registro( int id );
    void cerrate();

private:
    QString contenido, destinatario;
    double total;
    QDate fecha;

    QTextCursor *cursor;
    QTextCharFormat centrado, f;
    QTextBlockFormat fb;
protected slots:
    void imprimir();
    void aPdf();

signals:
	void agregarVentana( QWidget * );
};

#endif
