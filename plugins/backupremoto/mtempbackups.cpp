#include "mtempbackups.h"

MTempBackups::MTempBackups(QObject *parent) :
QAbstractTableModel(parent)
{
    _id_backup = new QHash<int, int>();
    _fechas = new QHash<int, QDate>();
    _tamanos = new QHash<int, double>();
}

MTempBackups::~MTempBackups()
{
    delete _id_backup;
    delete _fechas;
    delete _tamanos;
}

bool MTempBackups::setData( const QModelIndex &index, const QVariant &value, int role )
{
    return false;
}


int MTempBackups::columnCount(const QModelIndex &) const
{ return 3; }

void MTempBackups::clear()
{
    emit beginRemoveRows( QModelIndex(), 0,_id_backup->size() );
    _id_backup->clear();
    _fechas->clear();
    _tamanos->clear();
    emit endRemoveRows();
}


int MTempBackups::rowCount(const QModelIndex &) const
{  return _id_backup->size(); }


Qt::ItemFlags MTempBackups::flags(const QModelIndex &) const
{
    return Qt::ItemFlags( !Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable );
}


QVariant MTempBackups::data( const QModelIndex &index, int role ) const
{
    switch( index.column() ) {
        case 0:
        {
            if( role == Qt::DisplayRole ) {
                return _id_backup->value( index.row() );
            }
            break;
        }
        case 1:
        {
            if( role == Qt::DisplayRole ) {
                return _fechas->value( index.row() ).toString( Qt::LocaleDate );
            }
            break;
        }
        case 2:
        {
            if( role == Qt::DisplayRole ) {
                return _tamanos->value( index.row() );
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return QVariant();
}
