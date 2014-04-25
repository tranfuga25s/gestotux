#ifndef EDATABASETEST_H
#define EDATABASETEST_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QString>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>

class EDatabaseTest : public QObject
{
    Q_OBJECT
public:
    EDatabaseTest( QObject *parent = 0 );
    ~EDatabaseTest();
    void generarTabla( QString nombre );

private:
    /*!
     * \brief mapa
     * Mapa de dependencia entre tablas
     */
    QMap<QString, QList<QString> > mapa;
    /*!
     * \brief archivos
     * Mapa de en que archivo se encuentra las tablas
     */
    QMap<QString, QString> archivos;
    /*!
     * \brief _lista_archivos
     * Lista de archivos a ejecutar
     */
    QList<QString> *_lista_archivos;

    void buscarDepenencias( QString nombre );
};

#include <QSqlDatabase>
#include <QFile>
#include <QSqlError>

EDatabaseTest::EDatabaseTest( QObject *parent )
: QObject( parent )
{
    if( QSqlDatabase::isDriverAvailable( "QSQLITE" ) )
    {
     QFile *base = new QFile( QCoreApplication::applicationDirPath().append( QDir::separator() ).append( "gestotux.database" ).toLocal8Bit() );
     if( !base->open( QIODevice::ReadOnly ) )
     {
       qDebug( "-------------------------------------------------" );
       qDebug( "El archivo de Base de datos no existe!");
       qDebug( "-------------------------------------------------" );
              QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
              DB.setDatabaseName( QCoreApplication::applicationDirPath().append( QDir::separator() ).append( "gestotux.database" ) );
              if( !DB.open() )
              {
                      qDebug() << "Ultimo error: " << DB.lastError().text();
                      abort();
              }
      }
      else
      {
              // Aunque exista chequeo que no sea de tam 0
              if( base->size() <= 0 )
              { qFatal( "Error! El archivo de db tiene menos o es igual a 0 bytes " ); }
      }
      delete base;
      QSqlDatabase DB = QSqlDatabase::addDatabase( "QSQLITE" );
      DB.setDatabaseName( QCoreApplication::applicationDirPath().append( QDir::separator() ).append( "test.database" ) );
      if( !DB.open() )
      {
              qDebug() << "Ultimo error: " << DB.lastError().text();
              abort();
      } else { qDebug( "Base de datos SQLite abierta correctamente" ); }
     } else {
        qFatal( "No se puede encontrar el driver de SQLITE" );
    }
    // Inicializo el listado de archivos
    _lista_archivos = new QList<QString>();

}

EDatabaseTest::~EDatabaseTest() {
    delete _lista_archivos;
    _lista_archivos = 0;
}

void EDatabaseTest::generarTabla( QString nombre )
{
    // Ejecutar los nombres de tablas que dependan de esta
    if( mapa.find( nombre ) == mapa.end() ) {
        qFatal( "No se encuentra la tabla necesaria" );
        return;
    }

    // busco las depenencias
    buscarDependencias( nombre );

    // Ejecuto la lista de archivos

    foreach( QString archivo, _lista_archivos ) {
        if( QFile::exists(  QCoreApplication::applicationDirPath().append( QDir::separator() )+nombrePlug+"."+QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName()+".sql" ) )
        {
               QFile archivo( ":/sql/"+nombrePlug+"."+QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName()+".sql" );
               if( archivo.open( QIODevice::ReadOnly | QIODevice::Text ) )
               {
                       QStringList cadenas = QString( archivo.readAll() ).split( ";" );
                       QString cadena; QSqlQuery cola;
                       foreach( cadena, cadenas )
                       {
                               //qDebug() << cadena;
                               if( cadena.isEmpty() || cadena.isNull() ) {
                                       qDebug() << "Cadena vacia, salteandola...";
                                   } else {
                                       if( !cola.exec( cadena ) )
                                       {
                                               qDebug() << cadena;
                                               qDebug() << " Fallo...." << cola.lastError().text();
                                               return false;
                                       }
                                       else
                                       { qDebug( "Ok" ); }
                                   }
                       }
                       return true;
               }
               else
               {
                       qWarning() << "Error al abrir el archivo: :/sql/" << nombrePlug << "." << QSqlDatabase::database().driverName() << ".sql";
                       return false;
               }
        }
        else
        {
         qWarning() << "No se pudo generar las tablas del plugin " << nombrePlug << ". No se encontro el archivo: :/sql/" << nombrePlug << "." << QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() << ".sql";
         return false;
        }
    }
}

void EDatabaseTest::buscarDepenencias( QString nombre )
{
    // Agrego el archivo actual
    _lista_archivos->append( archivos.value( tabla ) );

    // Verifico a ver si existen las dependencias
    QList<QString> lista = mapa.value( nombre );
    foreach( QString tabla, lista ) {
        if( _lista_archivos->contains( archivos.value( tabla ) ) ) {
            continue;
        } else {
            buscarDependencias( tabla );
        }
    }
}

#endif // EDATABASETEST_H