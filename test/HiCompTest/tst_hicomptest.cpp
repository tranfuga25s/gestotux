#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"

/**
 * @brief The HiCompTest class
 */
class HiCompTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    HiCompTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testCajaPagoReciboDiferido();
};

HiCompTest::HiCompTest()
{
    this->tablas << "recibos"
                 << "caja";
}

void HiCompTest::initTestCase() { EDatabaseTest::initTestCase(); }
void HiCompTest::init(){ EDatabaseTest::init(); }
void HiCompTest::cleanup() { EDatabaseTest::cleanup(); }
void HiCompTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

#include "eregistroplugins.h"
#include "mcajas.h"
#include "mpagos.h"
#include "DPagarRecibo.h"
/**
 * @brief HiCompTest::testCajaPagoReciboDiferido
 * Verifica que al pagar un recibo aparezca en la caja
 * Ver ticket: #159
 */
void HiCompTest::testCajaPagoReciboDiferido()
{
    /*
     * Pasos a probar
     * - Generar un recibo
     * - Marcarlo como pagado
     * - Verificar entrada en la caja
     */

    /* Busco la caja predeterminada y su saldo */
    MCajas *caja = new MCajas();
    double saldo_actual_caja = caja->saldo( caja->cajaPredeterminada() );
    double saldo_original_caja = saldo_actual_caja;

    // Genero un recibo por una cantidad X < saldo de la caja
    double total_recibo = fmod((double) rand(), saldo_original_caja);
    int id_cliente = 1; // Consumidor final
    MPagos *mp = new MPagos();
    int id_recibo = mp->agregarRecibo( id_cliente, QDate::currentDate(), "sarasa", total_recibo, true, false, "sarasa");
    QVERIFY(id_recibo > 0);

    // Genero el pago de el recibo diferido
    DPagarRecibo *d = new DPagarRecibo(0);
    d->setearIdRecibo( id_recibo );
    d->DSBPagar->setValue( total_recibo );
    d->CkBEfectivo->setChecked( true );
    d->accept();

    // Cargo el saldo de la caja
    saldo_actual_caja = caja->saldo( caja->cajaPredeterminada() );
    double nuevo_saldo = saldo_original_caja + total_recibo;

    QCOMPARE(saldo_actual_caja, nuevo_saldo);
    delete caja;
    delete mp;
    delete d;
}

QTEST_MAIN(HiCompTest)

#include "tst_hicomptest.moc"