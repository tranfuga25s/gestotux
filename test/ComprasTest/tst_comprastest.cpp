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
    void testAgregadoStockInicial();
};

ComprasTest::ComprasTest()
{
    this->tablas << "compras" << "compras_productos" << "productos";
}
void ComprasTest::initTestCase() { EDatabaseTest::generarTablas(); }
void ComprasTest::init() { EDatabaseTest::iniciarTablas(); }
void ComprasTest::cleanup() { EDatabaseTest::vaciarTablas(); }
void ComprasTest::cleanupTestCase() { EDatabaseTest::borrarTablas(); }

void ComprasTest::testAgregadoStockInicial()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(ComprasTest)

#include "tst_comprastest.moc"
