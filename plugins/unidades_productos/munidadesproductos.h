#ifndef MUNIDADESPRODUCTOS_H
#define MUNIDADESPRODUCTOS_H

#include <QSqlTableModel>

/**
 * @brief The MUnidadesProductos class
 * Modelo utilizado para gestionar el arbol de unidades
 * @author Esteban Zeller
 */
class MUnidadesProductos : public QSqlTableModel
{
    Q_OBJECT

public:
    MUnidadesProductos( QObject *parent = 0 );

    QVariant data(const QModelIndex &idx, int role) const;

    double getValorNominal( const int id_unidad, const double cantidad );
    bool tienePadre( const int id_unidad );
    double getMultiplo( const int id_unidad );

};

#endif // MUNIDADESPRODUCTOS_H
