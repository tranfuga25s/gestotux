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
#ifndef GESTOTUX_H
#define GESTOTUX_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QDir>
#include <QPluginLoader>
#include <QHash>

class QAction;
class QMenu;
class barraAbajo;
class barraLateral;
class QToolBar;
class EInfoProgramaInterface;
class EPlugin;
class FormularioCentral;

/**
 * \brief Ventana principal del programa
 *
 * Clase que se encarga de generar la ventana principal del programa, cargar los plugins, crear entonrno grafico, menus, barras de estado, verificador de backups y demas.
 * @author Esteban Zeller <tranfuga25s@gmail.com>
 */
class gestotux : public QMainWindow
{
      Q_OBJECT
public:
      gestotux();
      ~gestotux();
      void inicializar();
      static FormularioCentral *formCentral;
      static FormularioCentral *formCen();
      static QToolBar *barraAcciones();
      static QToolBar *_barraAcciones;

protected:
      void closeEvent(QCloseEvent *event);
      void keyPressEvent( QKeyEvent *event );

public slots:
      void verClientes();
      void verPreferencias();
      void verBackup();
      void agregarDock( Qt::DockWidgetArea area, QDockWidget *ventana );
      void editarCliente( int id_cliente );

protected slots:
      void salir();
      void acerca();
      void verActualizacion();
      void ayuda();
      void resumenGeneral();

private slots:
      void ocultar_mostrar( QSystemTrayIcon::ActivationReason razon = QSystemTrayIcon::Trigger );

signals:
      void saliendoGestotux();

private:
      void createActions();
      void createMenus();
      void createStatusBar();
      void createToolBar();
      void crearReloj();
      void crearPantallaInicio();
      void bandeja_sistema();
      void crearBarraLateral();

      barraLateral *barra;
      barraAbajo *barraA;

      QToolBar *tb;

      QMenu *fileMenu;
      QMenu *menuHer;
      QMenu *menuVer;
      QMenu *menuAyuda;

      QAction *ActClientes;
      QAction *exitAct;
      QAction *acercade;
      QAction *ActCerrar;
      QAction *ActPreferencias;
      QAction *ActBackup;
      QAction *ActActualizar;
      QAction *ActAyuda;
      QAction *ActRestaurar;
      QAction *ActResumenGeneral;

      QSystemTrayIcon * iconoBandeja;

};

#endif
