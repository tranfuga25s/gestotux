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
#ifndef VVENTAS_H
#define VVENTAS_H

#include <QDate>
#include <evlista.h>

/**
 * \brief Listado de facturas emitidas
 *
 * Vista de la lista de facturas emitidas
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class VVentas : public EVLista
{
Q_OBJECT

public:
    VVentas( QWidget *parent = 0 );

protected slots:
    void agregar();
    void eliminar();
    void imprimir();
    void modificar();
    void anular();
    void pagar();
    void cambioVerAnuladas( bool parametro );
    void verItems();
    void actualizarListado();

signals:
    void emitirRecibo( int, QString, double );

private:
    void imprimirAnulacion( const int id_factura, const QString razon, const QString numero );

    QAction *ActVerItems;

};

#endif
