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
#ifndef REMITOPLUGIN_H
#define REMITOPLUGIN_H

#include <QDate>
#include <QObject>
#include <eplugin.h>
#include <QDate>
#include "mproductostotales.h"
#include "../cuotas/mplancuota.h"

/**
 * \brief Plugin de remitos
 *
 * Plugin para la implementación de remitos
 *
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class RemitoPlugin : public QObject, public EPlugin
{
Q_OBJECT
Q_INTERFACES( EPlugin )

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
    QAction *botonPantallaInicial() { return ActAgregarRemito; }
    void crearAccionesGlobales(QMainWindow *){}

signals:
    void agregarVentana( QWidget * );
    void agregarDockWidget(Qt::DockWidgetArea area, QDockWidget *ventana);
    void emitirRecibo( int, QDate, QString, double );
    void emitirPlanCuota( int, double, MPlanCuota::TipoComprobante );
    void planCuotaSetearIdCuota( int );
    void emitirPlanCuotaSetIdFactura( int, int );
    void emitirGarantia( int, int, QString, int );

public slots:
    void seCierraGestotux();
    void agregarRemito( int, QDate, MProductosTotales * );
    void mostrarRemito( int );

protected slots:
    void verRemitos();
    void agregarNuevoRemito();

private:
    QAction *ActAgregarRemito;
    QAction *ActVerRemitos;
};

#endif
