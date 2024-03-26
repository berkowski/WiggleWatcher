#include "../tcpsocketfactory.h"

#include <chrono>
#include <QtCore/qobject.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qtcpserver.h>
#include <QtTest/qtest.h>

class TestTcpSocketFactory : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void invalidConfig();
    Q_SLOT void invalidConfig_data();

    Q_SLOT void validConfig();
    Q_SLOT void validConfig_data();
};

void TestTcpSocketFactory::invalidConfig_data()
{
    QTest::addColumn<QString>("config");

    QTest::newRow("wrong type (udp)") << "udp://127.0.0.1:4000";
    QTest::newRow("wrong type (serial)") << "serial:///dev/ttyUSB0";
    QTest::newRow("invalid hostname") << "tcp://&^%*^@!Q#$:4000";
    QTest::newRow("invalid port (negative)") << "tcp://127.0.0.1:-2341";
}
void TestTcpSocketFactory::invalidConfig()
{
    QFETCH(QString, config);

    const auto result = TcpSocketFactory::from_string(config);
    QCOMPARE(result, nullptr);
}
void TestTcpSocketFactory::validConfig_data()
{
    QTest::addColumn<QString>("config");
    QTest::addColumn<QHostAddress>("host");
    QTest::addColumn<int>("port");

    QTest::newRow("loopback (127.0.0.1)")
        << "tcp://127.0.0.1:2341" << QHostAddress("127.0.0.1") << 2341;
}
void TestTcpSocketFactory::validConfig()
{
    QFETCH(QString, config);
    QFETCH(QHostAddress, host);
    QFETCH(int, port);

    auto server = QTcpServer{};
    QVERIFY(server.listen(QHostAddress::Any, port));

    const auto result = TcpSocketFactory::from_string(config);
    QTRY_COMPARE(result->waitForConnected(5000), true);
    QCOMPARE_NE(result, nullptr);
    QCOMPARE(result->peerName(), host.toString());

    // can't test port value -- only available after connection is made
    QCOMPARE(result->peerPort(), port);

    const auto generated_config = TcpSocketFactory::to_string(result);
    QCOMPARE(generated_config, config);
}
QTEST_MAIN(TestTcpSocketFactory)
#include "tst_tcpsocketfactory.moc"
