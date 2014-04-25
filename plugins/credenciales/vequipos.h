#ifndef VEQUIPOS_H
#define VEQUIPOS_H

#include "evlista.h"
#include "mequipos.h"

class VEquipos : public EVLista
{
    Q_OBJECT
public:
    explicit VEquipos(QWidget *parent = 0);

public slots:
    void agregar(bool autoeliminarid);

};

#endif // VEQUIPOS_H
