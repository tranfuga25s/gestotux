#include <QString>
#include <QtTest>
#include "../edatabasetest.h"

#include "mequipamiento.h"

class EquipamientosTest : public QObject, public EDatabaseTest
{
    Q_OBJECT

public:
    EquipamientosTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testCargaDatos();
    void testCargaDatos_data();

private:
    MEquipamiento *mequipamiento;
};


EquipamientosTest::EquipamientosTest()
{
    this->tablas << "equipamientos";
}

void EquipamientosTest::init() { EDatabaseTest::iniciarTablas(); }

void EquipamientosTest::initTestCase() {
    EDatabaseTest::generarTablas();
    mequipamiento = new MEquipamiento( this );
}

void EquipamientosTest::cleanupTestCase() {
    EDatabaseTest::borrarTablas();
    delete mequipamiento;
    mequipamiento = 0;
}

void EquipamientosTest::cleanup() { EDatabaseTest::vaciarTablas(); }

void EquipamientosTest::testCargaDatos()
{
    QFETCH( int, id_cliente );
    QFETCH( QString, descripcion );
    QFETCH( QString, marca );
    QFETCH( QString, modelo );
    QFETCH( QString, num_serie );
    QFETCH( QString, observaciones );
    QFETCH( bool, resultado );
    if( resultado == true ) {
        QVERIFY2( mequipamiento->agregarEquipamiento( id_cliente, descripcion, marca, modelo, num_serie, observaciones ), "Falla fatal" );
    } else if( resultado == false ) {
        QCOMPARE( false, mequipamiento->agregarEquipamiento( id_cliente, descripcion, marca, modelo, num_serie, observaciones ) );
    }
}

void EquipamientosTest::testCargaDatos_data()
{
    QTest::addColumn<int>("id_cliente");
    QTest::addColumn<QString>("descripcion");
    QTest::addColumn<QString>("marca");
    QTest::addColumn<QString>("modelo");
    QTest::addColumn<QString>("num_serie");
    QTest::addColumn<QString>("observaciones");
    QTest::addColumn<bool>("resultado");
    // Fila correcta
    QTest::newRow("Fila correcta") <<  0 << "Descripcion 1" << "Marca 1" << "Modelo" << "NumSerie1" << "Observaciones1" << true;
    QTest::newRow("Fila descripcion vacía") <<  0 << QString() << "Marca 1" << "Modelo" << "NumSerie1" << "Observaciones1" << false;
    /*QTest::newRow("Fila correcta") <<  0 << "Descripcion 1" << "Marca 1" << "Modelo" << "NumSerie1" << "Observaciones1" << true;
    QTest::newRow("Fila correcta") <<  0 << "Descripcion 1" << "Marca 1" << "Modelo" << "NumSerie1" << "Observaciones1" << true;
    QTest::newRow("Fila correcta") <<  0 << "Descripcion 1" << "Marca 1" << "Modelo" << "NumSerie1" << "Observaciones1" << true;
    QTest::newRow("Fila correcta") <<  0 << "Descripcion 1" << "Marca 1" << "Modelo" << "NumSerie1" << "Observaciones1" << true;*/
}

QTEST_MAIN(EquipamientosTest)

#include "tst_equipamientostest.moc"
