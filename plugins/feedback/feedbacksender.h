#ifndef FEEDBACKSENDER_H
#define FEEDBACKSENDER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class FeedbackSender : public QObject
{
    Q_OBJECT
public:
    FeedbackSender( QObject *parent = 0 );

public slots:
    void verificarEnvio();

private slots:
    void respuesta( QNetworkReply * );

signals:
    void terminar();
    void pausar( unsigned int );

private:
    /*!
     * Administrador de red
     */
     QNetworkAccessManager *manager;

     /*!
      * Puntero de request
      */
     QNetworkRequest *req;

    /*!
     * Lista de solicitudes de red -  envio de colas
     */
     QNetworkReply *lista;

    /*!
     * Apuntador de archivo
     */
     QFile *f;

    /*!
     * Contador de reintentos
     */
     int reintentos;

};

#endif // FEEDBACKSENDER_H
