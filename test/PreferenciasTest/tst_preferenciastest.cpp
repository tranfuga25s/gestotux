#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "preferencias.h"
#include "formprefdb.h"

class PreferenciasTest : public QObject
{
    Q_OBJECT
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGuardarPreferencias();

 private:
    preferencias *p;
    FormPrefDb *form;

};

void PreferenciasTest::initTestCase()
{
    p = preferencias::getInstancia();
    form = new FormPrefDb();
}

void PreferenciasTest::cleanupTestCase()
{
    p=0;
    delete form;
}

void PreferenciasTest::testGuardarPreferencias()
{
    form->cargar();
    form->CkBPuerto->setChecked( false );
    form->guardar();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "General" );
    p->beginGroup( "mysql" );
    QCOMPARE( false, p->contains( "puerto" ) );
}

QTEST_MAIN(PreferenciasTest)

#include "tst_preferenciastest.moc"

