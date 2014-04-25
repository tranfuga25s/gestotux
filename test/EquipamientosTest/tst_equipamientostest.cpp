#include <QString>
#include <QtTest>
#include "../edatabasetest.h"

#include "mequipamiento.h"

class EquipamientosTest : public EDatabaseTest, public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCargaDatos();
    void testCargaDatos_data();

private:
    MEquipamiento *mequipamiento;
};

void EquipamientosTest::initTestCase()
{
    mequipamiento = new MEquipamiento( this );
}

void EquipamientosTest::cleanupTestCase()
{
    delete mequipamiento;
    mequipamiento = 0;
}

void EquipamientosTest::testCargaDatos()
{
    QFETCH( int, id_cliente );
    QFETCH( QString, descripcion );
    QFETCH( QString, marca );
    QFETCH( QString, modelo );
    QFETCH( QString, num_serie );
    QFETCH( QString, observaciones );
    QFETCH( bool, resultado );
    QCOMPARE( resultado, mequipamiento->agregarEquipamiento( id_cliente, descripcion, marca, modelo, num_serie, observaciones ) );
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
    QTest::newRow("Fila descripcion vacía") <<  0 << QString() << "Marca 1" << "Modelo" << "NumSerie1" << "Observaciones1" << true;
    /*QTest::newRow("Fila correcta") <<  0 << "Descripcion 1" << "Marca 1" << "Modelo" << "NumSerie1" << "Observaciones1" << true;
    QTest::newRow("Fila correcta") <<  0 << "Descripcion 1" << "Marca 1" << "Modelo" << "NumSerie1" << "Observaciones1" << true;
    QTest::newRow("Fila correcta") <<  0 << "Descripcion 1" << "Marca 1" << "Modelo" << "NumSerie1" << "Observaciones1" << true;
    QTest::newRow("Fila correcta") <<  0 << "Descripcion 1" << "Marca 1" << "Modelo" << "NumSerie1" << "Observaciones1" << true;*/
}

QTEST_APPLESS_MAIN(EquipamientosTest)

#include "tst_equipamientostest.moc"
