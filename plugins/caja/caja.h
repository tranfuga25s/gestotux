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

#ifndef CAJA_H
#define CAJA_H

#include <QObject>
#include <eplugin.h>
#include <QtPlugin>

/**
 * \brief Plugin de Caja
 *
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class Caja : public QObject, public EPlugin
{
    Q_OBJECT
    Q_INTERFACES(EPlugin)

    public:
        QList<QActionGroup *> accionesBarra();
        QString nombre() const;
        QWidgetList formsPreferencias();
        bool inicializar();
        bool verificarTablas( QStringList tablas );
        int tipo() const;
        void crearMenu( QMenuBar *m );
        double version() const;
        static QStackedWidget *tabs();
        void crearToolBar( QToolBar *t );
        bool publicidad() { return true; }
        QAction *botonPantallaInicial() { return ActCajas; }
        void crearAccionesGlobales(QMainWindow *){}

    public slots:
        void seCierraGestotux();

    signals:
        void agregarVentana( QWidget * );
        void agregarDockWidget(Qt::DockWidgetArea area, QDockWidget *ventana);

    private:
        QAction *ActCajas;
        QAction *ActAgregarMovimiento;
        QAction *ActVerEstado;
        QAction *ActHacerCierre;
        QAction *ActResumenes;

    protected slots:
        void verCajas();
        void verResumenCaja();
        void verEstadoCaja();
        void hacerCierre();

};

#endif // CAJA_H
