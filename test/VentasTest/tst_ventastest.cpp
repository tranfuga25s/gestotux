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
    this->tablas << "clientes"
                 << "factura"
                 << "item_factura";
}

void VentasTest::init() { EDatabaseTest::iniciarTablas(); }

void VentasTest::initTestCase() { EDatabaseTest::generarTablas(); }

void VentasTest::cleanupTestCase() { EDatabaseTest::borrarTablas(); }

void VentasTest::cleanup() { EDatabaseTest::vaciarTablas(); }

#include "preferencias.h"
#include "formagregarventa.h"
void VentasTest::testPreferenciaConsumidorFinalPredeterminado()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Ventas" );
    p->setValue( "siempre_cf", true );
    p->endGroup();

    FormAgregarVenta *fav = new FormAgregarVenta();
    QVERIFY( fav->CBCliente->idClienteActual() == 0 );
    QVERIFY( fav->CBCliente->currentText() == "Consumidor Final" );
    delete fav;
    fav = 0;

    p->beginGroup( "Ventas" );
    p->setValue( "siempre_cf", false );
    p->endGroup();

    fav = new FormAgregarVenta();
    QVERIFY( fav->CBCliente->idClienteActual() == 0 );
    QVERIFY( fav->CBCliente->currentText() == "" );
    delete fav;
}

QTEST_MAIN(VentasTest)

#include "tst_ventastest.moc"
