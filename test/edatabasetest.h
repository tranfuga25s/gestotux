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
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QFile>
#include <QSqlError>
#include <QSettings>

/*!
 * \brief The EDatabaseTest class
 *
 * \author Esteban Zeller
 */
class EDatabaseTest
{
public:
    EDatabaseTest();
    ~EDatabaseTest();
    void generarTabla( QString nombre );
    void iniciarTabla( QString nombre );
    void vaciarTabla( QString nombre );
    void borrarTabla( QString nombre );

    void generarTablas();
    void borrarTablas();
    void iniciarTablas();
    void vaciarTablas();

protected:
    QStringList tablas;

private:
    /*!
     * \brief mapa
     * Mapa de dependencia entre tablas
     */
    QMap<QString, QList<QString> > mapa;

    /*!
     * \brief _lista_tablas
     * Lista de tablas inicializadas
     */
    QList<QString> _lista_tablas;

    /*!
     * \brief _dependencias
     * Guarda el orden de las dependencias
     * Por cada vez que una tabla es dependiente de la primera se suma un número a su valor
     */
    QMap<QString, int> _dependencias;
    QMap<int, QString> _inverso_depenencias;

    /*!
     * \brief buscarDepenencias
     * \param nombre
     */
    void buscarDepenencias( QString nombre );


};

/*!
 * \brief EDatabaseTest::EDatabaseTest
 * \param parent
 */
