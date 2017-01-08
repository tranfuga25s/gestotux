#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"
//#include "../../plugins/servicios/mservicios.h"

/*!
 * \brief The AdherirClienteServicio class
 *
 * Esta clase permite testear las acciones a llevar a cabo para cancelar la emisión de un
 * periodo de servicio
 */
class AdherirClienteServicioTest : public QObject, private EDatabaseTest
{
    Q_OBJECT

public:
    AdherirClienteServicioTest();
    
private Q_SLOTS:
    void initTestCase(); //  will be called before the first testfunction is executed
    void cleanupTestCase(); // will be called after the last testfunction was executed
    void init(); // will be called before each testfunction is executed
    void cleanup(); // will be called after every testfunction
    void testAdherirClienteServicio();
    void testAdherirClienteServicio_data();
};

/**
 * @brief AdherirClienteServicioTest::AdherirClienteServicioTest
 */
AdherirClienteServicioTest::AdherirClienteServicioTest()
{
    // Pongo la lista de tablas necesarias
    this->tablas << "servicios"
                 << "clientes_servicio"
                 << "clientes";

}

void AdherirClienteServicioTest::initTestCase() { EDatabaseTest::initTestCase(); }

void AdherirClienteServicioTest::init() { EDatabaseTest::init(); }

void AdherirClienteServicioTest::cleanup() { EDatabaseTest::cleanup(); }

void AdherirClienteServicioTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

/*!
 * \brief AdherirClienteServicioTest::testAdherirClienteServicioNuevoCliente
 *
 */
void AdherirClienteServicioTest::testAdherirClienteServicio()
{
    QFETCH( int, id_servicio);
    QFETCH( int, id_cliente );
    QFETCH( bool, resultado );
    MServicios *ms = new MServicios();
    bool salida = ms->asociarCliente(id_cliente, id_servicio);
    delete ms;
    QCOMPARE( salida, resultado );
}

/*!
 * \brief AdherirClienteServicioTest::testCancelarPeriodoServicio_data
 */
void AdherirClienteServicioTest::testAdherirClienteServicio_data()
{
    QTest::addColumn<int>("id_servicio");
    QTest::addColumn<int>("id_cliente");
    QTest::addColumn<bool>("resultado");
    QTest::newRow("Servicio Inexistente") << -1 << 1 << false;
    QTest::newRow("Cliente inexistente") << 1 << -1 << false;
    QTest::newRow("Correcto") << 1 << 1 << true;
    QTest::newRow("Adherido anterior") << 1 << 1 << true;
    QTest::newRow("Correcto otro servicio") << 2 << 1 << true;
}

QTEST_MAIN(AdherirClienteServicioTest)