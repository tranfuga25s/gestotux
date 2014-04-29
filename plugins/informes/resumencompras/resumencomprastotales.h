#ifndef RESUMENCOMPRASTOTALES_H
#define RESUMENCOMPRASTOTALES_H

#include <QSqlTableModel>
#include "TiposPeriodos.h"
/*!
 * \brief The ResumenComprasTotales class
 * Modelo para mostrar los totales de compras por fecha
 */
class ResumenComprasTotales : public QSqlTableModel
{
    Q_OBJECT

public:
    ResumenComprasTotales( QObject *parent = 0 );
    void setearMetodoTemporal( DivisionTemporal metodo );

    QVariant data(const QModelIndex &idx, int role) const;

private:
    void actualizarDatos();
    DivisionTemporal _metodo_temporal;
};

#endif // RESUMENCOMPRASTOTALES_H
