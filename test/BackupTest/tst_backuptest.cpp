#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "edatabasetest.h"

class BackupTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    BackupTest();
    
private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void testGenerarBackup();
    void testCaseColasCorrectas();
    void testCaseColasCorrectas_data();
};

BackupTest::BackupTest()
{
    this->tablas << "compras"
                 << "compras_productos"
                 << "categorias_productos"
                 << "clientes"
                 << "factura"
                 << "item_factura"
                 << "paises"
                 << "periodo_servicio"
                 << "productos"
                 << "proveedor"
                 << "provincias"
                 << "servicios";
}

void BackupTest::init() { EDatabaseTest::init(); }

void BackupTest::initTestCase() { EDatabaseTest::initTestCase(); }

void BackupTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

void BackupTest::cleanup() { EDatabaseTest::cleanup(); }

#include "ebackup.h"
/*!
 * Genera el backup
 */
void BackupTest::testGenerarBackup()
{
    EBackup *eb = new EBackup();
    eb->CkBEstructura->setChecked( true );
    eb->CkBBaseDatos->setChecked( true );
    eb->CkBConfiguracion->setChecked( true );
    eb->Pestanas->setCurrentIndex( 0 );
    eb->iniciar( "test2" );

    QVERIFY( QFile::exists( QApplication::applicationDirPath().append( "test2.bkp" ) ) );
    QFile *archivo = new QFile( QApplication::applicationDirPath().append( "test2.bkp" ) );
    QVERIFY( archivo->open( QIODevice::ReadOnly ) );
    QVERIFY( archivo->size() > 0 );

    delete eb;
    eb=0;
    delete archivo;
    archivo=0;
}

#include <QSqlQuery>
/**
 *
 */
void BackupTest::testCaseColasCorrectas()
{
    QFETCH( QString, cola );

    QSqlQuery ccola;
    QVERIFY2( ccola.exec( cola ), QString( "Error: %1 - %2" ).arg( ccola.lastError().text() ).arg( ccola.lastQuery() ).toLocal8Bit() );
}

void BackupTest::testCaseColasCorrectas_data()
{
    QTest::addColumn<QString>("cola");
    QFile archivo( QApplication::applicationDirPath().append( "test.bkp" ) );
    if( archivo.open( QIODevice::ReadOnly ) ) {
        QString contenido = archivo.readAll();
        archivo.close();
        if( contenido.isEmpty() )
        { qWarning() << "El archivo esta vacio. \n Seleccione otro archivo para restaurar"; return; }
        // empiezo a analizar el contenido
        // tengo que encontrar la cabecera sino es invalido
        if( contenido.startsWith( "|->basedatossql->", Qt::CaseSensitive ) )
        {
         // Encontrado datos sql, elimino la cabecera
         contenido.remove( 0, QString( "|->basedatossql->formato= " ).size() );
         // ahora tiene que estar el formato
         QString formato = contenido.section( ";", 0, 0 );
         // saco esa subcadena
         contenido.remove( 0, formato.size() + 1 );
         // desde ahora hasta el fin de la etiqueta, es sql puro
         // busco la etiqueta de fin
         int posfinal = contenido.indexOf( "<-basedatossql<-|" );
         contenido.remove( posfinal, contenido.size() - posfinal );
         int posicion = 0;
         foreach( QString cola, contenido.split( ";" ) ) {
             cola.remove( '\n' );
            if( !cola.isEmpty() ) {
                QTest::newRow( QString::number( posicion ).toLocal8Bit() ) << cola.append( ';' );
                posicion++;
            }
         }
        } else {
            QVERIFY2( false, "No se pudo entender el archivo" );
        }
    } else {
        QVERIFY2( false, "No se pudo abrir el archivo" );
    }
}

QTEST_MAIN(BackupTest)

#include "tst_backuptest.moc"
