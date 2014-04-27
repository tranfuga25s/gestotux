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

private:
    MPeriodoServicio *mp;
};

PeriodoServicioTest::PeriodoServicioTest()
{
    this->tablas << "periodo_servicio";
}

void PeriodoServicioTest::init() { EDatabaseTest::iniciarTablas(); }

void PeriodoServicioTest::initTestCase() { EDatabaseTest::generarTablas(); }

void PeriodoServicioTest::cleanupTestCase() { EDatabaseTest::borrarTablas(); }

void PeriodoServicioTest::cleanup() { EDatabaseTest::vaciarTablas(); }

void PeriodoServicioTest::testCalcularPeriodo()
{
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
    int ano = QDate::currentDate().year();
    QSqlQuery cola( "SELECT id FROM servicios" );
    MPeriodoServicio *mp = new MPeriodoServicio();
    QTest::addColumn<int>( "id_servicio");
    QTest::addColumn<int>( "mes" );
    QTest::addColumn<QDate>( "fi" );
    QTest::addColumn<QDate>( "ff" );
    while( cola.next() ) {
        int id_servicio = cola.record().value(0).toInt();
        for( int i=1; i<=12; i++ ) {
            QDate fi = mp->generarFechaInicioPeriodo( id_servicio, i, ano+1 );
            QTest::newRow( QString::number( i ).toAscii() )<< id_servicio <<  i << fi << mp->obtenerFechaFinPeriodo( id_servicio, fi );
        }
    }
    delete mp;
}

void PeriodoServicioTest::testRevisarPeriodo()
{
    QFETCH( int, periodo );
    QFETCH( int, id_servicio );
    QFETCH( QDate, fi );
    QFETCH( QDate, ff );
    QCOMPARE( fi, mp->generarFechaInicioPeriodo( id_servicio, periodo, fi.year() ) );
    QCOMPARE( ff, mp->obtenerFechaFinPeriodo( id_servicio, fi ) );
}

void PeriodoServicioTest::testRevisarPeriodo_data()
{
    QSqlQuery cola( "SELECT id FROM servicios" );
    QSqlQuery cola2;
    int contador = 0;
    mp = new MPeriodoServicio();
    QTest::addColumn<int>( "id_servicio");
    QTest::addColumn<int>( "periodo" );
    QTest::addColumn<QDate>( "fi" );
    QTest::addColumn<QDate>( "ff" );
    while( cola.next() ) {
        int id_servicio = cola.record().value(0).toInt();
        if( cola2.exec( QString( "SELECT periodo, fecha_inicio, fecha_fin FROM periodo_servicio WHERE id_servicio = %1").arg( id_servicio ) ) ) {
            cola.next();
            while( cola.next() ) {
                QTest::newRow( QString::number( contador ).toAscii() ) << id_servicio
                                                                       << cola2.record().value(0).toInt()
                                                                       << cola2.record().value(1).toInt()
                                                                       << cola2.record().value(2).toInt();
                contador++;
            }
        }
    }
}

QTEST_MAIN(PeriodoServicioTest)

#include "tst_periodoserviciotest.moc"
