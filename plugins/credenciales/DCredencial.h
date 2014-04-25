#ifndef DCREDENCIAL_H
#define DCREDENCIAL_H

#include "ui_DCredencial.h"
class EReporte;
class MCredenciales;

class DCredencial : public QDialog, private Ui::DCredencialBase
{
    Q_OBJECT

public:
    explicit DCredencial(QWidget *parent = 0);
    void setearEquipo(const int id_equipo, const QString equipo );

signals:
    void agregarVentana( QWidget * );

public slots:
    void accept();

private:
    void limpiarDatos();
    QString equipo;
    int id_equipo;
    EReporte *reporte;
    MCredenciales *modelo;

};

#endif // DCREDENCIAL_H
