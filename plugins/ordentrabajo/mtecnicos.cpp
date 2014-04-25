#include "mtecnicos.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QDate>

MTecnicos::MTecnicos(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable( "tecnico" );
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "Razon Social" );
    setHeaderData( 2, Qt::Horizontal, "Habilitado" );
    setHeaderData( 3, Qt::Horizontal, "Fecha de Ingreso" );
    setEditStrategy( QSqlTableModel::OnFieldChange );
}

QVariant MTecnicos::data(const QModelIndex &idx, int role) const
{
    switch( idx.column() ) {
        case 2:
        {
            if(role == Qt::DisplayRole) {
                return QSqlTableModel::data( idx, role ).toBool();
                break;
            }
        }
        case 3:
        {
            if( role == Qt::DisplayRole ) {
                return QSqlTableModel::data( idx, role ).toDate().toString( Qt::LocaleDate );
                break;
            }
        }
        default:
        {
            break;
        }
    }
    return QSqlTableModel::data( idx, role );
}

Qt::ItemFlags MTecnicos::flags( const QModelIndex & ) const
{
    return Qt::ItemFlags( !Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable );
}

bool MTecnicos::existe(const QString nombre)
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(razon_social) FROM tecnico WHERE razon_social = %1" ).arg( nombre ) ) ) {
        cola.next();
        if( cola.record().value(0).toInt() > 0 ) {
            return true;
        }
    } else {
        qDebug() << "Error al ejecutar la cola de contabilidad de cantidad de tecnicos en el sistema";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

bool MTecnicos::tieneDatosRelacionados( const int id_tecnico )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(id_tecnico) FROM operacion_orden_trabajo WHERE id_tecnico = %1" ).arg( id_tecnico ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                return true;
            } else {
                qWarning() << "Existe alguna orden de trabajo que está relacionada con este tecnico";
            }
        } else {
            qDebug() << "Error al ejecutar la cola de averiguacion de tecnico<->operacion_orden_trabajo";
            qDebug() << cola.lastError().text() << endl << cola.lastQuery();
        }
    }
    return false;
}
