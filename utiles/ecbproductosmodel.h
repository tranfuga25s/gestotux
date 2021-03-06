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

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole ) const;
    
    int columnCount(const QModelIndex &parent = QModelIndex() ) const;
    int rowCount(const QModelIndex &parent = QModelIndex() ) const;

    bool insertRow(int row, const QModelIndex &parent = QModelIndex() );
    bool removeRow(int row, const QModelIndex &parent = QModelIndex() );

    void inicializar();

    int agregarItem( const QString texto, double stock = 1.0, bool habilitado = true, int proveedor = 0 );

    bool arreglarItemTemporal( const int anterior, const int nuevo );

    bool existeID( const int id_producto );
    QString nombreProductoSegunID( const int id_producto );
    int buscarPorCodigo( const QString buscar );

    int obtenerPosicionSegunId( const int id_producto );

    QList<int> *getListaIDs();

    enum Columnas {
        Ids=0,
        Codigos=1,
        Nombres=2,
        Stock=3,
        Habilitado=4,
        IdProveedor=5,
        NombresStock=6
    };

private:
    QHash<int, int> *_ids;
    QHash<int, QString> *_codigos;
    QHash<int, QString> *_nombres;
    QHash<int, double>* _stock;
    QHash<int, bool> *_habilitado;
    QHash<int, int> *_proveedor;

    int _min;


};

#endif // ECBPRODUCTOSMODEL_H
