#ifndef MFACTURACIONEMITIDA_H
#define MFACTURACIONEMITIDA_H

#include <QSqlQueryModel>

class MFacturacionEmitida : public QSqlQueryModel
{
    Q_OBJECT
public:
    MFacturacionEmitida( bool deudor = false, QObject *parent = 0 );
    void setearServicio( int s );
    void setearPeriodo( int p );
    void generarDatos();
    QVariant data(const QModelIndex &item, int role) const;
    
private:
    bool _deudor;
    int _id_servicio;
    int _id_periodo_servicio;
    
};

#endif // MFACTURACIONEMITIDA_H
