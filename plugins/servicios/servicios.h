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
#ifndef SERVICIOS_H
#define SERVICIOS_H

#include "eplugin.h"
#include <QtPlugin>
#include <QAction>
#include <QObject>

/**
 * \brief Clase base del plugin
 *
 * Clase base del plugin de servicios
 *
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class Servicios : public QObject, public EPlugin
{
Q_OBJECT
Q_INTERFACES( EPlugin )
public:
    bool inicializar();
    bool verificarTablas( QStringList tablas );
    double version() const;
    int tipo() const;
    QList<QActionGroup *> accionesBarra();
    QString nombre() const;
    QWidgetList formsPreferencias();
    void crearMenu( QMenuBar* m );
    void crearToolBar( QToolBar* t );
    QAction *botonPantallaInicial();
    void crearAccionesGlobales(QMainWindow *){}

signals:
    void agregarVentana( QWidget* v );
    void agregarDockWidget( Qt::DockWidgetArea area, QDockWidget *ventana );
    void editarCliente( int );

public slots:
    void seCierraGestotux();
    void verificarRecargos();

private:
       /*!
        * Accion para mostrar los distintos servicios
        */
        QAction *ActServicios;
       /*!
        * Accion para mostrar los distintos recargos posibles a servicios
        */
        QAction *ActRecargos;
       /*!
        * Accion para realizar una facturación de un servicio
        */
        QAction *ActFacturarServicios;
       /*!
        * Accion para verificar los recargos de un servicio
        */
        QAction *ActVerfRecargos;

protected slots:
    void mostrarServicios();
    void mostrarRecargos();
    void facturarServicios();

};

#endif
