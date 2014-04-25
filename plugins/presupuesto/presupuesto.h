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
#ifndef PRESPUESTO_H
#define PRESPUESTO_H

#include <QtPlugin>
#include <QObject>
#include "eplugin.h"
#include <QWidgetList>
#include <QList>
#include <QDate>
#include "mproductostotales.h"
class QAction;
class QSettings;
class QStackedWidget;
class QMenuBar;

/**
 *  \brief Clase del plugin de presupuestos
 *
 *  Plugin para hacer presupuestos
 *
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class presupuesto : public QObject, public EPlugin
{
Q_OBJECT
Q_INTERFACES(EPlugin)

public:
    QList<QActionGroup *> accionesBarra();
    QString nombre() const;
    QWidgetList formsPreferencias();
    bool inicializar();
    int tipo() const;
    void crearMenu( QMenuBar *m );
    double version() const;
    bool verificarTablas( QStringList tablas );
    void crearToolBar( QToolBar *t );
    QAction *botonPantallaInicial() { return ActNuevoPresu; }
    void crearAccionesGlobales(QMainWindow *){}

private:
    QList<QAction *> _acciones;
    QAction *ActNuevoPresu;
    QAction *ActPresuAnteriores;

protected slots:
    void nuevoPresupuesto();
    void verAnteriores();

signals:
    void agregarVentana( QWidget * );
    void agregarDockWidget( Qt::DockWidgetArea, QDockWidget * );
    void emitirFactura( int, QDate, MProductosTotales * );

public slots:
    void seCierraGestotux();

};

#endif
