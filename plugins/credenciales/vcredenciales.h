#ifndef VCREDENCIALES_H
#define VCREDENCIALES_H

#include "evlista.h"
#include "MCredenciales.h"

class VCredenciales : public EVLista
{
    Q_OBJECT
public:
    explicit VCredenciales(QWidget *parent = 0);

public slots:
    void imprimir();
    void buscar();
    void agregar( bool autoeliminarid );

private:
    MCredenciales *rmodel;
};

#endif // VCREDENCIALES_H
