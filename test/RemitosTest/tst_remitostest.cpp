#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"

class RemitosTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    RemitosTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testCantidadDecimalesAgregarRemito();
    void testCantidadMaxima();
};

#include "preferencias.h"
#include "formagregarremito.h"

RemitosTest::RemitosTest()
{
    this->tablas << "remito" << "item_remito";
}

void RemitosTest::initTestCase() { EDatabaseTest::initTestCase(); }
void RemitosTest::init() { EDatabaseTest::init(); }
void RemitosTest::cleanup() { EDatabaseTest::cleanup(); }
void RemitosTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

/**
 * @brief RemitosTest::testCantidadDecimales
 * Verifica que se cumpla la cantidad de decimales en el formulario de agregar compra
 */
void RemitosTest::testCantidadDecimalesAgregarRemito()
{
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->beginGroup( "Stock" );
    p->setValue( "cantidad-decimales", 4 );
    p->setValue( "mostrar-decimales", true );
    p->sync();

    FormAgregarRemito *ac = new FormAgregarRemito();
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

    ac = new FormAgregarRemito();
    QCOMPARE( ac->DSBCant->decimals(), 0 );
    delete ac;
    ac=0;
}

/*!
 * Verifica que se tenga la cantidad suficiente de valores para ingresar
 */
void RemitosTest::testCantidadMaxima()
{
    FormAgregarRemito *ac = new FormAgregarRemito();
    ac->DSBCant->setValue( 9999999.0 );
    QCOMPARE( ac->DSBCant->value(), 9999999.0 );
    delete ac;
    ac=0;
}

QTEST_MAIN(RemitosTest)

#include "tst_remitostest.moc"
