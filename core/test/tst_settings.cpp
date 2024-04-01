#include <core/settings.h>

#include <QtTest/qtest.h>

class TestSettings : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void logRoot();
};

void TestSettings::logRoot()
{
    const auto log_root = QStringLiteral("/data/maggui");
    const auto toml = QStringLiteral(R"(log.root = "%1")").arg(log_root);

    const auto settings = Settings::fromString(toml);
    QVERIFY(settings.isValid());
    const auto result = settings.dir();
    QCOMPARE(result, log_root);
}

QTEST_MAIN(TestSettings)
#include "tst_settings.moc"
