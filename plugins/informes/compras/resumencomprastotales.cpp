#include "resumencomprastotales.h"

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
}
