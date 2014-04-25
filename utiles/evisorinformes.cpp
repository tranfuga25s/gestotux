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
#include "evisorinformes.h"
#include "eactcerrar.h"
#include "eactimprimir.h"

EVisorInformes::EVisorInformes( QPrinter *impre, QWidget *parent)
 : QPrintPreviewWidget( impre, parent)
{
 setAttribute( Qt::WA_DeleteOnClose );
 setWindowTitle( "Visor de Informe" );
 this->impresora = impre;
 // Inicializo el formualrio y creo las acciones
 QAction *ActSeparador = new QAction( this );
 ActSeparador->setSeparator( true );
 EActCerrar *ActCerrar = new EActCerrar( this );
 connect( ActCerrar, SIGNAL( triggered() ), this, SLOT( close() ) );
 addAction(ActCerrar);

 EActImprimir *ActImprimir = new EActImprimir( this );
 ActImprimir->setStatusTip( "Imprime el reporte actual" );
 connect( ActImprimir, SIGNAL( triggered() ), this, SLOT( imprimir() ) );
 addAction(ActImprimir);

 QAction *ActAPdf = new QAction( this );
 ActAPdf->setText( "Guardar a PDF" );
 ActAPdf->setIcon( QIcon( ":/imagenes/acroread.png" ) );
 ActAPdf->setStatusTip( "Guarda el informe en un archivo pdf en el disco" );
 connect( ActAPdf, SIGNAL( triggered() ), this, SLOT( aPdf() ) );
 addAction(ActAPdf);
 addAction(ActSeparador);

 QAction *ActPrimera = new QAction( this );
 ActPrimera->setIcon( QIcon( ":/imagenes/primera.png" ) );
 ActPrimera->setStatusTip( "Salta a la primer pagina" );
 connect( ActPrimera, SIGNAL( triggered() ), this, SLOT( primerPagina() ) );
 addAction( ActPrimera );

 QAction *ActAnterior = new QAction( "", this );
 ActAnterior->setIcon( QIcon( ":/imagenes/anterior.png" ) );
 ActAnterior->setStatusTip( "Salta a la pagina anterior" );
 connect( ActAnterior, SIGNAL( triggered() ), this, SLOT( anterior() ) );
 addAction( ActAnterior );

 QAction *ActSiguiente = new QAction( this );
 ActSiguiente->setIcon( QIcon( ":/imagenes/siguiente.png" ) );
 ActSiguiente->setStatusTip( "Salta a la pagina siguiente" );
 connect( ActSiguiente, SIGNAL( triggered() ), this, SLOT( siguiente() ) );
 addAction( ActSiguiente );

 QAction *ActUltima = new QAction( this );
 ActUltima->setIcon( QIcon( ":/imagenes/ultima.png" ) );
 ActUltima->setStatusTip( "Salta a la ultima hoja" );
 connect( ActUltima, SIGNAL( triggered() ), this, SLOT( ultimaPagina() ) );
 addAction( ActUltima );
 QAction *ActSeparador1 = new QAction( this );
 ActSeparador1->setSeparator( true );
 addAction( ActSeparador1 );

 QAction *ActAgrandar = new QAction( this );
 ActAgrandar->setIcon( QIcon( ":/imagenes/agrandar.png" ) );
 ActAgrandar->setStatusTip( "Aumenta el zoom" );
 connect( ActAgrandar, SIGNAL( triggered() ), this, SLOT( zoomIn() ) );
 addAction( ActAgrandar );

 QAction *ActAchicar = new QAction( this );
 ActAchicar->setIcon( QIcon( ":/imagenes/achicar.png" ) );
 ActAchicar->setStatusTip( "Disminuye el zoom" );
 connect( ActAchicar, SIGNAL( triggered() ), this, SLOT( zoomOut() ) );
 addAction( ActAchicar );

 QAction *ActSeparador2 = new QAction( this );
 ActSeparador2->setSeparator( true );
 addAction( ActSeparador2 );

 QAction *ActAjustarAncho = new QAction( this );
 ActAjustarAncho->setIcon( QIcon( ":/imagenes/ajustarancho.png" ) );
 ActAjustarAncho->setStatusTip( "Ajusta el zoom para que se vea todo el ancho de la hoja" );
 connect( ActAjustarAncho, SIGNAL(triggered()), this, SLOT( fitToWidth() ) );
 addAction( ActAjustarAncho );

 QAction *ActAjustarHoja = new QAction( this );
 ActAjustarHoja->setIcon( QIcon(":/imagenes/ajustarpagina.png") );
 ActAjustarHoja->setStatusTip( "Ajusta el zomm para que la hoja entre en la ventana" );
 connect( ActAjustarHoja, SIGNAL( triggered() ), this, SLOT( fitInView() ) );
 addAction( ActAjustarHoja );
 QAction *ActSeparador3 = new QAction( this );
 ActSeparador3->setSeparator( true );
 addAction( ActSeparador3 );

 QActionGroup *ActDisposicion = new QActionGroup( this );

 QAction *ActOpuestas = new QAction( this );
 ActOpuestas->setCheckable( true );
 ActOpuestas->setIcon( QIcon( ":/imagenes/ajustaropuestas.png" ) );
 ActOpuestas->setStatusTip( "Permite ver las paginas opuestas enfrentadas" );
 connect( ActOpuestas, SIGNAL( triggered() ), this, SLOT( setFacingPagesViewMode() ) );
 ActDisposicion->addAction( ActOpuestas );

 QAction *ActTodas = new QAction( this );
 ActTodas->setCheckable( true );
 ActTodas->setIcon( QIcon( ":/imagenes/todas.png" ) );
 ActTodas->setStatusTip( "Permite ver todas las hojas a imprimir" );
 connect( ActTodas, SIGNAL( triggered() ), this, SLOT( setAllPagesViewMode() ) );
 ActDisposicion->addAction( ActTodas );

 QAction *ActSola = new QAction( this );
 ActSola->setCheckable( true );
 ActSola->setIcon( QIcon( ":/imagenes/sola.png" ) );
 ActSola->setStatusTip( "Muestra solo una hoja en la vista" );
 connect( ActSola, SIGNAL( triggered() ), this, SLOT( setSinglePageViewMode() ) );
 ActDisposicion->addAction( ActSola );

 addActions( ActDisposicion->actions() );
 setObjectName( "visorInformes" );
}


