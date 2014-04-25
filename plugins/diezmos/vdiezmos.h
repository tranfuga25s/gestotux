#ifndef VDIEZMOS_H
#define VDIEZMOS_H

#include "mdiezmos.h"
#include "evlista.h"

class VDiezmos : public EVLista
{
    Q_OBJECT
public:
    VDiezmos(QWidget *parent = 0);

protected slots:
    void agregar(bool autoeliminarid);
    void eliminar();

protected:
    MDiezmos *modelo;
    
};

#endif // VDIEZMOS_H
