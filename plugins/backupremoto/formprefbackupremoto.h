#ifndef FORMPREFBACKUPREMOTO_H
#define FORMPREFBACKUPREMOTO_H

#include "ui_formprefbackupremoto.h"
#include "preferencias.h"
#include "formprefhijo.h"
class QNetworkReply;
class QNetworkAccessManager;

class FormPrefBackupRemoto : public QWidget, private Ui::FormPrefBackupRemoto, public FormPrefHijo
{
    Q_OBJECT
    
public:
    explicit FormPrefBackupRemoto( QWidget *parent = 0, Qt::WFlags = 0 );

public slots:
    void guardar();
    void aplicar();
    void cargar();

private slots:
    void respuesta( QNetworkReply * );
    void cargarDatos();

private:
    QNetworkAccessManager *manager;

};

#endif // FORMPREFBACKUPREMOTO_H
