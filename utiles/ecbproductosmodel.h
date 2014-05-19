#ifndef ECBPRODUCTOSMODEL_H
#define ECBPRODUCTOSMODEL_H

#include <QAbstractTableModel>

/*!
 * \brief The ECBProductosModel class
 * Clase que permite filtrar los productos en el sistema.
 */
class ECBProductosModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ECBProductosModel( QObject *parent = 0 );
    ~ECBProductosModel();

    QVariant data(const QModelIndex &index, int role) const;
    
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    void inicializar();

private:
    QHash<int, int> *_ids;
    QHash<int, QString> *_codigos;
    QHash<int, QString> *_nombres;
    QHash<int, double>* _stock;
    QHash<int, bool> *_habilitado;
    QHash<int, int> *_proveedor;

    int _min;

    enum Columnas {
        Ids=0,
        Codigos=1,
        Nombres=2,
        Stock=3,
        Habilitado=4,
        IdProveedor=5
    };
};

#endif // ECBPRODUCTOSMODEL_H
