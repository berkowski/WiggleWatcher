#include "../udpsocketfactory.h"

#include <chrono>
#include <QtCore/qobject.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qudpsocket.h>
#include <QtNetwork/qhostinfo.h>
#include <QtTest/qtest.h>
#include <QtTest/qsignalspy.h>

class TestUdpSocketFactory : public QObject
{
    Q_OBJECT
private:
    Q_SLOT void loopback();
    Q_SLOT void loopback_raw();
    Q_SLOT void invalidConfig();
    Q_SLOT void invalidConfig_data();

    Q_SLOT void validConfig();
    Q_SLOT void validConfig_data();
};

void TestUdpSocketFactory::loopback()
{
    auto client = QPointer<QUdpSocket>(UdpSocketFactory::from_string("udp://127.0.0.1:70000:70001"));
    auto server = QPointer<QUdpSocket>(UdpSocketFactory::from_string("udp://127.0.0.1:70001:70000"));

    QVERIFY(!client.isNull());
    QVERIFY(!server.isNull());

    auto spy_readyread_server = QSignalSpy{server, &QUdpSocket::readyRead};
    auto spy_readyread_client = QSignalSpy{client, &QUdpSocket::readyRead};

    const auto message = "some message\r\n";
    QCOMPARE(server->write(message), strlen(message));
    QTRY_COMPARE_EQ(spy_readyread_client.count(), 1);
    auto peek = client->peek(1024);
    QCOMPARE(peek.size(), strlen(message));
    QCOMPARE(peek.constData(), message);
    QVERIFY(client->canReadLine());
    QCOMPARE(client->readAll().constData(), message);

    QCOMPARE(client->write(message), strlen(message));
    QTRY_COMPARE_EQ(spy_readyread_server.count(), 1);
    peek = server->peek(1024);
    QCOMPARE(peek.size(), strlen(message));
    QCOMPARE(peek.constData(), message);
    QVERIFY(server->canReadLine());
    QCOMPARE(server->readAll().constData(), message);
}

void TestUdpSocketFactory::loopback_raw()
{
    qWarning() << "QHostInfo::localHostName(): " << QHostInfo::localHostName();
    qWarning() << "QHostInfo::fromName(\"localhost\"): " << QHostInfo::fromName("localhost").addresses();
    qWarning() << "QHostInfo::fromName(\"127.0.0.1\"): " << QHostInfo::fromName("127.0.0.1").addresses();

    auto client = QUdpSocket{};
    auto server = QUdpSocket{};

    QHostAddress server_address = QHostAddress::LocalHost;
    QVERIFY2(server.bind(0), server.errorString().toLatin1().constData());

    const auto server_port = server.localPort();
    if (!(server.localAddress() == QHostAddress::Any || server.localAddress() == QHostAddress::AnyIPv6)) {
        server_address = server.localAddress();
    }


    QHostAddress client_address = QHostAddress::LocalHost;
    QVERIFY2(client.bind(0), client.errorString().toLatin1().constData());
    const auto client_port = client.localPort();
    if (!(client.localAddress() == QHostAddress::Any || client.localAddress() == QHostAddress::AnyIPv6)) {
        client_address = client.localAddress();
    }

    server.connectToHost(client_address, client_port);
    QVERIFY(server.waitForConnected(3000));

    client.connectToHost(server_address, server_port);
    QVERIFY(client.waitForConnected(3000));

    qWarning() << "client local port: " << client_port << "remote address: " << client_address;
    qWarning() << "server local port: " << server_port << "remote address: " << server_address;

    auto spy_readyread_server = QSignalSpy{&server, &QUdpSocket::readyRead};
    auto spy_readyread_client = QSignalSpy{&client, &QUdpSocket::readyRead};

    const auto message = "some message\r\n";
    QCOMPARE(server.write(message), strlen(message));
    QTRY_COMPARE_EQ(spy_readyread_client.count(), 1);
    auto peek = client.peek(1024);
    QCOMPARE(peek.size(), strlen(message));
    QCOMPARE(peek.constData(), message);
    QVERIFY(client.canReadLine());
    QCOMPARE(client.readAll().constData(), message);

    QCOMPARE(client.write(message), strlen(message));
    QTRY_COMPARE_EQ(spy_readyread_server.count(), 1);
    peek = server.peek(1024);
    QCOMPARE(peek.size(), strlen(message));
    QCOMPARE(peek.constData(), message);
    QVERIFY(server.canReadLine());
    QCOMPARE(server.readAll().constData(), message);
}
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

    // test generation of config string
    const auto generated_config = UdpSocketFactory::to_string(result);
    QCOMPARE(generated_config, config);

    result->abort();
    delete result;
}
QTEST_MAIN(TestUdpSocketFactory)
#include "tst_udpsocketfactory.moc"
