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
#include "emodelomails.h"
#include "eservidoremail.h"

#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

EModeloMails::EModeloMails(QObject *parent, QSqlDatabase db)
 : QSqlTableModel(parent, db)
{
 setTable( "emails" );
 this->setEditStrategy( QSqlTableModel::OnManualSubmit );
}


EModeloMails::~EModeloMails()
{}

bool EModeloMails::existePendiente() const
{
    if( this->rowCount() > 0 ) {
        return true;
    }
    return false;
}


/*Mail * EModeloMails::takeFirst()
{
 //carga el primer email de la lista y lo elimina de la cola
 this->select();
 QSqlRecord primero = this->record( 0 );
 //Mail *mail = EEmail::instancia()->email();
 mail->setIdentificadorUnico( QUuid( primero.value("id_unico" ).toString() ) );
 EServidorEmail servidor( this, this->database() );
 mail->setHeader( servidor.de(),
			primero.value( "para" ).toString(),
			primero.value( "bcc" ).toString(),
			primero.value( "cc" ).toString(),
			primero.value( "asunto" ).toString()
		);
 mail->setMessageBody( primero.value( "cuerpo" ).toString() );
 mail->setContentType( primero.value( "content_type" ).toString() );
 this->removeRow( 0 );
 if( !this->submitAll() )
 {
  qDebug( "Error la poner desencolar un email" );
  qDebug( qPrintable( this->lastError().text() ) );
 }
 return mail;
}


void EModeloMails::encolar( void *mail )
{
 QSqlRecord encolar = this->record();
 encolar.remove(0);
 encolar.setValue( "id_unico", mail->identificadorUnico().toString() );
 encolar.setValue( "para", mail->to() );
 encolar.setValue( "bcc", mail->bcc() );
 encolar.setValue( "cc", mail->cc() );
 encolar.setValue( "asunto", mail->subject() );
 encolar.setValue( "cuerpo", mail->messageBody() );
 encolar.setValue( "content_type", mail->contentType() );
 if( this->insertRecord( -1, encolar ) )
 {
  if( !this->submitAll() )
  {
    qDebug( "Error en el submit del email" );
    qDebug( qPrintable( this->lastError().text() ) );
    return;
  }
  else
  {
   emit nuevoMail();
   qDebug( "Email encolado correctamente" );
   return;
  }
 }
 else
 {
  qDebug( "Error al agregar el email: no se enviara" );
  return;
 }
}
*/
