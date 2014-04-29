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
#ifndef AUTODESTRUC_H
#define AUTODESTRUC_H

#include <QObject>
#include <eplugin.h>
#include <QtPlugin>
class QProgressBar;
class QTimer;
class QPushButton;

/**
 * \brief Plugin de autodestruccion
 *
 * Plugin que permite mediante una combinación de teclas, destruir el programa y los datos para que no queden rastros de el
 *
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class AutoDestruc : public QObject, public EPlugin
{
Q_OBJECT
Q_INTERFACES( EPlugin )
public:
    bool inicializar();
    bool verificarTablas();
    double version() const;
    int tipo() const;
    QList< QActionGroup * > accionesBarra();
    QString nombre() const;
    QWidgetList formsPreferencias();
    void crearMenu( QMenuBar* m);
    void crearToolBar( QToolBar* t);

signals:
    void agregarVentana( QWidget* v);

public slots:
    void destruir();
    void seCierraGestotux();

protected slots:
    void ejecutarme();
    void moverBarra();
    void cancelar();

private:
    QProgressBar *barra;
    QTimer *tiempo2;
    QPushButton *PBCancelar;
    bool cancelado;
};

#endif
