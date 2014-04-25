#ifndef DHISTORIALORDENTRABAJO_H
#define DHISTORIALORDENTRABAJO_H

#include <QDialog>

#include "mhistorialordentrabajo.h"

namespace Ui {
class DHistorialOrdenTrabajoBase;
}

class DHistorialOrdenTrabajo : public QDialog
{
    Q_OBJECT
    
public:
    explicit DHistorialOrdenTrabajo( QWidget *parent = 0 );
    ~DHistorialOrdenTrabajo();
    void setearComoCancelacion();
    void setearComoFacturacion();
    void setearComoCierre();
    void setearComoDevolverAlCliente();
    void setearIdOrdenTrabajo( const int id ) { _id_orden_trabajo = id; }
    void setearModeloHistorial( MHistorialOrdenTrabajo *m ) { _modelo_historial = m; }
    void filtrarBloqueantes();

public slots:
    void accept();

protected slots:
    void cambioTipo( int tipo );

private:
    Ui::DHistorialOrdenTrabajoBase *ui;
    MHistorialOrdenTrabajo *_modelo_historial;
    int _id_orden_trabajo;
};

#endif // DHISTORIALORDENTRABAJO_H