EVisorInformes::~EVisorInformes()
{
}




/*!
    \fn EVisorInformes::primerPagina()
 */
void EVisorInformes::primerPagina()
{
 this->setCurrentPage( 1 );
}


/*!
    \fn EVisorInformes::anterior()
 */
void EVisorInformes::anterior()
{
 this->setCurrentPage( this->currentPage() - 1 );
}


/*!
    \fn EVisorInformes::ultimaPagina()
 */
void EVisorInformes::ultimaPagina()
{
 this->setCurrentPage( this->numPages() );
}


/*!
    \fn EVisorInformes::siguiente()
 */
void EVisorInformes::siguiente()
{
 this->setCurrentPage( this->currentPage() + 1 );
}

#include <QPrintDialog>
/*!
    \fn EVisorInformes::imprimir()
 */
void EVisorInformes::imprimir()
{
 if( impresora->outputFormat() != QPrinter::NativeFormat )
 {
  // no sabe usar esos tipos de dialogo, imprimo directamente
  qWarning( "No se puede usar el dialogo de configuracion, se imprimira directamente a la impresora predeterminada" );
  print();
 }
 else
 {
  QPrintDialog *dialogo = new QPrintDialog( impresora, this );
  if( dialogo->exec() == QDialog::Accepted )
  {
    emit paintRequested( dialogo->printer() );
  }
 }
 return;
}

#include <QFileDialog>
#include <QDir>
/*!
    \fn EVisorInformes::aPdf()
 */
void EVisorInformes::aPdf()
{
 this->impresora->setOutputFormat( QPrinter::PdfFormat );
 QString nombreArchivo = QFileDialog::getSaveFileName( this, "Guardar como pdf", QDir::homePath(), "Archivos PDF (*.pdf)" );
 if( !nombreArchivo.isEmpty() )
 {
  // ver que tenga la extension
  this->impresora->setOutputFileName( nombreArchivo );
  emit paintRequested( this->impresora );
 }
}
