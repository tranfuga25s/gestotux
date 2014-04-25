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
#include "emysql.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QProgressBar>
#include <QTimer>
#include <QMessageBox>
#include "preferencias.h"
#include <QDebug>

EMysql::EMysql(QWidget* parent, Qt::WFlags fl)
: QDialog( parent, fl ), Ui::EMysqlBase()
{
        setupUi(this);
        this->setModal( false );
        id_timer = -1;
        preferencias *p = preferencias::getInstancia();
        p->beginGroup( "mysql" );
        LEUsuario->setText( p->value( "usuario" ).toString() );
        if( p->contains( "contra" ) )
        {
                LEContra->setText( p->value( "contra" ).toString() );
        }
        adjustSize();
        this->setWindowTitle( "Conexion remota a gestotux" );
        this->setWindowIcon( QIcon(":/imagenes/mysql.png" ) );
        LEContra->setEchoMode( QLineEdit::Password );
        connect( PBInterna, SIGNAL( clicked() ), this, SLOT( dbinterna() ) );
        connect( PBConectar, SIGNAL( clicked() ), this, SLOT( accept() ) );
        PBConectar->setDefault( true );
        if( p->value( "automatico" ).toBool() && p->contains( "contra" ) )
        {
                id_timer = this->startTimer( 100 );
                if( id_timer == 0 )
                {
                        qDebug( "Error al iniciar el timer" );
                }
        }
        p->endGroup();
        p = 0;
}

/*!
 * @fn EMysql::reject()
 * Slot llamado cuando se apreta el boton de cancelar
 */
void EMysql::reject()
{
  this->done( Cancelado );
}

/*!
 * @fn EMysql::accept()
 * Slot llamado cuando se apreta el boton de  aceptar, para conectar con la base de datos.
 */
void EMysql::accept()
{
  // intento conectar
  preferencias *p = preferencias::getInstancia();
  p->beginGroup( "mysql" );
  QSqlDatabase _db = QSqlDatabase::addDatabase( "QMYSQL" );
  _db.setHostName( p->value( "host", "localhost" ).toString() );
  _db.setPort( p->value( "puerto", 3306).toInt() );
  _db.setDatabaseName( p->value( "base", "gestotux" ).toString() );
  _db.setUserName( LEUsuario->text() );
  _db.setPassword( LEContra->text() );
  p->endGroup();
  p = 0;
  if( _db.open() )
  {
   qDebug( "Conectado con mysql" );
   this->done( Conectado );
  }
  else
  {
   switch( _db.lastError().type() )
   {
        case QSqlError::ConnectionError:
        {
                QMessageBox::information( this, QString::fromUtf8( "Error de conexión" ), QString::fromUtf8( "No se ha podido conectar a la base de datos. Verifique que se encuentre disponible y que su usuario y contraseña sean correctas" ) );
                _db.removeDatabase( _db.connectionName() );
                break;
        }
        default:
        {
                qDebug( "Error de conección" );
                qWarning( qPrintable( "Ultimo error: -> " + QString::number( _db.lastError().number() ) + "<- - " + _db.lastError().text() ) );
                 _db.removeDatabase( _db.connectionName() );
        }
    }
  }
}

/*!
    \fn EMysql::dbinterna()
    Slot llamado cuando se apreta el boton para utilizar la db interna ( SQLITE )
 */
void EMysql::dbinterna()
{
 this->done( Interna );
}


/*!
    \fn EMysql::timerEvent ( QTimerEvent * event )
    Slot llamado cuando se vence el tiempo de espera
 */
void EMysql::timerEvent ( QTimerEvent * event )
{
  qDebug() << "Timer ID:" << event->timerId();
  if( id_timer != -1 )
  {
   if( event->timerId() == id_timer )
   {
    this->killTimer( id_timer );
    qDebug( "autoconexion" );
    this->accept();
   }
   else
   {
    qDebug( "autoconexion pasada por id erroneo" );
    QDialog::timerEvent( event );
   }
  }
  else
  {
    qDebug( "autoconexion pasada por no id" );
   QDialog::timerEvent( event );
  }
}
