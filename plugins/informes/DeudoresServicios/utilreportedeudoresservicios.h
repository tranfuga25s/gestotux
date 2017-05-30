#ifndef UTILREPORTEDEUDORESSERVICIOS_H
#define UTILREPORTEDEUDORESSERVICIOS_H

#include <QObject>
#include <QDate>

class UtilReporteDeudoresServicios : public QObject
{
    Q_OBJECT
public:
    explicit UtilReporteDeudoresServicios(QObject *parent = 0);
    bool static existeFacturacionServicio(const int id_servicio);
    QString static getNombreServicio(const int id_servicio);
    QDate static periodoMinimo();
    QDate static periodoMaximo();
    QString static getRazonSocial(const int id_cliente);
    QList<int> static listaDeServicios();
};

#endif // UTILREPORTEDEUDORESSERVICIOS_H
