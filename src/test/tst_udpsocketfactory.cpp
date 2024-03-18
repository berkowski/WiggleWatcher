#include "udpsocketfactory.h"

#include <chrono>
#include <QtCore/qobject.h>
#include <QtTest/qtest.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qudpsocket.h>

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
    QTest::addColumn<QHostAddress>("host");
    QTest::addColumn<int>("port");

    QTest::newRow("loopback (127.0.0.1)") << "udp://127.0.0.1:2341" << QHostAddress("127.0.0.1") << 2341;
    QTest::newRow("'any' interface") << "udp://2341" << QHostAddress(QHostAddress::Any) << 2341;
}
void TestUdpSocketFactory::validConfig()
{
    QFETCH(QString, config);
    QFETCH(QHostAddress, host);
    QFETCH(int, port);

    const auto result = UdpSocketFactory::from_string(config);
    QCOMPARE_NE(result, nullptr);
    QCOMPARE_EQ(result->localAddress().toString(), host.toString());
    QCOMPARE_EQ(result->localPort(), port);
}
QTEST_MAIN(TestUdpSocketFactory)
#include "tst_udpsocketfactory.moc"
