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
#ifndef ESPLASH_H
#define ESPLASH_H

#include <QSplashScreen>
#include <QColor>
#include <QString>
#include <QPixmap>

/**
 * \brief Ventana de bienvenida a la aplicacion
 *
 * Ventana que se muestra cuando se esta iniciando la aplicacion. intenta cargar el archivo splash.sgv del directorio donde se encuentra el ejecutable y si no lo encuentra utiliza el archivo de splash que tiene incluido en el ejecutable
 *
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class ESplash : public QSplashScreen
{
public:
    ESplash(QWidget *parent = 0);
    void showMessage(const QString &message, int alignment = Qt::AlignLeft, const QColor &color = Qt::black );


protected:
    void drawContents(QPainter *painter);
    /*void paintEvent(QPaintEvent *);*/

    QString texto;
    int alineacion;
    QColor color;

};

#endif
