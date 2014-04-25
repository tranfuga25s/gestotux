#ifndef MVREMITO_H
#define MVREMITO_H

#include <QSqlTableModel>

class MVRemito : public QSqlTableModel
{
    Q_OBJECT
public:
    MVRemito(QObject *parent = 0);
    QVariant data( const QModelIndex& item, int role ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void verAnulados( bool sino );

};

#endif // MVFACTURAS_H
