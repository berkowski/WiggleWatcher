#ifndef MAGGUI_UDPSOCKETFACTORY_H
#define MAGGUI_UDPSOCKETFACTORY_H

#include <core/global.h>

#include <QRegularExpression>

class QUdpSocket;

class MAGGUI_CORE_EXPORT UdpSocketFactory
{
public:
    static const QRegularExpression RE;
    /// Construct a Udp socket from the provided config string
    ///
    /// The UDP config string contains the address and port numbers to
    /// communicate with.  It looks like:
    ///
    ///     udp://$ADDRESS:$REMOTE_PORT[:$LOCAL_PORT]
    ///
    /// where $LOCAL_PORT is optional if $REMOTE_PORT is desired to be used
    /// for the local reception as well.  For example, to send data to
    /// 192.168.0.100 port 5555 and listen for responses locally on port 5555
    /// as well, use
    ///
    ///     udp://192.168.0.100:5555
    ///
    /// If instead we want to listen locally on port 9000, then:
    ///
    ///     udp://192.168.0.100:5555:9000
    ///
    /// \param string
    /// \return
    static auto from_string(const QString &string) -> QUdpSocket *;

    static auto to_string(const QUdpSocket *device) -> QString;
};

#endif //MAGGUI_UDPSOCKETFACTORY_H
