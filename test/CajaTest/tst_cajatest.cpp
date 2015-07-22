#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"

/**
 * @brief The CajaTest class
 * @author Esteban Zeller <esteban.zeller@gmail.com>
 */
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
    void testBuscarCajaPredeterminadaInexistente();
};

/**
 * @brief CajaTest::CajaTest
 */
CajaTest::CajaTest()
{
    this->tablas << "movimiento_caja"
                 << "caja";
}

/**
 * @brief CajaTest::initTestCase
 */
void CajaTest::initTestCase() { EDatabaseTest::initTestCase(); }
/**
 * @brief CajaTest::init
 */
void CajaTest::init(){ EDatabaseTest::init(); }
/**
 * @brief CajaTest::cleanup
 */
void CajaTest::cleanup() { EDatabaseTest::cleanup(); }
/**
 * @brief CajaTest::cleanupTestCase
 */
void CajaTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

#include "mcajas.h"
/**
 * @brief CajaTest::testBuscarCajaRepetida
 */
void CajaTest::testBuscarCajaRepetida()
{
    QFETCH(QString, nombre);
    QFETCH(bool, resultado);
    MCajas *m = new MCajas();
    QCOMPARE(resultado, m->existeCaja(nombre));
    delete m;
}

/**
 * @brief CajaTest::testBuscarCajaRepetida_data
 */
void CajaTest::testBuscarCajaRepetida_data()
{
    QTest::addColumn<QString>("nombre");
    QTest::addColumn<bool>("resultado");
    QTest::newRow("Predeterminadas") << QString("Caja predeterminadas") << false;
    QTest::newRow("Predeterminada") << QString("Caja predeterminada") << true;
}

#include "preferencias.h"
/**
 * @brief CajaTest::testBuscarCajaPredeterminadaInexistente
 */
void CajaTest::testBuscarCajaPredeterminadaInexistente() {
    MCajas *mc = new MCajas();
    int caja_id = mc->cajaPredeterminada();
    QVERIFY(caja_id > 0);
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias");
    p->beginGroup( "Caja" );
    p->beginGroup( "caja-predeterminada" );
    p->setValue( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName(), caja_id+1 );
    p->endGroup();
    p->endGroup();
    p->endGroup();
    p = 0;
    QCOMPARE(caja_id, mc->cajaPredeterminada());
    delete mc;
}

QTEST_MAIN(CajaTest)

#include "tst_cajatest.moc"
