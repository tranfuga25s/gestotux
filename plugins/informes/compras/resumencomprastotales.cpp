#include "resumencomprastotales.h"

#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlField>
#include <QDebug>

#include "mcompra.h"

ResumenComprasTotales::ResumenComprasTotales(QObject *parent) :
QStandardItemModel(parent)
{
    _metodo_temporal = PorMes;
}

/*!
 * \brief ResumenComprasTotales::setearMetodoTemporal
 * \param metodo
 */
void ResumenComprasTotales::setearMetodoTemporal( DivisionTemporal metodo )
{
    this->_metodo_temporal = metodo;
    //this->update();
}

/*!
 * \brief ResumenComprasTotales::actualizarDatos
 */
void ResumenComprasTotales::actualizarDatos()
{
    QString groupBy;
    groupBy.clear();
    if( _metodo_temporal == ResumenComprasTotales::SinDivision ) {
        // No separo nada
    } else if( _metodo_temporal == ResumenComprasTotales::PorMes ||
               _metodo_temporal == ResumenComprasTotales::PorBimestre ||
               _metodo_temporal == ResumenComprasTotales::PorCuatrimestre ||
               _metodo_temporal == ResumenComprasTotales::PorSeximestre ) {
        groupBy = " GROUP BY MONTH( fecha ), YEAR( fecha ) ";
    } else if( _metodo_temporal == ResumenComprasTotales::PorAno ) {
        groupBy = " GROUP BY YEAR( fecha ) ";
    }

    QSqlQuery cola;
    if( !cola.exec( QString( "SELECT * FROM resumen_compra GROUP BY %1" ).arg( groupBy ) ) ) {
            qWarning() << "No se pudieron cargar los datos necesarios!";
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
            return;
    }
    // Cargo los datos nuevos
    /*while( cola.next() ) {

    }*/
}
