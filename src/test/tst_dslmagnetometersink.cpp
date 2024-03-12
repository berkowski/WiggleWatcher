#include "dslmagnetometersink.h"

#include <chrono>
#include <QtCore/qobject.h>
#include <QtTest/qtest.h>

class TestDslMagnetometerSink : public QObject
{
    Q_OBJECT

private:
    Q_SLOT void firstRecord();
};

void TestDslMagnetometerSink::firstRecord()
{
    const auto path = QDir::temp();
    const auto prefix = QStringLiteral("prefix");
    const auto record = VectorMagnetometerData{.timestamp = QDateTime::currentDateTimeUtc(),
                                               .x = 0,
                                               .y = 0,
                                               .z = 0,
                                               .temperature = 0.0};

    const auto expected_filename = path.filePath(
        QStringLiteral("%1_%2.dat")
            .arg(prefix)
            .arg(record.timestamp.toString(DslMagnetometerSink::FILENAME_DATETIME_FMT)));

    auto sink = DslMagnetometerSink{path, prefix};
    QVERIFY2(!QFileInfo::exists(expected_filename),
             qPrintable(
                 QStringLiteral("did not expect %1 to exist already").arg(expected_filename)));
    sink.write(record);
    QVERIFY2(QFileInfo::exists(expected_filename),
             qPrintable(QStringLiteral("Expected %1 to exist after writing record")
                            .arg(expected_filename)));
}

QTEST_MAIN(TestDslMagnetometerSink)
#include "tst_dslmagnetometersink.moc"
