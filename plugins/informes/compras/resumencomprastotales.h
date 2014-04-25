#ifndef RESUMENCOMPRASTOTALES_H
#define RESUMENCOMPRASTOTALES_H

#include <QStandardItemModel>
#include <QSqlQueryModel>

/*!
 * \brief The ResumenComprasTotales class
 */
class ResumenComprasTotales : public QStandardItemModel
{
    Q_OBJECT

public:
    enum DivisionTemporal {
        SinDivision = 1,
        PorMes = 2,
        PorBimestre = 3,
        PorCuatrimestre = 4,
        PorSeximestre = 5,
        PorAno = 6
    };
    ResumenComprasTotales( QObject *parent = 0 );
    void setearMetodoTemporal( DivisionTemporal metodo );

private:
    void actualizarDatos();

    QStandardItem *padre;
    DivisionTemporal _metodo_temporal;
};

#endif // RESUMENCOMPRASTOTALES_H
