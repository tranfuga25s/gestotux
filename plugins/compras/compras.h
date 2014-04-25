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
#ifndef COMPRAS_H
#define COMPRAS_H

#include <QObject>
#include <eplugin.h>

/**
 * \brief Plugin de compras
 *
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class Compras : public QObject, public EPlugin
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
    void crearMenu(QMenuBar* m);
    void crearToolBar(QToolBar* t);
    QAction *botonPantallaInicial() { return ActCompras; }
    void crearAccionesGlobales(QMainWindow *){}

signals:
    void agregarVentana(QWidget* v);
    void agregarDockWidget(Qt::DockWidgetArea area, QDockWidget *ventana);

private:
        QAction *ActAgregarCompra;
        QAction *ActCompras;

public slots:
    void agregarCompra();
    void ver_compras();
    void seCierraGestotux();

};

#endif
