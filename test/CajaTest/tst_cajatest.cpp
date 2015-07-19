#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"


class CajaTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    CajaTest();
    
private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void testBuscarCajaRepetida();
    void testBuscarCajaRepetida_data();
};

CajaTest::CajaTest()
{
    this->tablas << "movimiento_caja"
                 << "caja";
}

void CajaTest::initTestCase() { EDatabaseTest::initTestCase(); }
void CajaTest::init(){ EDatabaseTest::init(); }
void CajaTest::cleanup() { EDatabaseTest::cleanup(); }
void CajaTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

#include "mcajas.h"
void CajaTest::testBuscarCajaRepetida()
{
    QFETCH(QString, nombre);
    QFETCH(bool, resultado);
    MCajas *m = new MCajas();
    QCOMPARE(resultado, m->existeCaja(nombre));
    delete m;
}

void CajaTest::testBuscarCajaRepetida_data()
{
    QTest::addColumn<QString>("nombre");
    QTest::addColumn<bool>("resultado");
    QTest::newRow("Predeterminada s") << QString("Caja Predeterminadas") << false;
    QTest::newRow("Predeterminada") << QString("Caja Predeterminada") << true;
}

QTEST_MAIN(CajaTest)

#include "tst_cajatest.moc"
