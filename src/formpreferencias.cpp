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
#include "formpreferencias.h"
#include "eplugin.h"

#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QAction>
#include <QSplitter>

#include "preferencias.h"
#include "formprefhijo.h"
#include "fprefgeneral.h"
#include "formprefdb.h"
#include "eregistroplugins.h"
#include "einfoprogramainterface.h"
#include "formprefclientes.h"

FormPreferencias::FormPreferencias(QWidget *parent)
 : EVentana(parent)
{
    setObjectName( "Preferencias" );
    setWindowIcon( QIcon( ":/imagenes/configure.png" ) );
    this->setAttribute( Qt::WA_DeleteOnClose );
    contentsWidget = new QListWidget( this );
    contentsWidget->setViewMode( QListView::IconMode );
    contentsWidget->setMovement( QListView::Static );
    contentsWidget->setMinimumWidth( 128 );
    contentsWidget->setMaximumWidth( 128 );
    contentsWidget->setSpacing( 12 );
    contentsWidget->sizePolicy().setVerticalPolicy( QSizePolicy::MinimumExpanding );
    contentsWidget->setResizeMode( QListView::Adjust );
    contentsWidget->setGridSize( QSize( 128, 64 ) );

    pagesWidget = new QStackedWidget;
    QListWidgetItem *configButton = new QListWidgetItem( contentsWidget );
    configButton->setIcon( QIcon( ":/imagenes/general.png" ) );
    configButton->setText( "General" );
    configButton->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    configButton->setTextAlignment( Qt::AlignBottom | Qt::AlignHCenter );
    configButton->setSizeHint( QSize( 128, 128 ) );
     ////////////////////////////////////////////////////////////////////
     /// Agregar aqui los widgets de configuracion
     FPrefGeneral *formGeneral = new FPrefGeneral( this );
     connect( this, SIGNAL( aplicar() ), formGeneral, SLOT( aplicar() ) );
     connect( this, SIGNAL( cargar() ), formGeneral, SLOT( cargar() ) );
     connect( this, SIGNAL( guardar() ), formGeneral, SLOT( guardar() ) );
     pagesWidget->addWidget( formGeneral );
     preferencias *p = preferencias::getInstancia();
     p->inicio();
     p->beginGroup( "Preferencias" );
     p->beginGroup( "General" );
     if( p->value( "mostrardb", true ).toBool() )
     {
        QListWidgetItem *db = new QListWidgetItem( contentsWidget );
        db->setIcon( QIcon( ":/imagenes/dbconfig.png" ) );
        db->setText( "Base de datos" );
        db->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        db->setTextAlignment( Qt::AlignBottom | Qt::AlignHCenter );
        FormPrefDb *formDb = new FormPrefDb( this );
        connect( this, SIGNAL( aplicar() ), formDb, SLOT( aplicar() ) );
        connect( this, SIGNAL( cargar() ), formDb, SLOT( cargar() ) );
        connect( this, SIGNAL( guardar() ), formDb, SLOT( guardar() ) );
        pagesWidget->addWidget( formDb );
     }
     p->endGroup();
     p->endGroup();
     // Preferencias de clientes
     QListWidgetItem *clientes = new QListWidgetItem( contentsWidget );
     clientes->setIcon( QIcon( ":/imagenes/clientes.png" ) );
     clientes->setText( "Clientes" );
     clientes->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
     clientes->setTextAlignment( Qt::AlignBottom | Qt::AlignHCenter );
     FormPrefClientes *formCli = new FormPrefClientes( this );
     connect( this, SIGNAL( aplicar() ), formCli, SLOT( aplicar() ) );
     connect( this, SIGNAL( cargar() ), formCli, SLOT( cargar() ) );
     connect( this, SIGNAL( guardar() ), formCli, SLOT( guardar() ) );
     pagesWidget->addWidget( formCli );
     // Cargo las preferencias de los plugins
     EPlugin *plugin;
     foreach( plugin, ERegistroPlugins::getInstancia()->pluginsPunteros() )
     {
        //qDebug( QString( "Plugin: %1" ).arg( plugin->nombre() ).toLocal8Bit() );
        if( !plugin->formsPreferencias().isEmpty() )
        {
                QWidget *form;
                foreach( form, plugin->formsPreferencias() )
                {
                        // agrego el item a la lista
                        QListWidgetItem *opciones = new QListWidgetItem( contentsWidget );
                        opciones->setIcon( form->windowIcon() );
                        opciones->setText( form->windowTitle() );
                        opciones->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
                        opciones->setTextAlignment( Qt::AlignBottom | Qt::AlignHCenter );
                        contentsWidget->addItem( opciones );
                        // Agrego el formulario
                        pagesWidget->addWidget( form );
                        connect( this, SIGNAL( guardar() ), form, SLOT( guardar() ) );
                        connect( this, SIGNAL( cargar() ), form, SLOT( cargar() ) );
                        connect( this, SIGNAL( aplicar() ), form, SLOT( aplicar() ) );
                }
        }
     }
    // seteo el tamaño de los iconos
    contentsWidget->setIconSize( QSize( 42, 42 ) );

    ActCerrar  = new QAction( "Cerrar", this );
    ActCerrar->setShortcut( QKeySequence( "Ctrl+c" ) );
    ActCerrar->setIcon( QIcon( ":/imagenes/fileclose.png" ) );
    ActCerrar->setToolTip( "Cierra la ventana de preferencias ( Ctrl + c )" );
    ActCerrar->setStatusTip( "Cierra la ventana de preferencias, pero no modifica ningun valor" );
    connect( ActCerrar, SIGNAL( triggered() ), this, SLOT( cancelar()) );

    ActAplicar = new QAction( "Ap&licar", this );
    ActAplicar->setShortcut( QKeySequence( "Ctrl+l" ) );
    ActAplicar->setIcon( QIcon( ":/imagenes/aplicar.png" ) );
    ActAplicar->setToolTip( "Aplica los cambios realizados ( Ctrl + l )" );
    ActAplicar->setStatusTip( "Aplica los cambios realizados a la configuracion" );
    connect( ActAplicar, SIGNAL( triggered() ), this, SIGNAL( aplicar() ) );

    ActGuardar = new QAction( "&Guardar", this  );
    ActGuardar->setShortcut( QKeySequence( "Ctrl+g" ) );
    ActGuardar->setIcon( QIcon( ":/imagenes/guardar.png" ) );
    ActGuardar->setStatusTip( "Guarda la preferencias y cierra la ventana" );
    ActGuardar->setToolTip( "Guarda las preferencias y cierra la ventana ( Ctrl + g )" );
    connect( ActGuardar, SIGNAL( triggered() ), this, SIGNAL( guardar() ) );
    connect( ActGuardar, SIGNAL( triggered() ), this, SIGNAL( cerrar() ) );

    addAction( ActGuardar );
    addAction( ActAplicar );
    addAction( ActCerrar );

    setObjectName( "Configuracion" );
    setWindowTitle( "Configuracion de " + ERegistroPlugins::getInstancia()->pluginInfo()->nombrePrograma() );

    contentsWidget->setCurrentRow(0);

    Splitter = new QSplitter;
    Splitter->addWidget(contentsWidget);
    Splitter->addWidget(pagesWidget);
    Splitter->setCollapsible( 0, false );
    Splitter->setCollapsible( 1, false );

    QGridLayout *gl = new QGridLayout( this );
    gl->addWidget( Splitter );
    setLayout(gl);

    connect(contentsWidget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));

        connect( this, SIGNAL( guardar() ), this, SIGNAL( aplicar() ) );

        // Cargo la ultima pos de el spliter
        p->beginGroup( "Ventanas" );
        p->beginGroup( "Preferencias" );
        Splitter->restoreState( p->value( "spliter", QByteArray() ).toByteArray() );
        p->endGroup();
        p->endGroup();

        // Cargo las preferencias
        emit cargar();
}


FormPreferencias::~FormPreferencias()
{
        preferencias *p = preferencias::getInstancia();
        p->inicio();
        p->beginGroup( "Ventanas" );
        p->beginGroup( "Preferencias" );
        p->setValue( "spliter", Splitter->saveState() );
        p->endGroup();
        p->endGroup();
}


/*!
    \fn FormPreferencias::changePage(QListWidgetItem *current, QListWidgetItem *previous)
        Cambia la pagina de la configuracion
        @param current Item actual de la lista
        @param previous Item anterior en la lista
 */
void FormPreferencias::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}


/*!
    \fn FormPreferencias::cancelar()
 */
void FormPreferencias::cancelar()
{
 /*
 bool _no_guardo = false;
 for( int i=0; i< this->count(); i++ )
 {
  _no_guardo = qobject_cast<FormPrefHijo *>(this->widget(i))->tieneQueGuardar();
 }
 */
 close();
}

