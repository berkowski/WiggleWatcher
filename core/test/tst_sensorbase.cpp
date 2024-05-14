#include "../include/core/sensorbase.h"
#include <QtTest/qtest.h>

using namespace maggui;

class TestSensorBase : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void sensorType();
    Q_SLOT void sensorType_data();
};

void TestSensorBase::sensorType()
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

void TestSensorBase::sensorType_data()
{
    QTest::addColumn<SensorKind>("enum_");
    QTest::addColumn<QString>("string");
    QTest::addColumn<bool>("expected_success");

    QTest::addRow("unknown") << SensorKind::APS1540 << "unknown" << false;
    QTest::addRow("UNKNOWN") << SensorKind::APS1540 << "UNKNOWN" << false;
    QTest::addRow("APS1540") << SensorKind::APS1540 << "APS1540" << true;
    QTest::addRow("HMR2300") << SensorKind::HMR2300 << "HMR2300" << true;
}

QTEST_MAIN(TestSensorBase)
#include "tst_sensorbase.moc"
