#ifndef FORMORDENTRABAJO_H
#define FORMORDENTRABAJO_H

#include "eventana.h"
#include "ui_formordentrabajobase.h"
#include "mordentrabajo.h"
#include "mhistorialordentrabajo.h"
#include "mequipamiento.h"

using namespace Ui;

class FormOrdenTrabajo : public EVentana, public FormOrdenTrabajoBase
{
    Q_OBJECT
    
public:
    FormOrdenTrabajo( QWidget *parent = 0 );
    void setearIdOrdenTrabajo( const int id_orden_trabajo );

protected slots:
    void cerrarOrden();
    void devolverCliente();
    void agregarFacturacion();
    void eliminarFacturacion();
    void agregarHistorial();
    void eliminarHistorial();
    void facturarOrden();
    void imprimir();
    void pdf();
    void cancelar();
    void guardar();
    
protected:
    void changeEvent( QEvent *e );

private:
    QAction *ActCerrarOrden;
    QAction *ActFacturarOrden;
    QAction *ActDevolverCliente;
    QAction *ActImprimir;
    QAction *ActPdf;
    QAction *ActCancelarOrden;

    int _id_orden_trabajo;

    void cargarDatosEquipamiento();

    MEquipamiento *_modelo_equipamiento;
    MOrdenTrabajo *_modelo_orden;
    MHistorialOrdenTrabajo *_modelo_historial;
    MHistorialOrdenTrabajo *_modelo_historial_facturacion;

};

#endif // FORMORDENTRABAJO_H
