#include "dslmagnetometersink.h"

#include <QtTest/qtest.h>
#include <QtCore/qobject.h>
#include <chrono>

class TestDslMagnetometerSink : public QObject {
  Q_OBJECT

private:
  Q_SLOT void rolloverTime_data();
  Q_SLOT void rolloverTime();
};

void TestDslMagnetometerSink::rolloverTime_data() {
    QTest::addColumn<QDateTime>("current");
    QTest::addColumn<int>("duration");
    QTest::addColumn<QDateTime>("expected");

    QTest::newRow("top-of-the-hour")
    << QDateTime(QDate(2024, 2, 16), QTime(12, 34, 23))
    << 3600 * 1000
    << QDateTime(QDate(2024,2, 16), QTime(13, 0, 0));
}

void TestDslMagnetometerSink::rolloverTime() {
  QFETCH(QDateTime, current);
  QFETCH(int, duration);
  QFETCH(QDateTime, expected);

  const auto current_ms = std::chrono::milliseconds{current.toMSecsSinceEpoch()};
  const auto duration_ms = std::chrono::milliseconds{duration};
  const auto expected_ms =
    std::chrono::milliseconds{expected.toMSecsSinceEpoch()};
  const auto result_ms =
    DslMagnetometerSink::nextRolloverMS(current_ms, duration_ms);
  QCOMPARE(result_ms.count(), expected_ms.count());
}

QTEST_MAIN(TestDslMagnetometerSink)
#include "tst_dslmagnetometersink.moc"
