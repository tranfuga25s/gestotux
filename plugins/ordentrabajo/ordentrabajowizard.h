#ifndef ORDENTRABAJOWIZARD_H
#define ORDENTRABAJOWIZARD_H

#include <QWizard>

class OrdenTrabajoWizard : public QWizard
{
    Q_OBJECT
public:

    enum {
        Pagina_Cliente,
        Pagina_ClienteNuevo,
        Pagina_Equipamiento,
        Pagina_EquipamientoExistente,
        Pagina_EquipamientoNuevo,
        Pagina_Final
    };

    OrdenTrabajoWizard( QWidget *parent = 0 );

 public slots:
    void done( int result );

signals:
    void actualizarModeloOrdenTrabajo();
    void actualizarModeloEquipamientos();
    void actualizarModeloClientes();
    
};

#endif // ORDENTRABAJOWIZARD_H
