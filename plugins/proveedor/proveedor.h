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
#ifndef PRESUPUESTO_H
#define PRESUPUESTO_H

#include <QObject>
#include "../../src/eplugin.h"
#include <QtPlugin>

/**
 * \brief Plugin de Proveedores
 *
 * Clase que sirve de implementación para el plugin de proveedores, dando el sistema de carga y acciones para menus y barras
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class proveedor : public QObject, public EPlugin
{
Q_OBJECT
Q_INTERFACES( EPlugin )
public:
    bool inicializar();
    bool verificarTablas( QStringList tablas );
    double version() const;
    int tipo() const;
    QList< QActionGroup * > accionesBarra();
    QString nombre() const;
    QWidgetList formsPreferencias();
    void crearMenu(QMenuBar* m);
    void crearToolBar(QToolBar* t);
    void crearAccionesGlobales(QMainWindow *){}
    QAction *botonPantallaInicial();

signals:
    void agregarVentana( QWidget *v );
    void agregarDockWidget(Qt::DockWidgetArea area, QDockWidget *ventana);

public slots:
    void proveedores();
    void seCierraGestotux();

private:
    QAction *ActProveedores;

};

#endif
