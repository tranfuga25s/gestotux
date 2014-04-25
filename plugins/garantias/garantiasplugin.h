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
#ifndef GARANTIASPLUGIN_H
#define GARANTIASPLUGIN_H

class QString;
class QStackedWidget;
class QMenuBar;
#include <QObject>
#include <QtPlugin>
#include <QList>
#include <QAction>
#include "../../src/eplugin.h"

class GarantiasPlugin: public QObject, EPlugin
{
 Q_OBJECT
 Q_INTERFACES(EPlugin)

public:
    QList<QActionGroup *> accionesBarra();
    bool inicializar();
    QWidgetList formsPreferencias();
    QString nombre() const;
    int tipo() const;
    void crearMenu( QMenuBar *m );
    bool verificarTablas( QStringList tablas );
    double version() const;
    void crearToolBar( QToolBar *t );
    QAction *botonPantallaInicial();
    void crearAccionesGlobales(QMainWindow *){}

private:
    QAction *ActGarantias;
    QAction *ActAgregarGarantia;
    QAction *ActVerVencimientos;

public slots:
    void seCierraGestotux();
    void verGarantias();
    void agregarGarantiaProducto( int id_comprobante, int id_producto, QString nombre_producto, int id_cliente );
    void agregarGarantiaEquipamiento( int id_comprobante, int id_equipamiento, QString nombre_producto, int id_cliente );
    void agregarGarantia();
    void verVencimientos();

signals:
    void agregarVentana( QWidget *v );
    void agregarDockWidget( Qt::DockWidgetArea area, QDockWidget *ventana );

};

#endif // GARANTIASPLUGIN_H
