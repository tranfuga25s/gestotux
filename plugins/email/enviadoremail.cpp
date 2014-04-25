#include "enviadoremail.h"
#include "emodelomails.h"

#include "smtpclient.h"
#include "mimetext.h"
#include "emailaddress.h"

EnviadorEmail::EnviadorEmail( QString usuario,
                              QString contrasena,
                              QString direccion,
                              QObject *parent,
                              QSqlDatabase db ) :
QObject(parent)
{
    this->db = db;
    this->_usuario = usuario;
    this->_contrasena = contrasena;
    this->_host = direccion;
    listaEmails = new EModeloMails( this, this->db );
}

/*!
 * \brief EnviadorEmail::terminarEjecucion
 */
void EnviadorEmail::terminarEjecucion()
{
}

/*!
 * \brief EnviadorEmail::enviarEmail
 */
void EnviadorEmail::enviarEmail()
{

}

/*!
 * \fn EnviadorEmail::iniciar()
 */
void EnviadorEmail::iniciar()
{
    // Verifico si existe un email para enviar y lo envía
    if( listaEmails->existePendiente() ) {
        // Envio el email correspondiente
        SmtpClient smtp( this->_host, 465, SmtpClient::SslConnection );
        smtp.setUser( this->_usuario );
        smtp.setPassword( this->_contrasena );

        MimeMessage email;

        // Cabeceras
        email.setSender( new EmailAddress( "email", "nombre" ) );
        email.addRecipient( new EmailAddress( "email", "nombre" ) );
        email.setSubject( "asunto" );

        // Formato texto
        MimeText contenido;
        contenido.setText( "Contenido" );

        email.addPart( &contenido );

        smtp.connectToHost();
        smtp.login();
        smtp.sendMail( email );
        smtp.quit();
    }
}
