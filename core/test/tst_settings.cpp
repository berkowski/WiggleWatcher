#include <core/settings.h>

#include <QtTest/qtest.h>

#include <chrono>

class TestSettings : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void logSection();
    Q_SLOT void toString();
};

void TestSettings::logSection()
{
    const auto log_path = QStringLiteral("/data/maggui");
    const auto log_interval = std::chrono::minutes{60};
    const auto toml = QStringLiteral(
            R"([log]
path = "%1"
interval = %2
)").arg(log_path).arg(log_interval.count());

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
