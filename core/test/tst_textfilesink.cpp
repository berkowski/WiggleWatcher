#include <core/textfilesink.h>

#include <QtCore/qobject.h>
#include <QtTest/qtest.h>
#include <QtTest/qsignalspy.h>

class TestTextFileSink : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void nullDateTimeAlwaysLessThan();
    Q_SLOT void initialState();
    Q_SLOT void writeWhileStopped();
    Q_SLOT void callingStartWritesNoData();
};


void TestTextFileSink::nullDateTimeAlwaysLessThan()
{
    const auto now = QDateTime::currentDateTimeUtc();
    QVERIFY(QDateTime{} < now);
}

void TestTextFileSink::initialState()
{
    const auto dir = QDir::temp();
    auto sink = TextFileSink{dir};
    QVERIFY(!sink.isActive());
    QCOMPARE(sink.dir(), dir);
    QVERIFY(sink.currentFileName().isEmpty());
    QVERIFY(sink.timeUntilRollover() < 0);
}

void TestTextFileSink::writeWhileStopped() {
    auto sink = TextFileSink{};
    QVERIFY(!sink.isActive());
    sink.setHeader("# Some header line");

    auto spy = QSignalSpy{&sink, &TextFileSink::bytesWritten};
    QVERIFY(spy.isValid());
    const auto bytes = QStringLiteral("some data").toUtf8();
    sink.write(bytes);
    QCOMPARE_EQ(spy.count(), 0);
}

void TestTextFileSink::callingStartWritesNoData()
{

    auto sink = TextFileSink{};
    const auto header_lines = QStringList {"# First header line", "# Second header line"};
    sink.setHeader(header_lines);
    QVERIFY(!sink.isActive());

    auto filename_spy = QSignalSpy{&sink, &TextFileSink::fileNameChanged};
    auto nbytes_spy = QSignalSpy{&sink, &TextFileSink::bytesWritten};
    // no current filename
    QVERIFY(sink.currentFileName().isEmpty());


    const auto now = QDateTime::currentDateTimeUtc();
    sink.start();

    // Time until next rollover is now non-negative
    QVERIFY(sink.timeUntilRollover() >= 0);

    // current filename updated to expected value
    const auto expected_filename = sink.filenameForDateTime(now);
    const auto filename = sink.currentFileName();
    QCOMPARE(filename, expected_filename);

    // filename changed signal was emitted
    QCOMPARE(filename_spy.size(), 1);
    QCOMPARE(filename_spy.at(0).at(0), expected_filename);

    // but no bytes have been written.
    QCOMPARE(nbytes_spy.size(), 0);

    const auto file_info = QFileInfo{filename};

    // File does not exist yet (has not been written to)
    QVERIFY(!file_info.exists());

    // but has no data
    QCOMPARE(file_info.size(), 0);
}

QTEST_MAIN(TestTextFileSink)
#include "tst_textfilesink.moc"
