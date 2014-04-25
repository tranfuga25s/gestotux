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
#ifndef EMOVIMIENTO_H
#define EMOVIMIENTO_H

#ifdef GESTOTUX_CARAVANAS_TIENEN_DUENOS
#warning "Utilizando duenos"
#endif

#include <QObject>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QDate>
class QProgressDialog;
#include "TipoMovs.h"

/**
 * \brief Encapsulacion de un movimiento
 *
 * Clase que carga, administra y guarda los movimientos y todas sus relaciones
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class EMovimiento : public QObject
{
Q_OBJECT
public:
    EMovimiento( QObject *parent = 0 );
    ~EMovimiento();

    bool cargarMovimiento( int idDb );
    int guardar( QProgressDialog *dialogo );

    QStringList caravanas();
    QString getDTA() const;
    QDate getFecha() const;
    int tipoMov() const;

    void setTipoMov( const int& theValue );
    bool setDTA( const QString& theValue,  bool nuevo = true);
    void setComprador( int id );
    void setVendedor( int idDB );
    void setEstablecimientoDestino( int idDB );
    void setEstablecimientoOrigen( int idDB );
    void setCategoria( int id );
    void setFecha( const QDate& theValue );
    void eliminarCaravana( QString codigo );
    bool agregarCaravana( QString codigo, bool verificar = true );
    void setNumGuia( const QString numeroGuia );
    int getTri() const;
    QPair< int, QString> getVendedor() const;
    QPair< int, QString> getComprador() const;
    QPair< int, QString> getEstablecimientoOrigen() const;
    QPair< int, QString> getEstablecimientoDestino() const;
    QString getNumeroGuia() const;
    QPair< int, QString > getCategoria() const;
    void setCantidadAnimales ( int theValue );
    int getCantidadAnimales() const;
    bool setComprador( QString nombre );
    void setCaravanas( QStringList caravanas );
    bool eliminarTRI( const int idDB );

private:
   /*!
    * Datos del establecimiento de destino
    */
	QPair<int,QString> destino;
   /*!
    * Datos del establecimiento de origen
    */
	QPair<int,QString> origen;
   /*!
    * Datos del vendedor
    */
	QPair<int,QString> vendedor;
   /*!
    * Datos del comprador
    */
	QPair<int,QString> comprador;
   /*!
    * Datos de la categoria
    */
	QPair<int,QString> categoria;
   /*!
    * Fecha en que se realizo el TRI
    */
	QDate fecha;
   /*!
    * NUmero de DTA correspondiente al TRI cargado
    */
	QString DTA;
   /*!
    * Numero de Guia correspondiente al TRI cargado
    */
	QString _numGuia;
   /*!
    * Cantidad de animales para un movimiento de categoria especial
    */
	int _cantidad_animales;
   /*!
    * Indentificador de indice en la base de datos
    */
	int id_db;
   /*!
    * Lista de caravanas cargas o actualizadas para guardar
    */
	QStringList _caravanas;

private:
    void cargarNombreCategoria( const int idDB );
    void cargarNombreComprador( const int idDB );
    void cargarNombreVendedor( const int idDB );
    void cargarNombreEstablecimientoDestino( const int idDB );
    void cargarNombreEstablecimientoOrigen( const int idDB );
    bool guardarCaravana( QString codigo );
    bool cargarCaravanas();
    bool asociarCaravana( int id_caravana );
    bool aduenarCaravana( int id_caravana, int id_cliente, QDate fecha, bool cambiar_dueno = true );
    int getIDCaravana( QString codigo );
    bool borrarCaravana( QString codigo, bool disasociar = true );
    bool deshacerCambiosDb();
    bool iniciarTransaccion();
    bool hacerCommit();

   /*!
    * Indicador de tipo de movimiento que representa el objeto
    */
    int tipo_mov;

};

#endif
