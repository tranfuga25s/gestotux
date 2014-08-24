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
    QModelIndex parent(const QModelIndex &child) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    double getValorNominal( const int id_unidad, const double cantidad );
    double getMultiplo( const int id_unidad );

    bool tienePadre( const int id_unidad ) const;
    QVector<int> getHijos( const int id_unidad );
    int obtenerPadre( const int id_unidad ) const;

    bool agregarUnidad( const int id_padre, const QString nombre, double multiplicador );

    bool eliminar( const int id_unidad, const bool eliminar_hijos = false, const bool tiene_transaccion = false );

};

#endif // MUNIDADESPRODUCTOS_H
