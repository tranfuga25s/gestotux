/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller   *
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
#ifndef EENVIOBACKUP_H
#define EENVIOBACKUP_H

#include <QThread>
class QFtp;
class QFile;

/**
        \brief Clase de envio de backup al servidor

        Clase que ejecutandose como un hilo independiente del sistema grafico, realiza el envio del backup
        al servidor central, obteniendo informacion del plugin de informacion.
        Utiliza un servicio ftp que puede ser configurado desde el plugin o desde las preferencias. Por ahora el
        sistema utiliza un ftp con login anonimo.

        @author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class EEnvioBackup : public QThread
{
Q_OBJECT
public:
    EEnvioBackup(QObject *parent = 0);
    ~EEnvioBackup();
    virtual void run();

private slots:
    void finComando( int id, bool error );

private:
   /*!
    * Clase que realiza las comunicacion ftp con el servidor
    */
    QFtp *ftp;
   /*!
    * Punto del archivo de backup
    */
    QFile *archivo;
};

#endif
