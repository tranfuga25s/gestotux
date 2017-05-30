#include "dialogoresumenservicioshistorico.h"

#include "EReporte.h"

#include "utilreportedeudoresservicios.h"

DialogoResumenServiciosHistorico::DialogoResumenServiciosHistorico(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setWindowTitle("Resumen Historico de Servicios Pagos");
    // Buscar rango de datos necesario
    QDate minimo = UtilReporteDeudoresServicios::periodoMinimo();
    QDate maximo = UtilReporteDeudoresServicios::periodoMaximo();
    if (!maximo.isValid()) {
        maximo = QDate::currentDate();
    }
    DEInicio->setDate(minimo);
    DEFin->setDate(maximo);
}

/**
 * @brief DialogoResumenServiciosHistorico::changeEvent
 * @param e
 */
void DialogoResumenServiciosHistorico::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

/**
 * @brief DialogoResumenServiciosHistorico::accept
 */
void DialogoResumenServiciosHistorico::accept()
{
    EReporte *rep = new EReporte( 0 );
    ParameterList parametros;
    parametros.append("fecha_inicio", DEInicio->date().toString(Qt::ISODate));
    parametros.append("fecha_fin"   , DEFin->date().toString(Qt::ISODate)   );
    rep->especial( "ServiciosHistoricos", parametros );
    rep->hacerPDF( parametros, QString( "Deudas Cuotas al %1" ).arg( QDate::currentDate().toString( "dd-MM-yyyy" ) ) );
    delete rep;
    this->close();
}
