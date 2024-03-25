#include "sensorbase.h"
#include <QtTest/qtest.h>

class TestSensorBase : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void sensorType();
    Q_SLOT void sensorType_data();
};

void TestSensorBase::sensorType()
{
    QFETCH(SensorBase::SensorType, enum_);
    QFETCH(QString, string);

    const auto enum_from_string = fromString(string);
    QCOMPARE_EQ(enum_from_string, enum_);

    const auto string_from_enum = toString(enum_);
    QCOMPARE_EQ(string_from_enum, string.toUpper());
}

void TestSensorBase::sensorType_data()
{
    QTest::addColumn<SensorBase::SensorType>("enum_");
    QTest::addColumn<QString>("string");

    QTest::addRow("unknown") << SensorBase::SensorType::UNKNOWN << "unknown";
    QTest::addRow("UNKNOWN") << SensorBase::SensorType::UNKNOWN << "UNKNOWN";
    QTest::addRow("APS1540") << SensorBase::SensorType::APS1540 << "APS1540";
    QTest::addRow("HMR2300") << SensorBase::SensorType::HMR2300 << "HMR2300";
    QTest::addRow("USER_TYPE") << SensorBase::SensorType::USER_TYPE << "USER_TYPE";
}

QTEST_MAIN(TestSensorBase)
#include "tst_sensorbase.moc"
