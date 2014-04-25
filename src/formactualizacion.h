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

#ifndef FORMACTUALIZACION_H
#define FORMACTUALIZACION_H

#include "eventana.h"
#include "ui_FormActualizacionBase.h"

#include <QHash>
#include <QPair>
class QFtp;

/**
 * \brief Actualizaciones del programa
 *
 * Clase que muestra el formulario de actualizaciones y maneja la descarga de indice y archivos.
 * @author Esteban Zeller
 */
class FormActualizacion : public EVentana, private Ui::FormActualizacionBase
{
  Q_OBJECT

public:
  FormActualizacion(QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~FormActualizacion();

protected slots:
    void iniciar();
    void detener();
    void cambioEstado( int estado );
    void inicio( int id );
    void terminado( int comando, bool  error );
    void transferencia( qint64 echo, qint64 total );


private:
       /*!
	* Variable utilizada para cortar el analisis del archivo indice si se presiona detener
	*/
	bool _continuar_actualizando;
       /*!
	* Puntero al servicio ftp
	*/
	QFtp *ftp;
       /*!
	* Hash de archivos y destinos para la copia de archivos al terminar de descargarse
	*/
	QHash<int, QPair<QString,QString> > _arch_dest;

	QAction *ActDetener;
	QAction *ActIniciar;
	QAction *ActCerrar;

public slots:
    void finComando( int comando, bool error );

private slots:
    void analizarGeneral();
};

#endif

