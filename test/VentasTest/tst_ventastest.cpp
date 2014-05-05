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
#include "formprefventa.h"
#include <unistd.h>
void VentasTest::testPreferenciaConsumidorFinalPredeterminado()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Ventas" );
    p->setValue( "siempre_cf", true );
    p->endGroup();
    p->endGroup();

    FormAgregarVenta *fav = new FormAgregarVenta();
    usleep( 9000 );
    QVERIFY( fav->CBCliente->idClienteActual() == 0 );
    //QVERIFY( fav->CBCliente->currentText() == "Consumidor Final" );
    delete fav;
    fav = 0;

    FormPrefVenta *fpv = new FormPrefVenta();
    fpv->cargar();
    QVERIFY( fpv->CkBConsumidorFinal->isChecked() );
    delete fpv;
    fpv=0;

    p->beginGroup( "Preferencias" );
    p->beginGroup( "Ventas" );
    p->setValue( "siempre_cf", false );
    p->endGroup();
    p->endGroup();

    fav = new FormAgregarVenta();
    usleep( 9000 );
    QVERIFY( fav->CBCliente->idClienteActual() == 0 );
    //QVERIFY( fav->CBCliente->currentText() == "" );
    delete fav;

    fpv = new FormPrefVenta();
    fpv->cargar();
    QVERIFY( fpv->CkBConsumidorFinal->isChecked() == false );
    delete fpv;
    fpv=0;
}

QTEST_MAIN(VentasTest)

#include "tst_ventastest.moc"
