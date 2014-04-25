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

#ifndef EAYUDA_H
#define EAYUDA_H

#include <QWidget>
#include <QtHelp>
#include "ui_EAyudaBase.h"

/*!
 * \brief Clase que mantiene la ayuda del programa
 *
 * Clase que hace de nexo entre el formulario que pide la ayuda y la ayuda que se encuentra disponible.
 * Busca en el archivo de documentacion comprimido (QtHelp4) el contenido de la ayuda y lo muestra en un
 * form de ayuda.
 * @author Esteban Zeller <tranfuga25s@gmail.com>
 */
class EAyuda : public QWidget, private Ui::EAyudaBase
{
  Q_OBJECT

public:
    bool hayAyuda( QString nombreObjeto );
    void mostrarAyuda( QString nombreObjecto );
    void mostrarIndice();
    static EAyuda * instancia();

private:
  EAyuda(QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~EAyuda();
  QHelpEngine *engine;
  static EAyuda *yo;

private slots:
  void errorEngine( const QString &mensaje );
  void inicioConstruccion();
  void finConstruccion();
};

#endif

