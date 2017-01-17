#include "dialogoresumenporservicio.h"

#include "mservicios.h"
#include "mperiodoservicio.h"
#include "EReporte.h"

#include <QMessageBox>

DialogoResumenPorServicio::DialogoResumenPorServicio(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);


    this->setWindowTitle("Resumen Historio de deudores de Servicios");
    // Buscar rango de datos necesario
    QDate minimo = MPeriodoServicio::periodoMinimo();
    QDate maximo = MPeriodoServicio::periodoMaximo();
    if (!maximo.isValid()) {
        maximo = QDate::currentDate();
    }
    DEInicio->setDate(minimo);
    DEFin->setDate(maximo);

    MServicios *mservicios = new MServicios(this);

    CBServicios->setModel(mservicios);
    CBServicios->setModelColumn(mservicios->fieldIndex("nombre"));
}

void DialogoResumenPorServicio::changeEvent(QEvent *e)
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
 * \brief DialogoResumenPorServicio::accept
 */
void DialogoResumenPorServicio::accept()
{
   // valido que haya alg{un servicio elegido.
    if (CBServicios->currentIndex() > 0 ) {
        // valido que el servicio tenga al menos periodos facturados
        int id_servicio = CBServicios->currentIndex();
        if (!MPeriodoServicio::existeFacturacion( id_servicio)) {
            QMessageBox::warning(this, "faltan datos", "No hay ninguna facturación realizada sobre este servicio");
            return;
        }

        EReporte *rep = new EReporte( 0 );
        ParameterList parametros;
        parametros.append("fecha_inicio", DEInicio->date().toString(Qt::ISODate));
        parametros.append("fecha_fin"   , DEFin->date().toString(Qt::ISODate)   );
        parametros.append("id_servicio" , id_servicio);
        rep->especial( "ServiciosHistoricosPorServicio", parametros );
        rep->hacerPDF( parametros, QString( "Deuda de servicio %1 al %2" )
                       .arg( MServicios::getNombreServicio(id_servicio))
                       .arg( QDate::currentDate().toString( "dd-MM-yyyy" ) ) );
        delete rep;
        this->close();

    } else {
        QMessageBox::warning(this, "error",  "No se seleccionó ning{un servicio");
        QDialog::accept();
    }
}
