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

#ifndef FORMASOCIARSERVICIOCLIENTE_H
#define FORMASOCIARSERVICIOCLIENTE_H

#include <QDialog>
#include "ui_FormAsociarBase.h"
#include <QDate>

/*!
 * \brief Formulario para asociar un cliente con un servicio
 *
 *
 * \author Esteban Zeller
 */
class FormAsociarServicioCliente : public QDialog, private Ui::Dialog
{
  Q_OBJECT

public:
   /*!
    * \enum Tipo de Forma del dialogo
    * Muestra las formas en que se puede mostrar el dialogo ante el usuario
    */
    enum tipoForm {
         Todo, /*! Muestra todos los campos */
         Cliente, /*! Muestra la lista de clientes para asociar a un servicio */
         Servicio, /*! Muestra el servicio al que se debe asociar un cliente */
         SoloFecha /*! Muestra solo la fecha de asociacion */
    }_tipo;

    FormAsociarServicioCliente( QWidget* parent = 0, tipoForm tipo = Todo, Qt::WFlags fl = 0 );
    int idCliente();
    int idServicio();
    QDate fecha();
    void setFecha( QDate fecha );
    void setIdCliente( int id_cliente );
    void setIdServicio( int id_servicio );

public slots:
    int exec();

private slots:
    void reject();
    void accept();

signals:
    void editarCliente( int );

private:
   /*!
    * Guarda el identificador del cliente
    */
    int _id_cliente;
   /*!
    * Guarda el identificador del servicio
    */
    int _id_servicio;
   /*!
    * Guarda la fecha de asociacion
    */
    QDate _fecha;
   /*!
    * Determina si se esta agregando una nueva asociación o modificando una existente
    */
    bool _agregado;

};

#endif

