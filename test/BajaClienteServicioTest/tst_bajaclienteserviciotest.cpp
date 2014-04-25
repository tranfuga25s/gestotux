#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include <QSqlDatabase>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

/**
 * @brief Caso de prueba para cuando el cliente al cual estamos dando de baja no posee cuenta corriente correctamente activada.
 * @author Esteban Zeller
 * @version 1
 * @bug #77
 */
class BajaClienteServicioTest : public QObject
{
    Q_OBJECT
    
public:
    BajaClienteServicioTest();
    void testDarBaja();
    void testDarBaja_data();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

private:
    bool hacerSql( QString nombre );
};

BajaClienteServicioTest::BajaClienteServicioTest()
{
}



void BajaClienteServicioTest::initTestCase()
{
    QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName( "test.gestotux.database" );
    if( !DB.open() )
    {
        qDebug() << "Ultimo error: " + DB.lastError().text();
        abort();
    } else {
        qDebug() << "Base de datos SQLite abierta correctamente";
    }
    // Genero los datos
    this->hacerSql( "BajaClienteServicioTest" );

}

void BajaClienteServicioTest::cleanupTestCase()
{
    // Vacío las tablas de datos

    // Cierro la base dedatos
    QSqlDatabase::database().close();
    // Elimino la conexión
    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
}

bool BajaClienteServicioTest::hacerSql( QString nombre )
{
 if( QFile::exists( ":/sql/"+nombre+"."+QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName()+".sql" ) )
 {
        QFile archivo( ":/sql/"+nombre+"."+QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName()+".sql" );
        if( archivo.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
                QStringList cadenas = QString( archivo.readAll() ).split( ";" );
                QString cadena; QSqlQuery cola;
                foreach( cadena, cadenas )
                {
                        qDebug() << cadena;
                        if( cadena.isEmpty() || cadena.isNull() ) {
                                qDebug() << "Cadena vacia, salteandola...";
                            } else {
                                if( !cola.exec( cadena ) )
                                {
                                        qDebug() << cadena;
                                        qDebug() << "Fallo...."  << cola.lastError().text();
                                        return false;
                                }
                                else
                                { qDebug() << "Ok"; }
                            }
                }
                return true;
        }
        else
        {
                qWarning() << "Error al abrir el archivo: :/sql/" << nombre << "." << QSqlDatabase::database().driverName() << ".sql";
                return false;
        }
 }
 else
 {
  qWarning() << "No se pudo generar las tablas del test " << nombre << ". No se encontro el archivo: :/sql/" << nombre << "." << QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() << ".sql";
  return false;
 }
}

void BajaClienteServicioTest::testDarBaja()
{
}

void BajaClienteServicioTest::testDarBaja_data()
{
}

QTEST_MAIN(BajaClienteServicioTest)

#include "tst_bajaclienteserviciotest.moc"
