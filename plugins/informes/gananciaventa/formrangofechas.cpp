#include "formrangofechas.h"

#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

FormRangoFechas::FormRangoFechas( QWidget *parent ) :
QDialog(parent)
{
    setupUi(this);
    this->setWindowTitle( "Elija el rango" );

    // Busco los maximos y mínimos
    QSqlQuery cola;
    if( cola.exec( "SELECT MAX(fecha), MIN(fecha) FROM factura" ) ) {
        if( cola.next() ) {
            DEDesde->setMinimumDate( cola.record().value(1).toDate() );
            DEHasta->setMinimumDate( DEDesde->date().addDays( 1 ) );
            DEHasta->setMaximumDate( cola.record().value(0).toDate() );
            DEDesde->setMaximumDate( DEHasta->date().addDays( -1 ) );
        }
    } else {
        qWarning( "No se pudieron encontrar datos de las fechas maximas y minimas permitidas. No se controlará" );
        qDebug( "Error de ejecución de la cola de obtención de fecha maxima y minima de reporte de ventas y ganancias" );
        qDebug( cola.lastError().text().toLocal8Bit() );
        qDebug( cola.lastQuery().toLocal8Bit() );
    }
}

void FormRangoFechas::accept()
{
    qWarning( "No implementado" );
}

void FormRangoFechas::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}
