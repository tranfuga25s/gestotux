#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../../plugins/servicios/mperiodoservicio.h"
#include <QSqlDatabase>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include "../edatabasetest.h"

class PeriodoServicioTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    PeriodoServicioTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testCalcularPeriodo();
    void testCalcularPeriodo_data();
    void testRevisarPeriodo();
    void testRevisarPeriodo_data();
    void testRevisarPeriodoInicioIntermedio();
    void testRevisarPeriodoInicioIntermedio_data();

private:
    MPeriodoServicio *mp;
};

PeriodoServicioTest::PeriodoServicioTest()
{
    this->tablas << "periodo_servicio";
}

void PeriodoServicioTest::init() { EDatabaseTest::init(); }

void PeriodoServicioTest::initTestCase() { EDatabaseTest::initTestCase(); }

void PeriodoServicioTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

void PeriodoServicioTest::cleanup() { EDatabaseTest::cleanup(); }

void PeriodoServicioTest::testCalcularPeriodo()
{
    //QFETCH( int, id_servicio );
    QFETCH( int, mes );
    QFETCH( QDate, fi );
    QFETCH( QDate, ff );
    QDate fecha;
    fecha.setDate( fi.year(), mes, 1 );
    QCOMPARE( fi, fecha );
    fecha.setDate( fecha.year(), fecha.month(), fecha.daysInMonth() );
    QCOMPARE( ff, fecha );
}

void PeriodoServicioTest::testCalcularPeriodo_data()
{

    QTest::addColumn<int>( "id_servicio");
    QTest::addColumn<int>( "mes" );
    QTest::addColumn<QDate>( "fi" );
    QTest::addColumn<QDate>( "ff" );

    QTest::newRow("Enero")      << 0 << 1  << QDate( 2015, 1,  1 ) << QDate( 2015, 1, 31 );
    QTest::newRow("Febrero")    << 0 << 2  << QDate( 2015, 2,  1 ) << QDate( 2015, 2, 28 );
    QTest::newRow("Marzo")      << 0 << 3  << QDate( 2015, 3,  1 ) << QDate( 2015, 3, 31 );
    QTest::newRow("Abril")      << 0 << 4  << QDate( 2015, 4,  1 ) << QDate( 2015, 4, 30 );
    QTest::newRow("Mayo")       << 0 << 5  << QDate( 2015, 5,  1 ) << QDate( 2015, 5, 31 );
    QTest::newRow("Junio")      << 0 << 6  << QDate( 2015, 6,  1 ) << QDate( 2015, 6, 30 );
    QTest::newRow("Julio")      << 0 << 7  << QDate( 2015, 7,  1 ) << QDate( 2015, 7, 31 );
    QTest::newRow("Agosto")     << 0 << 8  << QDate( 2015, 8,  1 ) << QDate( 2015, 8, 31 );
    QTest::newRow("Septiembre") << 0 << 9  << QDate( 2015, 9,  1 ) << QDate( 2015, 9, 30 );
    QTest::newRow("Octubre")    << 0 << 10 << QDate( 2015, 10, 1 ) << QDate( 2015, 10, 31 );
    QTest::newRow("Noviembre")  << 0 << 11 << QDate( 2015, 11, 1 ) << QDate( 2015, 11, 30 );
    QTest::newRow("Diciembre")  << 0 << 12 << QDate( 2015, 12, 1 ) << QDate( 2015, 12, 31 );
}

/**
 * @brief PeriodoServicioTest::testRevisarPeriodo
 */
void PeriodoServicioTest::testRevisarPeriodo()
{
    QFETCH( int, id_servicio );
    QFETCH( int, periodo );
    QFETCH( QDate, fi );
    QFETCH( QDate, ff );
    QCOMPARE( fi, mp->generarFechaInicioPeriodo( id_servicio, periodo, fi.year() ) );
    QCOMPARE( ff, mp->obtenerFechaFinPeriodo( id_servicio, fi ) );
}

/**
 * @brief PeriodoServicioTest::testRevisarPeriodo_data
 */
