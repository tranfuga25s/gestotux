#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "edatabasetest.h"

class BackupTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    BackupTest();
    
private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

BackupTest::BackupTest()
{
    this->tablas << "compras"
                 << "compras_productos";
}

void BackupTest::init() { EDatabaseTest::init(); }

void BackupTest::initTestCase() { EDatabaseTest::initTestCase(); }

void BackupTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

void BackupTest::cleanup() { EDatabaseTest::cleanup(); }

void BackupTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void BackupTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(BackupTest)

#include "tst_backuptest.moc"
