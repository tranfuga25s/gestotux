#ifndef PAGINAEQUIPAMIENTOEXISTENTE_H
#define PAGINAEQUIPAMIENTOEXISTENTE_H

#include "ui_paginaequipamientoexistente.h"
class MEquipamiento;

class PaginaEquipamientoExistente : public QWizardPage, private Ui::PaginaEquipamientoExistente
{
    Q_OBJECT
    
public:
    explicit PaginaEquipamientoExistente(QWidget *parent = 0);
    int nextId() const;
    bool validatePage();

private:
    MEquipamiento *modelo;

};

#endif // PAGINAEQUIPAMIENTOEXISTENTE_H
