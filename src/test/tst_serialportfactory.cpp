#include "serialportfactory.h"

#include <QtCore/qobject.h>
#include <QtTest/qtest.h>

#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>

class TestSerialPortFactory : public QObject
{
Q_OBJECT
private:
    Q_SLOT void initTestCase()
    {
        // find a serial port we can use to test
        auto ports = QSerialPortInfo::availablePorts();
        while (!ports.isEmpty()) {
            const auto info = ports.takeAt(0);
            auto port = QSerialPort(info);
            if (port.open(QIODeviceBase::ReadWrite)) {
                port.close();
                port_info = info;
                break;
            }
        }
    }
    Q_SLOT void invalidConfigType();
    Q_SLOT void invalidConfigType_data();

    Q_SLOT void invalidConfigParameters();
    Q_SLOT void invalidConfigParameters_data();

    Q_SLOT void validConfig();
    Q_SLOT void validConfig_data();

    QSerialPortInfo port_info;
};

void TestSerialPortFactory::invalidConfigType_data()
{
    QTest::addColumn<QString>("config");

    QTest::newRow("wrong type (tcp)") << "tcp://127.0.0.1:4000";
    QTest::newRow("invalid type (udp)") << "udp://127.0.0.1:4000";
}
void TestSerialPortFactory::invalidConfigType()
{
    QFETCH(QString, config);

    const auto result = SerialPortFactory::from_string(config);
    QCOMPARE(result, nullptr);
}

void TestSerialPortFactory::invalidConfigParameters()
{
    QFETCH(QString, config);

    const auto result = SerialPortFactory::from_string(config);
    QCOMPARE(result, nullptr);
}

void TestSerialPortFactory::invalidConfigParameters_data()
{
    QTest::addColumn<QString>("config");

    QTest::newRow("invalid baud (negative)") << "serial:///dev/ttyUSB0:8N1@-9600";
    QTest::newRow("invalid baud (missing)") << "serial:///dev/ttyUSB0:8N1";
    QTest::newRow("invalid data bits (10)") << "serial:///dev/ttyUSB0:10N1@9600";
    QTest::newRow("invalid parity (Z)") << "serial:///dev/ttyUSB0:8Z1@9600";
    QTest::newRow("invalid parity ('NONE')") << "serial:///dev/ttyUSB0:8NONE1@9600";
    QTest::newRow("invalid stop bits (4)") << "serial:///dev/ttyUSB0:8N4@9600";
    QTest::newRow("invalid stop bits (missing)") << "serial:///dev/ttyUSB0:8N@9600";

}

void TestSerialPortFactory::validConfig()
{
    QFETCH(QString, config);
    QFETCH(QString, path);
    QFETCH(int, baud_rate);
    QFETCH(QSerialPort::DataBits, data_bits);
    QFETCH(QSerialPort::Parity, parity);
    QFETCH(QSerialPort::StopBits, stop_bits);

    // wrap w/ a unique_ptr to make sure it gets deleted
    const auto ptr = std::unique_ptr<QSerialPort>(SerialPortFactory::from_string(config));
    const auto result = ptr.get();

    QCOMPARE_NE(result, nullptr);
    QVERIFY(result->isOpen());
    QCOMPARE_EQ(result->portName(), path);
    QCOMPARE_EQ(result->baudRate(), baud_rate);
    QCOMPARE_EQ(result->dataBits(), data_bits);

    result->close();
}

void TestSerialPortFactory::validConfig_data()
{
    if (port_info.isNull()) {
        QSKIP("No serial ports available to test");
    }

    QTest::addColumn<QString>("config");
    QTest::addColumn<QString>("path");
    QTest::addColumn<int>("baud_rate");
    QTest::addColumn<QSerialPort::DataBits>("data_bits");
    QTest::addColumn<QSerialPort::Parity>("parity");
    QTest::addColumn<QSerialPort::StopBits>("stop_bits");

    const auto config = QStringLiteral("serial://%1:8N1@9600").arg(port_info.portName());
    QTest::newRow(qUtf8Printable(QStringLiteral("valid (%1)").arg(config)))
        << config
        << port_info.portName()
        << 9600
        << QSerialPort::DataBits::Data8
        << QSerialPort::Parity::NoParity
        << QSerialPort::StopBits::OneStop;

}


QTEST_MAIN(TestSerialPortFactory)
#include "tst_serialportfactory.moc"
