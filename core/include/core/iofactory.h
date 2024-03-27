#ifndef IOFACTORY_H
#define IOFACTORY_H

#include <core/global.h>

#include <QtCore/qstring.h>

class QIODevice;

/// Helper class to build QIODevice objects from strings
class MAGGUI_CORE_EXPORT IOFactory
{
public:
    /// # Build a QIODevice from a string
    ///
    /// Helps provide protocol-agnostic IO interfaces using
    /// QIODevice's methods to abstract away the actual
    /// communication method (e.g. sockets, buffers, files)
    ///
    /// Works for 3 types:
    /// - QUdpSocket
    /// - QTcpSocket (as client)
    /// - QSerialPort
    ///
    /// ## QUdpSocket
    ///
    /// Config string will have the form as:
    ///
    /// "udp://$REMOTE_HOST:$REMOTE_PORT[:$LOCAL_PORT]"
    ///
    /// The local port is optional if you don't want to use the
    /// same value as the required remote argument.
    ///
    /// ## QTcpSocket
    ///
    /// Config string has the form
    ///
    /// "tcp://$REMOTE_HOST:$REMOTE_PORT"
    ///
    /// ## QSerialPort
    ///
    /// Config string has the form
    ///
    /// "serial://$PATH:$DATA$PARITY$STOP@$BAUD"
    ///
    /// which seems like a mouthful, so as an example
    /// here's the string for the typical 8 data bits,
    /// no parity, 1 stop bit, at 9600 baud on /dev/ttyUSB0:
    ///
    /// "serial:///dev/ttyUSB0:8N1@9600"
    ///
    /// Valid parameter values:
    ///
    /// - Data bits: 5, 6, 7, 8, or 9
    /// - Parity: N (none), E (even), O (odd), S (space), or M (mark)
    /// - Stop: 1, 1.5, or 2
    static auto from_string(const QString &string) -> QIODevice *;

    static auto to_string(const QIODevice *device) -> QString;
};

#endif //IOFACTORY_H
