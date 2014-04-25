#ifndef MVGARANTIASSVG_H
#define MVGARANTIASSVG_H

#include <QSqlTableModel>

class MVGarantiasSvg : public QSqlTableModel
{
    Q_OBJECT
public:
    MVGarantiasSvg(QObject *parent = 0);
    QSqlRecord obtenerRegistro( const int id_garantia );
    
};

#endif // MVGARANTIASSVG_H
