#ifndef VEQUIPAMIENTO_H
#define VEQUIPAMIENTO_H

#include "evlista.h"
class MVEquipamiento;
class MEquipamiento;
/**
 * @brief The VEquipamiento class
 * Esta clase sirve para ver cuales de los equipamientos actuales está dado de alto en el sistema
 * @author Esteban Zeller
 */
class VEquipamiento : public EVLista
{
    Q_OBJECT
public:
    VEquipamiento( QWidget *parent = 0 );

public slots:
    void agregar(bool autoeliminarid);
    void modificar();
    void eliminar();
    void darBaja();
    void darAlta();

    void menuContextual(const QModelIndex &indice, QMenu *menu);

private:
    MVEquipamiento *modelo;

    QAction *ActDarBaja;
    QAction *ActDarAlta;
};

#endif // VEQUIPAMIENTO_H
