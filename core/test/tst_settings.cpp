#include <core/settings.h>

#include <QtTest/qtest.h>

#include <chrono>

class TestSettings : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void fromString();
};

void TestSettings::fromString()
{
    const auto log_path = QStringLiteral("/data/maggui");
    const auto log_interval = std::chrono::minutes{60};
    const auto name = QString{"towcam_maggie"};
    const auto description = QString{"APS1540 installed in towcam"};
    const auto kind = maggui::SensorKind::APS1540;
    const auto connection = QString{"udp://127.0.0.0:5000:5050"};
    const auto toml = QStringLiteral(
        R"([log]
path = "%1"
interval = %2

[[sensor]]
name = "%3"
kind = "%4"
description = "%5"
connection = "%6"
)").arg(log_path, QString::number(log_interval.count()), name,  maggui::toString(kind), description, connection);
    const auto settings = Settings::fromString(toml);
    QVERIFY(settings.isValid());
    QCOMPARE(settings.dir(), log_path);
    QCOMPARE(settings.interval(), log_interval);
    const auto expected_sensors = QList<Settings::Sensor>{{name, description, connection, kind}};
    QCOMPARE(settings.sensors().count(), expected_sensors.count());
    for(auto i = 0; i < expected_sensors.count(); ++i) {
        QCOMPARE(settings.sensors().at(i)->name, expected_sensors.at(i).name);
        QCOMPARE(settings.sensors().at(i)->description, expected_sensors.at(i).description);
        QCOMPARE(settings.sensors().at(i)->connection, expected_sensors.at(i).connection);
        QCOMPARE(maggui::toString(settings.sensors().at(i)->kind), maggui::toString(expected_sensors.at(i).kind));
    }

}

QTEST_MAIN(TestSettings)
#include "tst_settings.moc"
