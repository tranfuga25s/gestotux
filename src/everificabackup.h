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
#ifndef EVERIFICABACKUP_H
#define EVERIFICABACKUP_H

#include <QPushButton>

/**
 * \brief Verifica la frecuencia de backups
 *
 * Verifica el ultimo envio del backup y lanza una advertencia si es muy viejo
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class EVerificaBackup : public QPushButton
{
Q_OBJECT
public:
    EVerificaBackup(QWidget *parent = 0);

protected slots:
    void verificar();

private:
       /*!
        * Timer para hacer el control del backup
        */
        QTimer *tiempo;

signals:
        void abrirBackups();

};

#endif
