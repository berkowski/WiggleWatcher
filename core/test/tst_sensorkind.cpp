#include "../include/core/global.h"
#include <QtTest/qtest.h>

using namespace wigglewatcher;

class TestSensorKind : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void sensorKind();
    Q_SLOT void sensorKind_data();
};

void TestSensorKind::sensorKind()
{
    QFETCH(SensorKind, enum_);
    QFETCH(QString, string);
    QFETCH(bool, expected_success);


    auto ok = false;
    const auto enum_from_string = fromString(string, &ok);
    QCOMPARE(ok, expected_success);

    if (!expected_success) {
        return;
    }

    QCOMPARE_EQ(enum_from_string, enum_);

    const auto string_from_enum = toString(enum_);
    QCOMPARE_EQ(string_from_enum, string.toUpper());
}

void TestSensorKind::sensorKind_data()
{
    QTest::addColumn<SensorKind>("enum_");
    QTest::addColumn<QString>("string");
    QTest::addColumn<bool>("expected_success");

    QTest::addRow("unknown") << SensorKind::APS1540 << "unknown" << false;
    QTest::addRow("UNKNOWN") << SensorKind::APS1540 << "UNKNOWN" << false;
    QTest::addRow("APS1540") << SensorKind::APS1540 << "APS1540" << true;
    QTest::addRow("HMR2300") << SensorKind::HMR2300 << "HMR2300" << true;
}

QTEST_MAIN(TestSensorKind)
#include "tst_sensorkind.moc"