void PeriodoServicioTest::testRevisarPeriodo_data()
{
    QTest::addColumn<int>( "id_servicio");
    QTest::addColumn<int>( "periodo" );
    QTest::addColumn<QDate>( "fi" );
    QTest::addColumn<QDate>( "ff" );
    QTest::newRow("Enero")      << 0 << 1  << QDate( 2015, 1,  1 ) << QDate( 2015, 1, 31 );
    QTest::newRow("Febrero")    << 0 << 2  << QDate( 2015, 2,  1 ) << QDate( 2015, 2, 28 );
    QTest::newRow("Marzo")      << 0 << 3  << QDate( 2015, 3,  1 ) << QDate( 2015, 3, 31 );
    QTest::newRow("Abril")      << 0 << 4  << QDate( 2015, 4,  1 ) << QDate( 2015, 4, 30 );
    QTest::newRow("Mayo")       << 0 << 5  << QDate( 2015, 5,  1 ) << QDate( 2015, 5, 31 );
    QTest::newRow("Junio")      << 0 << 6  << QDate( 2015, 6,  1 ) << QDate( 2015, 6, 30 );
    QTest::newRow("Julio")      << 0 << 7  << QDate( 2015, 7,  1 ) << QDate( 2015, 7, 31 );
    QTest::newRow("Agosto")     << 0 << 8  << QDate( 2015, 8,  1 ) << QDate( 2015, 8, 31 );
    QTest::newRow("Septiembre") << 0 << 9  << QDate( 2015, 9,  1 ) << QDate( 2015, 9, 30 );
    QTest::newRow("Octubre")    << 0 << 10 << QDate( 2015, 10, 1 ) << QDate( 2015, 10, 31 );
    QTest::newRow("Noviembre")  << 0 << 11 << QDate( 2015, 11, 1 ) << QDate( 2015, 11, 30 );
    QTest::newRow("Diciembre")  << 0 << 12 << QDate( 2015, 12, 1 ) << QDate( 2015, 12, 31 );
}


/**
 * @brief PeriodoServicioTest::testRevisarPeriodoInicioIntermedio
 */
void PeriodoServicioTest::testRevisarPeriodoInicioIntermedio()
{
    QFETCH( int, id_servicio );
    QFETCH( int, periodo );
    QFETCH( QDate, fi );
    QFETCH( QDate, ff );
    QCOMPARE( fi, mp->getFechaInicioPeriodo( id_servicio, periodo, fi.year() ) );
    QCOMPARE( ff, mp->obtenerFechaFinPeriodo( id_servicio, fi ) );
    QCOMPARE( periodo, mp->getPeriodoSegunFecha( id_servicio, fi ) );
}

/**
 * @brief PeriodoServicioTest::testRevisarPeriodoInicioIntermedio_data
 */
void PeriodoServicioTest::testRevisarPeriodoInicioIntermedio_data()
{
    QTest::addColumn<int>( "id_servicio");
    QTest::addColumn<int>( "periodo" );
    QTest::addColumn<QDate>( "fi" );
    QTest::addColumn<QDate>( "ff" );
    QTest::newRow("Enero")      << 1 << 1  << QDate( 2015, 1,  6 ) << QDate( 2015, 1, 30 );
    QTest::newRow("Febrero")    << 0 << 2  << QDate( 2015, 2,  1 ) << QDate( 2015, 2, 28 );
    QTest::newRow("Marzo")      << 0 << 3  << QDate( 2015, 3,  1 ) << QDate( 2015, 3, 31 );
    QTest::newRow("Abril")      << 0 << 4  << QDate( 2015, 4,  1 ) << QDate( 2015, 4, 30 );
    QTest::newRow("Mayo")       << 0 << 5  << QDate( 2015, 5,  1 ) << QDate( 2015, 5, 31 );
    QTest::newRow("Junio")      << 0 << 6  << QDate( 2015, 6,  1 ) << QDate( 2015, 6, 30 );
    QTest::newRow("Julio")      << 0 << 7  << QDate( 2015, 7,  1 ) << QDate( 2015, 7, 31 );
    QTest::newRow("Agosto")     << 0 << 8  << QDate( 2015, 8,  1 ) << QDate( 2015, 8, 31 );
    QTest::newRow("Septiembre") << 0 << 9  << QDate( 2015, 9,  1 ) << QDate( 2015, 9, 30 );
    QTest::newRow("Octubre")    << 0 << 10 << QDate( 2015, 10, 1 ) << QDate( 2015, 10, 31 );
    QTest::newRow("Noviembre")  << 0 << 11 << QDate( 2015, 11, 1 ) << QDate( 2015, 11, 30 );
    QTest::newRow("Diciembre")  << 0 << 12 << QDate( 2015, 12, 1 ) << QDate( 2015, 12, 31 );
}


QTEST_MAIN(PeriodoServicioTest)

#include "tst_periodoserviciotest.moc"
