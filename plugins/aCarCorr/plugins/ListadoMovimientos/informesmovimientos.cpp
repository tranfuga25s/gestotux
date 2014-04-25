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
#include "informesmovimientos.h"

#include <QAction>
#include <QMenu>
#include "formfiltro.h"
#include "erenderizadorinforme.h"
#include "einformeimpresora.h"

#include "evisorinformes.h"

QString InformesMovimientos::nombre() const
{
 return "informesMovimientos";
}

bool InformesMovimientos::inicializar()
{

 ActInformeFiltroTotal = new QAction( "Personalizado...", this );
 connect( ActInformeFiltroTotal, SIGNAL( triggered() ), this, SLOT( informeCompleto() ) );

 cargarArchivoEstilo();

 return true;
}

double InformesMovimientos::version() const
{
 return 0.1;
}

void InformesMovimientos::crearMenu( QMenu* m )
{
 m->addAction( ActInformeFiltroTotal );
}

Q_EXPORT_PLUGIN2(movimientos, InformesMovimientos );


/*!
    \fn InformesMovimientos::informeCompleto()
	Genera el fomulario de filtro y genera el visor de informes y el informe
 */
void InformesMovimientos::informeCompleto()
{
 FormFiltro *f = new FormFiltro();
 if( f->exec() == QDialog::Accepted )
 {
	// Genero un nuevo informe
	EVisorInformes *fa = new EVisorInformes( new EInformeImpresora() );
	// Genero los contenidos del informe
	ERenderizadorInforme *render = new ERenderizadorInforme( this );
	render->setPropiedades( f, cargarArchivoEstilo(), cargarCabecera() );
	// lo muestro
	if( render->hacerInforme() )
	{
		connect( fa, SIGNAL( paintRequested ( QPrinter * ) ), render, SLOT( imprimir( QPrinter * ) ) );
		emit agregarVentana( fa );
		render->cerrarDialogo();
	}

 }
 delete f;
}


/*!
    \fn InformesMovimientos::cargarArchivoEstilo()
	Carga la hoja CSS de estilo. Primero busca en la carpeta plugins/informes/estilo.css y si no lo encuentra utiliza la hoja de estilo embebida
 */
QString InformesMovimientos::cargarArchivoEstilo()
{
 QString nombre, _estilo;
 if( QFile::exists( QApplication::applicationDirPath() + "/plugins/informes/estilo.css" ) )
 {
  nombre = QApplication::applicationDirPath() + "/plugins/informes/estilo.css";
 }
 else
 {
  qDebug( QString( "archivo %1 no encontrado" ).arg( QApplication::applicationDirPath() + "/plugins/informes/estilo.css" ).toLocal8Bit() );
  nombre = ":/informes/estiloinforme.css";
 }
 archivoEstilo = new QFile( nombre );
 if( !archivoEstilo->open( QIODevice::ReadOnly ) )
 {
  qDebug( QString( "Error al abrir el archivo de estilo: %1" ).arg( nombre ).toLocal8Bit() );
  return "";
 }
 _estilo = archivoEstilo->readAll();
 archivoEstilo->close();
 delete( archivoEstilo );
 archivoEstilo = 0;
 return _estilo;
}


/*!
    \fn InformesMovimientos::cargarCabecera()
 */
QString  InformesMovimientos::cargarCabecera()
{
 QString _cabecera;
 if( QFile::exists( QApplication::applicationDirPath() + "/plugins/informes/cabecera.html" ) )
 {
  archivoEstilo = new QFile( QApplication::applicationDirPath() + "/plugins/informes/cabecera.html" );
  if( !archivoEstilo->open( QIODevice::ReadOnly ) )
  {
    qDebug( QString( "Error al abrir el archivo de estilo: %1" ).arg( archivoEstilo->fileName() ).toLocal8Bit() );
    return "";
  }
  _cabecera = archivoEstilo->readAll();
  archivoEstilo->close();
  delete( archivoEstilo );
  archivoEstilo = 0;
  return _cabecera;
 }
 else
 { return ""; }
}


/*!
    \fn InformesMovimientos::accionesBarra()
 */
QList<QAction *>  InformesMovimientos::accionesBarra()
{
 QList<QAction *> lista;
 lista.append( ActInformeFiltroTotal );
 return lista;
}
