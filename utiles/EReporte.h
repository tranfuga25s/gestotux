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

#ifndef EREPORTE_H
#define EREPORTE_H

#include "openreports.h"
#include "common/parameter.h"
#include "eplugin.h"
#include "einfoprogramainterface.h"

/*!
 * \brief Clase Wrapper para generación de reportes.
 *
 * Envoltorio para generar reportes y cargar los datos predeterminados de los tipos de reportes especificados.
 * \author Esteban Zeller tranfuga_25s@gmail.com
 */
class EReporte : public QObject {

    Q_OBJECT

  public:

     /*! \enum Tipo
      *  Define el reporte que se cargará en la funcion hacer.
      *  Cada tipo posee un nombre predeterminado o puede especificado por el plugin del cliente.
      *  Si se desea utilizar un reporte no incluido en la lista, utilice el @Especial.
      */
    enum Tipo {
        Invalido = -1, /** Tipo invalido, cuando esta recien creado el objeto y no se cargo la definicion, o hubo un error cargando el reporte */
        Presupuesto = 1, /** Tipo Presupuesto */
        Factura = 2, /** Tipo Factura */
        Recibo = 3, /** Tipo Recibo */
        AnulacionFactura = 4, /** Tipo Anulacion de una factura */
        Especial = 5, /** Tipo Especial, se deberá incluir el nombre del reporte en un parametro adicional */
        AnulacionRecibo = 6, /** Tipo Anulacion de recibo */
        Remito = 7 /** Tipo Remito ( Factura en negro ) */
    };
    explicit EReporte( QObject *padre, QString nombre_reporte, ParameterList parametros );
    explicit EReporte( QObject *padre );
    ~EReporte();

    void presupuesto();
    void factura();
    void recibo();
    void remito();
    void anulacionFactura();
    void anulacionRecibo();
    bool especial( const QString nombre, ParameterList parametros );

    bool hacer( ParameterList parametros, bool previsualizar = false, bool mostrarDialogoImpresora = true );
    bool hacer() { return hacer( _parametros, false, true ); }

    bool hacerPDF( ParameterList parametros, QString ruta = QString() );
    bool hacerPDF() { return hacerPDF( _parametros ); }

    void setearImpresora( QPrinter *imp );
    void mostrarError( QWidget *ventana );

    void setearParametros( ParameterList param );

    public slots:
    void imprimir( QPrinter *p );

    private:
       /*!
        * Tipo de reporte cargado actualmente
        */
        Tipo _tipo;
       /*!
        * Puntero interno al sistema que genera los reportes
        */
        orReport *_rep;
       /*!
        * Nombre del reporte a cargar o cargado.
        */
        QString _nombre;
       /*!
        * Lista de parametros actual
        */
        ParameterList _parametros;
       /*!
        * Impresora a utilizar
        */
        QPrinter *_impresora;

        bool cargar( const QString nombre );
};

#endif // EREPORTE_H
