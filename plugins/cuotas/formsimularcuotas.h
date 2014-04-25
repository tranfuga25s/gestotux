#ifndef FORMSIMULARCUOTAS_H
#define FORMSIMULARCUOTAS_H

#include "ui_formsimularcuotasbase.h"
#include "eventana.h"
#include "msimularcuotas.h"
class QTextDocument;

class FormSimularCuotas : public EVentana, private Ui::FormSimularCuotasBase
{
    Q_OBJECT
    
public:
    FormSimularCuotas( QWidget *parent = 0 );
    ~FormSimularCuotas();
    void setearIdCliente( int id_cliente ) { _id_cliente = id_cliente; }
    int idCliente() { return _id_cliente; }
    void setearTotal( double Total ) { DSBImporte->setValue( Total ); }
    void setearConfirmar( bool conf );
    void setearTipoComprobante( MPlanCuota::TipoComprobante tipo ) { _tipo_comprobante = tipo; }
    MPlanCuota::TipoComprobante tipoComprobante() { return _tipo_comprobante; }
    
protected:
    void changeEvent( QEvent *e );

protected slots:
    void cambioEntrega( double cantidad );
    void cambioImporte( double cantidad );
    void cambioInteres( double cantidad );
    void cambioCantidad( int cantidad );
    void cambioPeriodo( int idx );
    void cambioFechaInicio( QDate fecha );

private slots:
    void simular();
    void imprimir();
    void pdf();
    void confirmar();

signals:
    void emitirIdPlanCuota( int );

private:
    QAction *ActSimular;
    QAction *ActConfirmar;
    QAction *ActImprimir;
    QAction *ActPdf;

    MSimularCuotas *modelo;
    QTextDocument *documento;

    void generaReporte();

    int _id_cliente;
    MPlanCuota::TipoComprobante _tipo_comprobante;

};

#endif // FORMSIMULARCUOTAS_H
