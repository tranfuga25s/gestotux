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
#ifndef FORMPREFERENCIAS_H
#define FORMPREFERENCIAS_H

#include "eventana.h"

class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QSplitter;
class QPushButton;

/**
 * @brief Formulario base para todas ventanas de preferencias
 * Genera el formulario donde se colocan todas las preferencias de la aplicación
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class FormPreferencias : public EVentana
{
Q_OBJECT
public:
    FormPreferencias(QWidget *parent = 0);
    ~FormPreferencias();

protected slots:
    void changePage( QListWidgetItem *current, QListWidgetItem *previous );
    void cancelar();

private:
    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;
    QSplitter *Splitter;
    QAction *ActAplicar;
    QAction *ActCerrar;
    QAction *ActGuardar;

signals:
        void guardar();
        void aplicar();
        void cargar();
        void cerrar();

};

#endif
