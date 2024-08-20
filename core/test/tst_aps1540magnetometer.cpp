#include <core/aps1540magnetometer.h>

#include <QtCore/qbuffer.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qobject.h>

#include <QtTest/qsignalspy.h>
#include <QtTest/qtest.h>

using namespace wigglewatcher;

class TestAps1540Magnetometer : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void sensorType();
    Q_SLOT void stringToData();
    Q_SLOT void stringToData_data();

    Q_SLOT void handleReadyRead_with_valid_bytes();
    Q_SLOT void handleReadyRead_with_valid_bytes_data();
    Q_SLOT void handleReadyRead_with_partial();
    Q_SLOT void handleReadyRead_with_invalid_format();
};

void TestAps1540Magnetometer::sensorType()
{
    auto buffer = new QBuffer;
    auto sensor = Aps1540Magnetometer(buffer);

    QCOMPARE_EQ(sensor.type(), SensorKind::APS1540);
}

void TestAps1540Magnetometer::stringToData()
{
    QFETCH(QString, string);
    QFETCH(VectorMagnetometerData, expected);

    const auto result = Aps1540Magnetometer::stringToData(string);
    if (expected.isNull()) {
        QVERIFY(result.isNull());
    } else {
        QCOMPARE_EQ(result, expected);
    }
}

void TestAps1540Magnetometer::stringToData_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<VectorMagnetometerData>("expected");

    QTest::newRow("valid from manual")
        << "+0.2393145    +0.03288605     +0.1188259     +25.986\r\n"
        << VectorMagnetometerData{QDateTime{}, 23931.45, 3288.605, 11882.59, 25.986};
    QTest::newRow("invalid (missing field)") << "+0.2393145    +0.03288605     +0.1188259 \r\n"
                                             << VectorMagnetometerData{};
    QTest::newRow("invalid (missing separator)")
        << "+0.2393145+0.03288605     +0.1188259     +25.986\r\n"
        << VectorMagnetometerData{};
    QTest::newRow("invalid (invalid number)")
        << "+0.2393145    +0.03288605     +0.11Z8259     +25.986\r\n"
        << VectorMagnetometerData{};
}
void TestAps1540Magnetometer::handleReadyRead_with_valid_bytes()
{
    QFETCH(QByteArray, bytes);
    QFETCH(VectorMagnetometerData, expected);

    auto buffer = new QBuffer{};
    buffer->open(QBuffer::ReadWrite);
    QVERIFY(buffer->isOpen());

    //aps1540 takes ownership of buffer
    auto aps1540 = Aps1540Magnetometer{buffer};
    auto spy_raw = QSignalSpy(&aps1540, &Aps1540Magnetometer::bytesRead);
    auto spy_data = QSignalSpy(&aps1540, &Aps1540Magnetometer::valueChanged);

    QVERIFY(spy_raw.isValid());

    buffer->write(bytes);
    // need to rewind here to make data available to handler
    buffer->seek(0);

    QTRY_COMPARE_EQ(spy_raw.count(), 1);
    const auto &raw = spy_raw.at(0);
    QCOMPARE_EQ(qvariant_cast<QByteArray>(raw.at(0)), bytes);

    QTRY_COMPARE_EQ(spy_data.count(), 1);
    const auto &data = qvariant_cast<VectorMagnetometerData>(spy_data.at(0).at(0));
    QVERIFY(!data.timestamp.isNull());
    QCOMPARE_EQ(data.x, expected.x);
    QCOMPARE_EQ(data.y, expected.y);
    QCOMPARE_EQ(data.z, expected.z);
    QCOMPARE_EQ(data.temperature, expected.temperature);
}

void TestAps1540Magnetometer::handleReadyRead_with_valid_bytes_data()
{
    QTest::addColumn<QByteArray>("bytes");
    QTest::addColumn<VectorMagnetometerData>("expected");

    QTest::newRow("valid from manual")
        << QByteArray{"+0.2393145    +0.03288605     +0.1188259     +25.986\r\n"}
        << VectorMagnetometerData{QDateTime{}, 23931.45, 3288.605, 11882.59, 25.986};
}

void TestAps1540Magnetometer::handleReadyRead_with_partial()
{
    const auto bytes = QByteArray{"+0.2393145    +0.03288605     +0.1188259     +25.986\r\n"};
    const auto expected = VectorMagnetometerData{QDateTime{}, 23931.45, 3288.605, 11882.59, 25.986};
    const auto pos = bytes.size() - 5;

    auto buffer = new QBuffer{};
    buffer->open(QBuffer::ReadWrite);
    QVERIFY(buffer->isOpen());

    //aps1540 takes ownership of buffer
    auto aps1540 = Aps1540Magnetometer{buffer};
    auto spy_raw = QSignalSpy(&aps1540, &Aps1540Magnetometer::bytesRead);
    auto spy_data = QSignalSpy(&aps1540, &Aps1540Magnetometer::valueChanged);

    QVERIFY(spy_raw.isValid());

    // write a partial data string
    buffer->write(bytes.left(pos));
    buffer->seek(0);

    // nothing should happen
    QTRY_COMPARE_EQ(spy_raw.count(), 0);
    QTRY_COMPARE_EQ(spy_data.count(), 0);

    // write the rest
    buffer->seek(pos);
    buffer->write(bytes.right(bytes.size() - pos));
    buffer->seek(0);

    // signals are emitted
    QTRY_COMPARE_EQ(spy_raw.count(), 1);
    const auto &raw = spy_raw.at(0);
    QCOMPARE_EQ(qvariant_cast<QByteArray>(raw.at(0)), bytes);

    QTRY_COMPARE_EQ(spy_data.count(), 1);
    const auto &data = qvariant_cast<VectorMagnetometerData>(spy_data.at(0).at(0));
    QVERIFY(!data.timestamp.isNull());
    QCOMPARE_EQ(data.x, expected.x);
    QCOMPARE_EQ(data.y, expected.y);
    QCOMPARE_EQ(data.z, expected.z);
    QCOMPARE_EQ(data.temperature, expected.temperature);
}

// invalid format should still emit bytesRead
void TestAps1540Magnetometer::handleReadyRead_with_invalid_format()
{
    const auto bytes = QByteArray{"+0.2393145    +0.03288605     +0.1188259\r\n"};

    auto buffer = new QBuffer{};
    buffer->open(QBuffer::ReadWrite);
    QVERIFY(buffer->isOpen());

    //aps1540 takes ownership of buffer
    auto aps1540 = Aps1540Magnetometer{buffer};
    auto spy_raw = QSignalSpy(&aps1540, &Aps1540Magnetometer::bytesRead);
    auto spy_data = QSignalSpy(&aps1540, &Aps1540Magnetometer::valueChanged);

    QVERIFY(spy_raw.isValid());

    buffer->write(bytes);
    buffer->seek(0);

    // Should emit a warning
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("unable to convert string .+"));

    // signals are emitted
    QTRY_COMPARE_EQ(spy_raw.count(), 1);
    const auto &raw = spy_raw.at(0);
    QCOMPARE_EQ(qvariant_cast<QByteArray>(raw.at(0)), bytes);

    // no data is emitted
    QTRY_COMPARE_EQ(spy_data.count(), 0);
}

QTEST_MAIN(TestAps1540Magnetometer)
#include "tst_aps1540magnetometer.moc"
