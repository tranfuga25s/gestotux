#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "edatabasetest.h"

/*!
 * \brief The CancelacionPeriodoServicioTest class
 *
 * Esta clase permite testear las acciones a llevar a cabo para cancelar la emisión de un
 * periodo de servicio
 */
class CancelacionPeriodoServicioTest : public QObject, private EDatabaseTest
{
    Q_OBJECT

public:
    CancelacionPeriodoServicioTest();
    
private Q_SLOTS:
    void initTestCase(); //  will be called before the first testfunction is executed
    void cleanupTestCase(); // will be called after the last testfunction was executed
    void init(); // will be called before each testfunction is executed
    void cleanup(); // will be called after every testfunction
    void testCancelarPeriodoServicio();
    void testCancelarPeriodoServicio_data();
};

CancelacionPeriodoServicioTest::CancelacionPeriodoServicioTest()
{
    // Pongo la lista de tablas necesarias
    this->tablas << "servicios"
                 << "periodo_servicio"
                 << "cobro_servicio_cliente_periodo"
                 << "recibos"
                 << "factura";
}

void CancelacionPeriodoServicioTest::initTestCase() { EDatabaseTest::initTestCase(); }

void CancelacionPeriodoServicioTest::init() { EDatabaseTest::init(); }

void CancelacionPeriodoServicioTest::cleanup() { EDatabaseTest::cleanup(); }

void CancelacionPeriodoServicioTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

/*!
 * \brief CancelacionPeriodoServicioTest::testCancelarPeriodoServicio
 *
 */
void CancelacionPeriodoServicioTest::testCancelarPeriodoServicio()
{
    QFETCH( int, id_servicio);
    QFETCH( int, id_periodo_servicio );
    QFETCH( bool, resultado );
    QVERIFY( resultado );
}

/*!
 * \brief CancelacionPeriodoServicioTest::testCancelarPeriodoServicio_data
 */
void CancelacionPeriodoServicioTest::testCancelarPeriodoServicio_data()
{
    QTest::addColumn<int>("id_servicio");
    QTest::addColumn<int>("id_periodo_servicio");
    QTest::addColumn<bool>("resultado");
    QTest::newRow("Correcto") << 1 << 1 << true;
}

QTEST_MAIN(CancelacionPeriodoServicioTest)

#include "tst_cancelacionperiodoserviciotest.moc"
