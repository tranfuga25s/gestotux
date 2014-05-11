#ifndef RESUMENCOMPRASTOTALES_H
#define RESUMENCOMPRASTOTALES_H

#include <QSqlQueryModel>
#include <QMap>
#include "TiposPeriodos.h"
/*!
 * \brief The ResumenComprasTotales class
 * Modelo para mostrar los totales de compras por fecha
 */
class ResumenComprasTotales : public QSqlQueryModel
{
    Q_OBJECT

public:
    ResumenComprasTotales( QObject *parent = 0 );
    void setearMetodoTemporal( DivisionTemporal metodo );

    QVariant data(const QModelIndex &idx, int role) const;

private:
    void actualizarDatos();
    DivisionTemporal _metodo_temporal;
    QMap<QString,QString> consultas;
};

#endif // RESUMENCOMPRASTOTALES_H
