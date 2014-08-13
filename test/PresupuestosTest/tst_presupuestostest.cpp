#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"

class PresupuestosTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    PresupuestosTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testCantidadDecimalesAgregarPresupuesto();
    void testCantidadMaxima();
};

#include "preferencias.h"
#include "formagregarpresupuesto.h"

PresupuestosTest::PresupuestosTest()
{
    this->tablas << "presupuestos" << "item_presupuesto";
}

void PresupuestosTest::initTestCase() { EDatabaseTest::initTestCase(); }
void PresupuestosTest::init() { EDatabaseTest::init(); }
void PresupuestosTest::cleanup() { EDatabaseTest::cleanup(); }
void PresupuestosTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

/**
 * @brief PresupuestosTest::testCantidadDecimales
 * Verifica que se cumpla la cantidad de decimales en el formulario de agregar compra
 */
void PresupuestosTest::testCantidadDecimalesAgregarPresupuesto()
{
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->beginGroup( "Stock" );
    p->setValue( "cantidad-decimales", 4 );
    p->setValue( "mostrar-decimales", true );
    p->sync();

    FormAgregarPresupuesto *ac = new FormAgregarPresupuesto();
    QCOMPARE( ac->DSBCant->decimals(), 4 );
    delete ac;
    ac=0;

    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->beginGroup( "Stock" );
    p->setValue( "cantidad-decimales", 4 );
    p->setValue( "mostrar-decimales", false );
    p->sync();

    ac = new FormAgregarPresupuesto();
    QCOMPARE( ac->DSBCant->decimals(), 0 );
    delete ac;
    ac=0;
}

/*!
 * Verifica que se tenga la cantidad suficiente de valores para ingresar
 */
void PresupuestosTest::testCantidadMaxima()
{
    FormAgregarPresupuesto *ac = new FormAgregarPresupuesto();
    ac->DSBCant->setValue( 9999999.0 );
    QCOMPARE( ac->DSBCant->value(), 9999999.0 );
    delete ac;
    ac=0;
}

QTEST_MAIN(PresupuestosTest)

#include "tst_presupuestostest.moc"
