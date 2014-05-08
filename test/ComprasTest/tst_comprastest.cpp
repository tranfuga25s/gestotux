#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"

class ComprasTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    ComprasTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testComprasEtiquetaFecha();
};

ComprasTest::ComprasTest()
{
    this->tablas << "compras" << "compras_productos" << "productos";
}
void ComprasTest::initTestCase() { EDatabaseTest::generarTablas(); }
void ComprasTest::init() { EDatabaseTest::iniciarTablas(); }
void ComprasTest::cleanup() { EDatabaseTest::vaciarTablas(); }
void ComprasTest::cleanupTestCase() { EDatabaseTest::borrarTablas(); }

#include "mcompra.h"
/*!
 * \brief ComprasTest::testComprasEtiquetaFecha
 */
void ComprasTest::testComprasEtiquetaFecha()
{
    MCompra *mc = new MCompra();
    mc->select();
    QDate fecha = mc->data( mc->index( 0, mc->fieldIndex( "fecha" ) ), Qt::EditRole ).toDate();
    QVERIFY2( mc->data( mc->index( 0, mc->fieldIndex( "fecha" ) ), Qt::DisplayRole ).toString()
              ==
              fecha.toString( Qt::SystemLocaleShortDate )
             , "Formato de fecha incorrecto" );
    delete mc;
}

QTEST_MAIN(ComprasTest)

#include "tst_comprastest.moc"
