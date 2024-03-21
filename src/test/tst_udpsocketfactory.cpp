#include "udpsocketfactory.h"

#include <chrono>
#include <QtCore/qobject.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qudpsocket.h>
#include <QtTest/qtest.h>

class TestUdpSocketFactory : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void invalidConfig();
    Q_SLOT void invalidConfig_data();

    Q_SLOT void validConfig();
    Q_SLOT void validConfig_data();
};

void TestUdpSocketFactory::invalidConfig_data()
{
    QTest::addColumn<QString>("config");

    QTest::newRow("wrong type (tcp)") << "tcp://127.0.0.1:4000";
    QTest::newRow("wrong type (serial)") << "serial:///dev/ttyUSB0";
    QTest::newRow("invalid hostname") << "udp://&^%*^@!Q#$:4000";
    QTest::newRow("invalid port (negative)") << "tcp://127.0.0.1:-2341";
    QTest::newRow("missing hostname") << "udp://2341";
}
void TestUdpSocketFactory::invalidConfig()
{
    QFETCH(QString, config);

    const auto result = UdpSocketFactory::from_string(config);
    QVERIFY(result == nullptr);
}
void TestUdpSocketFactory::validConfig_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<QHostAddress>("address");
    QTest::addColumn<int>("remote_port");
    QTest::addColumn<int>("local_port");

    QTest::newRow("same port on local")
        << "udp://127.0.0.1:2341" << QHostAddress("127.0.0.1") << 2341 << 2341;
    QTest::newRow("different rx port")
        << "udp://127.0.0.1:2341:5555" << QHostAddress("127.0.0.1") << 2341 << 5555;
}

void TestUdpSocketFactory::validConfig()
{
    QFETCH(QString, config);
    QFETCH(QHostAddress, address);
    QFETCH(int, remote_port);
    QFETCH(int, local_port);

    const auto result = UdpSocketFactory::from_string(config);
    QCOMPARE_NE(result, nullptr);
    QVERIFY(result->peerAddress().isEqual(address, QHostAddress::TolerantConversion));
    QCOMPARE_EQ(result->peerPort(), remote_port);
    QCOMPARE_EQ(result->localPort(), local_port);
    result->abort();
    delete result;
}
QTEST_MAIN(TestUdpSocketFactory)
#include "tst_udpsocketfactory.moc"
