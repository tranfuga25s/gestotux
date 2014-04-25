#ifndef ENVIADOREMAIL_H
#define ENVIADOREMAIL_H

#include <QObject>
#include <QSqlDatabase>

#include "emodelomails.h"

class EnviadorEmail : public QObject
{
    Q_OBJECT
public:
    EnviadorEmail( QString usuario,
                   QString contrasena,
                   QString direccion,
                   QObject *parent,
                   QSqlDatabase db );
    void terminarEjecucion();

signals:
    void status( const QString &texto );

public slots:
    void enviarEmail();
    void iniciar();

private:
    QSqlDatabase db;
    QString _usuario;
    QString _contrasena;
    QString _host;
    EModeloMails *listaEmails;

};

#endif // ENVIADOREMAIL_H
