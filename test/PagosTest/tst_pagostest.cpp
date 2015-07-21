#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"

/**
 * @brief The PagosTest class
 * Genera los tests necesarios para controlar el funcionamiento de los pagos
 */
class PagosTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    PagosTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testEvitarIngresarReciboMontoCero();
};

PagosTest::PagosTest()
{
    this->tablas << "recibos";
}

void PagosTest::initTestCase() { EDatabaseTest::initTestCase(); }
void PagosTest::init() { EDatabaseTest::init(); }
void PagosTest::cleanup() { EDatabaseTest::cleanup(); }
void PagosTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

#include "mpagos.h"
/**
 * @brief PagosTest::testEvitarIngresarReciboMontoCero
 * Verifica que no se permita agregar un recibo con monto cero
 * Ver ticket: #162
 */
void PagosTest::testEvitarIngresarReciboMontoCero()
{
    MPagos *mp = new MPagos();
    int valor = mp->agregarRecibo( 0, QDate::currentDate(), " ", 0.0, false);
    QVERIFY2(valor <= 0, "No se debería de permitir agregar un recibo con valor 0");
    delete mp;
}

QTEST_MAIN(PagosTest)

#include "tst_pagostest.moc"