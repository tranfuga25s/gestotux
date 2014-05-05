#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"

class VentasTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    VentasTest();
    
private Q_SLOTS:
    void init();
    void initTestCase();
    void cleanupTestCase();
    void cleanup();
    void testPreferenciaConsumidorFinalPredeterminado();
};

VentasTest::VentasTest()
{
    this->tablas << "factura" << "item_factura";
}

void VentasTest::init() { EDatabaseTest::iniciarTablas(); }

void VentasTest::initTestCase() { EDatabaseTest::generarTablas(); }

void VentasTest::cleanupTestCase() { EDatabaseTest::borrarTablas(); }

void VentasTest::cleanup() { EDatabaseTest::vaciarTablas(); }

void VentasTest::testPreferenciaConsumidorFinalPredeterminado()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(VentasTest)

#include "tst_ventastest.moc"
