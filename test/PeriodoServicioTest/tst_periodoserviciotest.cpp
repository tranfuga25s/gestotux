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
   /* void testCalcularPeriodo();
    void testCalcularPeriodo_data();
    void testRevisarPeriodo();
    void testRevisarPeriodo_data();*/

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
/*
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

    QTest::newRow("Básico") << 0 << 1 << QDate( 2014, 01, 01 ) << QDate( 2014, 01, 31 );

    int ano = QDate::currentDate().year();
    MPeriodoServicio *mp = new MPeriodoServicio();
    QSqlQuery cola;
    if( cola.exec( "SELECT id_servicio FROM servicios" ) ) {
        while( cola.next() ) {
            int id_servicio = cola.record().value(0).toInt();
            for( int i=1; i<=12; i++ ) {
                QDate fi = mp->generarFechaInicioPeriodo( id_servicio, i, ano+1 );
                QTest::newRow( QString::number( i ).toLatin1() ) << id_servicio <<  i << fi << mp->obtenerFechaFinPeriodo( id_servicio, fi );
            }
        }
    } else {
        qDebug() << "No se pudo buscar los servicios";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    delete mp;
}

void PeriodoServicioTest::testRevisarPeriodo()
{
    QFETCH( int, id_servicio );
    QFETCH( int, periodo );
    QFETCH( QDate, fi );
    QFETCH( QDate, ff );
    QCOMPARE( fi, mp->generarFechaInicioPeriodo( id_servicio, periodo, fi.year() ) );
    QCOMPARE( ff, mp->obtenerFechaFinPeriodo( id_servicio, fi ) );
}

void PeriodoServicioTest::testRevisarPeriodo_data()
{
    QSqlQuery cola;
    QSqlQuery cola2;
    int contador = 0;
    mp = new MPeriodoServicio();
    QTest::addColumn<int>( "id_servicio");
    QTest::addColumn<int>( "periodo" );
    QTest::addColumn<QDate>( "fi" );
    QTest::addColumn<QDate>( "ff" );
    if( cola.exec( "SELECT id_servicio FROM servicios" ) ) {
        while( cola.next() ) {
            int id_servicio = cola.record().value(0).toInt();
            if( cola2.exec( QString( "SELECT periodo, fecha_inicio, fecha_fin FROM periodo_servicio WHERE id_servicio = %1").arg( id_servicio ) ) ) {
                cola2.next();
                while( cola2.next() ) {
                    QTest::newRow( QString::number( contador ).toAscii() ) << id_servicio
                                                                           << cola2.record().value(0).toInt()
                                                                           << cola2.record().value(1).toDate()
                                                                           << cola2.record().value(2).toDate();
                    contador++;
                }
            } else {
                qDebug() << "Error en la cola2";
                qDebug() << cola.lastError().text();
                qDebug() << cola.lastQuery();
            }
        }
    } else {
        qDebug() << "No se pudo buscar los servicios";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
} */

QTEST_MAIN(PeriodoServicioTest)

#include "tst_periodoserviciotest.moc"
