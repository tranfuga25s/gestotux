#ifndef FormDetalleCuotas_H
#define FormDetalleCuotas_H

#include "ui_formcuotasbase.h"
#include "eventana.h"
class QTextDocument;
class MPlanCuota;
class MItemPlanCuota;

class FormDetalleCuotas : public EVentana, public Ui::FormDetalleCuotasBase
{
    Q_OBJECT
    
public:
    FormDetalleCuotas( QWidget *parent = 0 );
    ~FormDetalleCuotas();
    void setearIdPlanCuota( int id, bool inicializar_submodelo = true );
    
protected:
    void changeEvent( QEvent *e );

private slots:
    void imprimir();
    void pdf();

private:
    QAction *ActImprimir;
    QAction *ActPdf;

    MPlanCuota *modelo;
    MItemPlanCuota *modelo_item;
    QTextDocument *documento;

    void generaReporte();

};

#endif // FormDetalleCuotas_H
