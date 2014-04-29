/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller & Daniel Sequeira		   *
 *   juiraze@yahoo.com.ar  - daniels@hotmail.com			   *
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
#ifndef VDUENOS_H
#define VDUENOS_H

#include <evlista.h>

class QComboBox;
class QCheckBox;
class QFrame;

/**
Ve la lista de los dueños

	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class VDuenos : public EVLista
{
 Q_OBJECT
public:
    VDuenos( QWidget *parent = 0 );
    ~VDuenos();

private slots:
   void antes_de_insertar( int row, QSqlRecord & record );
    void eliminar();

private:
    QCheckBox *ChBAscendente;
    QComboBox *CBOrden;
    QFrame *forden;
    QAction *ActFiltrar;

protected slots:
    void cambioOrdenSentido( int estado );
    void cambioOrden( const QString &columna );
    void cambioFiltro( bool estado );
    void menuContextual( const QModelIndex &indice, QMenu *menu );
};

#endif
