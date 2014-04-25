#ifndef FORMFACTURACIONEMITIDA_H
#define FORMFACTURACIONEMITIDA_H

#include "eventana.h"
#include "ui_formfacturacionemitida.h"
class MFacturacionEmitida;

class FormFacturacionEmitida : public EVentana, private Ui::FormFacturacionEmitida
{
    Q_OBJECT
    
public:
    FormFacturacionEmitida(QWidget *parent = 0);
    void setearIdServicio( const int id_servicio );
    void setearIdPeriodo ( const int id_periodo  );
    
protected slots:
    void cambioServicio( int id_servicio );
    void cambioPeriodo( int id_periodo );
    void cambioHabilitadoPagados( bool estado );
    void imprimirListadoGeneral();
    void imprimirListadoGeneralPDF();

protected:
    void changeEvent(QEvent *e);

private:
    void cargarDatos();

    int _id_servicio;
    int _id_periodo_servicio;

    MFacturacionEmitida *mpagado;
    MFacturacionEmitida *mdeudor;

    QAction *ActVerPagado;
    QAction *ActListadoGeneral;
    QAction *ActListadoGeneralPDF;
};

#endif // FORMFACTURACIONEMITIDA_H
