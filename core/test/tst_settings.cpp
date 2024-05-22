#include <core/settings.h>

#include <QtTest/qtest.h>

#include <chrono>

class TestSettings : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void fromString();
    Q_SLOT void toString();
};

void TestSettings::logSection()
{
    const auto log_path = QStringLiteral("/data/maggui");
    const auto log_interval = std::chrono::minutes{60};
    const auto name = QString{"towcam_maggie"};
    const auto description = QString{"APS1540 installed in towcam"};
    const auto kind = QString{"APS1540"};
    const auto connection = QString{"udp://127.0.0.0:5000:5050"};
    const auto toml = QStringLiteral(
        R"([log]
path = "%1"
interval = %2

[[sensors]]
name = "%3"
kind = "%4"
description = "%5"
connection = "%6"
)").arg(log_path, QString::number(log_interval.count()), name, description, kind, connection);
    const auto settings = Settings::fromString(toml);
    QVERIFY(settings.isValid());
    QCOMPARE(settings.dir(), log_path);
    QCOMPARE(settings.interval(), log_interval);
}


void TestSettings::toString()
{
    const auto log_path = QStringLiteral("/data/maggui");
    const auto log_interval = std::chrono::minutes{30};
    const auto toml = QStringLiteral(R"([log]
path = "%1"
interval = %2

)").arg(log_path).arg(log_interval.count());

    const auto settings = Settings::fromString(toml);
    QVERIFY(settings.isValid());
    QCOMPARE(settings.dir(), log_path);
    QCOMPARE(settings.interval().count(), log_interval.count());

    const auto toml_output = settings.toString();
    QCOMPARE(toml, toml_output);
}

QTEST_MAIN(TestSettings)
#include "tst_settings.moc"
