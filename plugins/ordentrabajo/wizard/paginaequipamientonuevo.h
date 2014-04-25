#ifndef PAGINAEQUIPAMIENTONUEVO_H
#define PAGINAEQUIPAMIENTONUEVO_H

#include "ui_paginaequipamientonuevo.h"

class PaginaEquipamientoNuevo : public QWizardPage, private Ui::PaginaEquipamientoNuevo
{
    Q_OBJECT
    
public:
    PaginaEquipamientoNuevo(QWidget *parent = 0);
    int nextId() const;
    
};

#endif // PAGINAEQUIPAMIENTONUEVO_H
