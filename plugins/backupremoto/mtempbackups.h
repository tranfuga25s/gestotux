#ifndef MTEMPBACKUPS_H
#define MTEMPBACKUPS_H

#include <QAbstractTableModel>
#include <QDate>

class MTempBackups : public QAbstractTableModel
{
    Q_OBJECT
public:
    MTempBackups( QObject *parent = 0 );
    ~MTempBackups();
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    void clear();

private:
    QHash<int, int> *_id_backup;
    QHash<int, QDate> *_fechas;
    QHash<int, double> *_tamanos;
    
};

#endif // MTEMPBACKUPS_H
