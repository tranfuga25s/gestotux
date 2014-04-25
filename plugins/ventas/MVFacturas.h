#ifndef MVFACTURAS_H
#define MVFACTURAS_H

#include <QSqlTableModel>

class MVFacturas : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit MVFacturas(QObject *parent = 0);
    QVariant data( const QModelIndex& item, int role ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void verAnuladas( bool sino );

};

#endif // MVFACTURAS_H