EDatabaseTest::EDatabaseTest()
{
    if( QSqlDatabase::isDriverAvailable( "QSQLITE" ) )
    {
        qDebug() << "Usando base de datos: " << QCoreApplication::applicationDirPath().append( QDir::separator() ).append( "test.database" );
     QFile *base = new QFile( QCoreApplication::applicationDirPath().append( QDir::separator() ).append( "test.database" ).toLocal8Bit() );
     if( !base->open( QIODevice::ReadOnly ) )
     {
         qDebug() << "Creando base de datos...";
         QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
         DB.setDatabaseName( QCoreApplication::applicationDirPath().append( QDir::separator() ).append( "test.database" ) );
         if( !DB.open() ) {
             qDebug() << "Ultimo error: " << DB.lastError().text();
             abort();
         }
      }
      delete base;
      QSqlDatabase DB = QSqlDatabase::addDatabase( "QSQLITE" );
      DB.setDatabaseName( QCoreApplication::applicationDirPath().append( QDir::separator() ).append( "test.database" ) );
      if( !DB.open() )
      {
              qDebug() << "Ultimo error: " << DB.lastError().text();
              abort();
      } else {
          qDebug( "Base de datos SQLite abierta correctamente" );
      }
     } else {
        qFatal( "No se puede encontrar el driver de SQLITE" );
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // Cargo las dependencias
    QDir path( QCoreApplication::applicationDirPath() );
    path.cdUp();
    if( !path.cd( "sql" ) ) {
        qFatal( "No se puede entrar al directorio sql!" );
    }
    QSettings dep( path.filePath( "dependences.ini" ), QSettings::IniFormat );
    if( dep.status() != QSettings::NoError ) {
        qDebug() << "Error al cargar las preferencias!";
    } else {
        qDebug() << "Cargadas dependencias desde " << path.filePath( "dependences.ini" );
    }
    dep.beginGroup( "Dependences" );
    QStringList lista_tablas = dep.childKeys();
    foreach( QString tabla, lista_tablas ) {
        mapa.insert( tabla, dep.value( tabla ).toStringList() );
    }
}

/*!
 * \brief EDatabaseTest::~EDatabaseTest
 */
EDatabaseTest::~EDatabaseTest() {}

/**
 * @brief EDatabaseTest::generarTablas
 */
void EDatabaseTest::generarTablas()
{
    foreach( QString t, this->tablas ) {
        this->generarTabla( t );
    }
}

/*!
 * \brief EDatabaseTest::generarTabla
 * \param nombre
 */
void EDatabaseTest::generarTabla( QString nombre )
{

    // Ejecutar los nombres de tablas que dependan de esta
    if( mapa.find( nombre ) == mapa.end() ) {
        // Agrego la tabla porque no hay dependencias
        this->tablas.append( nombre );
        this->_dependencias.insert( nombre, 1 );
        return;
    }

    // busco las depenencias
    this->buscarDepenencias( nombre );
}

/*!
 * \brief EDatabaseTest::buscarDepenencias
 * \param nombre
 */
void EDatabaseTest::buscarDepenencias( QString nombre )
{
    // Verifico a ver si existen las dependencias
    QList<QString> lista = mapa.value( nombre );
    foreach( QString tabla, lista ) {
        /// @todo Ver dependencias circulares!
        if( tablas.contains( tabla ) ) {
            if( this->_dependencias.contains( tabla ) ) {
                this->_dependencias.insert( tabla, this->_dependencias.value( tabla ) + 1 );
            } else {
                this->_dependencias.insert( tabla, 1 );
            }

            continue;
        } else {
            this->buscarDepenencias( tabla );
        }
    }
}

/**
 * @brief EDatabaseTest::borrarTablas
 */
void EDatabaseTest::borrarTablas()
{
    // Tengo que borrarlas en el orden inverso al que están creadas
    QMultiMap<int, QString>::iterator it;
    it = this->_inverso_depenencias.end();
    while( it != this->_inverso_depenencias.begin() ) {
        --it;
        this->borrarTabla( it.value() );
    }
}

/*!
 * \brief EDatabaseTest::borrarTabla
 * \param nombre
 */
void EDatabaseTest::borrarTabla( QString nombre )
{
    if( _lista_tablas.contains( nombre ) ) {
        QSqlQuery cola;
        if( cola.exec( "DROP TABLE " + nombre ) ) {
            _lista_tablas.removeAll( nombre );
        }
    } else {
        qDebug() << "La tabla " << nombre << " no está inicializada. - No se borrará.";
    }
}

/**
 * @brief EDatabaseTest::iniciarTablas
 */
void EDatabaseTest::iniciarTablas()
{
    // Busco las tablas en el orden correcto
    this->_inverso_depenencias.clear();
    QMapIterator<QString, int> it(this->_dependencias);
    while (it.hasNext())
    {
        it.next();
        this->_inverso_depenencias.insert( it.value(), it.key() ); // Intercambio clave y valor y quedan ordenados porque QMap ordena automaticamente
    }
    qDebug() << this->_inverso_depenencias;
    foreach( QString t, this->_inverso_depenencias ) {
        this->iniciarTabla( t );
    }
}

/**
 * @brief EDatabaseTest::vaciarTablas
 */
void EDatabaseTest::vaciarTablas()
{
    foreach( QString t, this->tablas ) {
        this->vaciarTabla( t );
    }
}

/*!
 * \brief EDatabaseTest::vaciarTabla
 * \param nombre
 */
void EDatabaseTest::vaciarTabla( QString nombre )
{
    qDebug() << _lista_tablas << nombre;
    if( _lista_tablas.contains( nombre ) ) {
        QSqlQuery cola;
        cola.exec( "TRUNCATE TABLE " + nombre + ";" );
    } else {
        qDebug() << "La tabla " << nombre << " no está inicializada! - No se truncara.";
    }
}



/*!
 * \brief EDatabaseTest::iniciarTabla
 * \param nombre
 */
void EDatabaseTest::iniciarTabla( QString nombre ) {
    // Busco si no está ya inicializada
    if( !_lista_tablas.contains( nombre ) ) {
        qDebug() << "Inicializando tabla " << nombre;
        QDir *path = new QDir( QCoreApplication::applicationDirPath() );
        path->cdUp();
        path->cd( "sql" );
        path->cd( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() );
        // Ejecuto la lista de archivos
        if( path->exists( nombre + ".sql" ) )
        {
               QString nombre_archivo = nombre;
               nombre_archivo.append( ".sql" );
               QFile archivo( path->filePath( nombre_archivo ) );
               if( archivo.open( QIODevice::ReadOnly | QIODevice::Text ) )
               {
                       QStringList cadenas = QString( archivo.readAll() ).split( ";" );
                       QString cadena; QSqlQuery cola;
                       foreach( cadena, cadenas )
                       {
                               //qDebug() << cadena;
                               if( !cadena.isEmpty() && !cadena.isNull() ) {
                                       if( !cola.exec( cadena ) )
                                       {
                                               qWarning() << cadena;
                                               qWarning() << " Fallo...." << cola.lastError().text();
                                               return;
                                       }
                               }
                       }
                       archivo.close();
                       qDebug() << "Tabla " << nombre << " inicializada.";
                       _lista_tablas.append( nombre );
               }
               else
               {
                   qDebug() << "Error al abrir el archivo: " << nombre_archivo;
               }
        }
        else
        {
            qDebug() << "No se pudo generar la tabla -" << nombre << "-. No se encontro el archivo: " << path->filePath( nombre + ".sql" );
        }

    } else {
        qDebug() << "No se generó la tabla tabla -> " << nombre << " <- Ya existe";
    }
}

#endif // EDATABASETEST_H