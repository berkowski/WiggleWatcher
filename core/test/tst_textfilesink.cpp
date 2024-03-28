#include <core/textfilesink.h>

#include <QtCore/qobject.h>
#include <QtTest/qtest.h>

class TestTextFileSink : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void nullDateTimeAlwaysLessThan();
};

void TestTextFileSink::nullDateTimeAlwaysLessThan()
{
    const auto now = QDateTime::currentDateTimeUtc();
    QVERIFY(QDateTime{} < now);
}

QTEST_MAIN(TestTextFileSink)
#include "tst_textfilesink.moc"
