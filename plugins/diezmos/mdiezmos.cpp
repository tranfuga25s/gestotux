#include "mdiezmos.h"

#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>

MDiezmos::MDiezmos(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable( "diezmos" );
    setHeaderData( 0, Qt::Horizontal, "#Id" );
    setHeaderData( 1, Qt::Horizontal, "Fecha" );
    setHeaderData( 2, Qt::Horizontal, "Descripcion" );
    setHeaderData( 3, Qt::Horizontal, "A dar" );
    setHeaderData( 4, Qt::Horizontal, "Diezmado" );
    setHeaderData( 5, Qt::Horizontal, "#Referencia" );
    setHeaderData( 6, Qt::Horizontal, "Saldo" );
    saldos = new QVector<double>();
}

MDiezmos::~MDiezmos()
{
    delete saldos;
    saldos = 0;
}

/*!
 * \brief MDiezmos::columnCount
 * \param parent
 * \return
 */
int MDiezmos::columnCount(const QModelIndex &parent) const
{
    return QSqlTableModel::columnCount( parent ) + 1;
}

/*!
 * \brief MDiezmos::data
 * \param idx
 * \param role
 * \return
 */
QVariant MDiezmos::data(const QModelIndex &idx, int role) const
{
    switch( idx.column() ) {
        case 1:
        {
            switch( role ) {
                case Qt::DisplayRole: {
                    return QSqlTableModel::data( idx, role ).toDate().toString( Qt::SystemLocaleShortDate );
                    break;
                }
                default: {break;}
            }
            break;
        }
        case 3:
        case 4:
        {
            switch( role ) {
                case Qt::DisplayRole:
                {
                    double valor = QSqlTableModel::data( idx, role ).toDouble();
                    return QString( "$ %L1").arg( valor, 10, 'f', 2 );
                    break;
                }
                default: { break; }
            }
            break;
        }
        case 6:
        {
            switch( role ) {
                case Qt::DisplayRole:
                {
                    double nuevo_saldo = 0.0;
                    if( idx.row() > 1 ) {
                     nuevo_saldo = saldos->at( idx.row() -1 );
                    }
                    nuevo_saldo -= data( index( idx.row(), fieldIndex( "haber" ) ), Qt::EditRole ).toDouble();
                    nuevo_saldo += data( index( idx.row(), fieldIndex( "debe" ) ), Qt::EditRole ).toDouble();
                    int pos = idx.row();
                    this->saldos->insert( pos, nuevo_saldo );
                    return QString( "$ %L1" ).arg( saldos->at( pos ), 10, 'f', 2 );
                    break;
                }
                default: { break; }
            }
            break;
        }
        default: { break; }
    }
    return QSqlTableModel::data( idx, role );
}

/*!
 * \brief MDiezmos::agregarReciboDiezmo
 * Agregar un dar diezmo a la tabla de diezmos
 * \param fecha Fecha en que se dío el diezmo
 * \param monto Monto de diezmo dado
 * \return Verdadero si se ingreso el registro correctamente o falso si fallo
 */
bool MDiezmos::agregarReciboDiezmo( const QDate fecha, const double monto )
{
    QSqlQuery cola;
    if( !cola.prepare( "INSERT INTO diezmos( fecha, descripcion, haber ) "
                       " VALURES ( :fecha, :descripcion, :monto )" ) ) {
        qWarning() << "Error al preparar la cola de ejecucion para dado de diezmo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    cola.bindValue( ":fecha", fecha );
    cola.bindValue( ":descripcion", "Diezmo dado" );
    cola.bindValue( ":monto", monto );
    if( !cola.exec() ) {
        qWarning() << "Error al ejecutar la cola de ejecucion para dado de diezmo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    } else {
        return true;
    }
}

/*!
 * \brief MDiezmos::eliminarEntrada
 * \param id_entrada_diezmo
 * \return
 */
bool MDiezmos::eliminarEntrada(const int id_entrada_diezmo)
{
    QSqlQuery cola;
    if( !cola.exec( QString( "DELETE FROM diezmos WHERE id_item_diezmo = %1" ).arg( id_entrada_diezmo ) ) ) {
        qWarning() << "Error al ejecutar la cola de eliminacion de item de diezmo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    return true;
}

/*!
 * \brief MDiezmos::agregarRegistro
 * \param fecha
 * \param monto
 * \param descripcion
 * \param id_referencia
 */
bool MDiezmos::agregarRegistro(const QDate fecha, const double monto, const QString descripcion, const int id_referencia)
{
    QSqlQuery cola;
    if( !cola.prepare( "INSERT INTO diezmos( fecha, descripcion, debe, id_referencia ) "
                       " VALURES ( :fecha, :descripcion, :monto, :id_referencia )" ) ) {
        qWarning() << "Error al preparar la cola de ejecucion de deuda de diezmo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    cola.bindValue( ":fecha"        , fecha         );
    cola.bindValue( ":descripcion"  , descripcion   );
    cola.bindValue( ":monto"        , monto         );
    cola.bindValue( ":id_referencia", id_referencia );
    if( !cola.exec() ) {
        qWarning() << "Error al ejecutar la cola de ejecucion de deuda de diezmo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    } else {
        return true;
    }
}
