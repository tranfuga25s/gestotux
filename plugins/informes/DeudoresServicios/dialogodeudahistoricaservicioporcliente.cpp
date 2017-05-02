#include "dialogodeudahistoricaservicioporcliente.h"

#include "EReporte.h"
#include "utilreportedeudoresservicios.h"

#include <QMessageBox>

DialogoDeudaHistoricaServicioPorCliente::DialogoDeudaHistoricaServicioPorCliente(QWidget *parent) :
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

void DialogoDeudaHistoricaServicioPorCliente::changeEvent(QEvent *e)
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

/*!
 * \brief DialogoDeudaHistoricaServicioPorCliente::accept
 */
void DialogoDeudaHistoricaServicioPorCliente::accept()
{
   // valido que haya algun servicio elegido.
    if (CBClientes->idClienteActual() >= 0 ) {
        // valido que el servicio tenga al menos periodos facturados
        int id_cliente = CBClientes->idClienteActual();

        EReporte *rep = new EReporte( 0 );
        ParameterList parametros;
        parametros.append("fecha_inicio", DEInicio->date().toString(Qt::ISODate));
        parametros.append("fecha_fin"   , DEFin->date().toString(Qt::ISODate)   );
        parametros.append("id_cliente" , id_cliente);
        rep->especial( "ServiciosHistoricosPorCliente", parametros );
        rep->hacerPDF( parametros, QString( "Deuda de servicios de %1 al %2" )
                       .arg( UtilReporteDeudoresServicios::getRazonSocial(id_cliente))
                       .arg( QDate::currentDate().toString( "dd-MM-yyyy" ) ) );
        delete rep;
        this->close();

    } else {
        QMessageBox::warning(
            this,
            "Error",
            QString::fromUtf8("No se seleccionó ningún servicio")
        );
        QDialog::accept();
    }
}
