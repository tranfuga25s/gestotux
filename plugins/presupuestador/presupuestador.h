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
#ifndef PRESUPUESTADOR_H
#define PRESUPUESTADOR_H

#include "einfoprogramainterface.h"
#include "eplugin.h"
#include <QObject>


class presupuestador : public QObject, EInfoProgramaInterface, EPlugin
{
 Q_OBJECT
 Q_INTERFACES(EInfoProgramaInterface EPlugin)

public:
    QString nombrePrograma() const;
    QIcon iconoPrograma()  const;
    bool inicializar();
    QString directorioBackup() const;
    QString directorioActualizaciones() const;
    QWidgetList formsPreferencias();
    QString nombre() const;
    int tipo() const;
    void crearMenu( QMenuBar *m );
    bool verificarTablas();
    double version() const;
    QString empresa() const;
    QString companeros();
    bool publicidad();
    QList<QActionGroup *> accionesBarra();
    void crearToolBar( QToolBar *t );

private:

    QAction *ActPresuAnteriores;
    QAction *ActNuevoPresu;

    QAction *ActAutos;

protected slots:
    void nuevoPresupuesto();

protected slots:
    void verAnteriores();
    void verAutos();

signals:
	void agregarVentana( QWidget * );

};
#endif
