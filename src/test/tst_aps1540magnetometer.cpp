#include "aps1540magnetometer.h"

#include <QtCore/qobject.h>
#include <QtTest/qtest.h>

auto operator<<(QDebug dbg, const VectorMagnetometerData &data) -> QDebug
{
    dbg.nospace() << "VectorMagnetometer{timestamp: "
                  << data.timestamp.toString(Qt::DateFormat::ISODateWithMs) << ", x: " << data.x
                  << ", y: " << data.y << ", z: " << data.z << ", temp: " << data.temperature
                  << "}";
    return dbg;
}

class TestAps1540Magnetometer : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void stringToData();
    Q_SLOT void stringToData_data();
};

void TestAps1540Magnetometer::stringToData()
{
    QFETCH(QString, string);
    QFETCH(VectorMagnetometerData, expected);

    const auto result = Aps1540Magnetometer::stringToData(string);
    QCOMPARE_EQ(result, expected);
}

void TestAps1540Magnetometer::stringToData_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<VectorMagnetometerData>("expected");

    QTest::newRow("valid from manual")
        << "+0.2393145    +0.03288605     +0.1188259     +25.986"
        << VectorMagnetometerData{QDateTime{}, 23931.45, 32886.05, 118826.59, 25.986};
}

QTEST_MAIN(TestAps1540Magnetometer)
#include "tst_aps1540magnetometer.moc"
