#ifndef MDESCUENTOSTIPO_H
#define MDESCUENTOSTIPO_H

#include <QSqlTableModel>

class MDescuentosTipo : public QSqlTableModel
{
    public:
     MDescuentosTipo( QObject *parent = 0 );
     QVariant data( const QModelIndex &idx, int role ) const;
     Qt::ItemFlags flags( const QModelIndex &index ) const;
};

#endif // MDESCUENTOSTIPO_H
