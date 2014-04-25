#ifndef MVEQUIPAMIENTO_H
#define MVEQUIPAMIENTO_H

#include <QSqlTableModel>

class MVEquipamiento : public QSqlTableModel
{
    Q_OBJECT
public:
    MVEquipamiento( QObject *parent = 0 );
    QVariant data(const QModelIndex &idx, int role) const;
    
};

#endif // MVEQUIPAMIENTO_H
