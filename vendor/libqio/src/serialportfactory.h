#ifndef SERIALPORTFACTORY_H
#define SERIALPORTFACTORY_H

//#include <core/global.h>
#include <libqio/iofactory.h>

#include <QString>
#include <QSerialPort>

class QSerialPort;

class LIBQIO_EXPORT SerialPortFactory
{
public:
    static const QRegularExpression RE;
    /// Construct a QSerialPort using parameters defined in a string
    ///
    ///
    /// The config string takes the form of:
    ///
    ///      serial://<path>:<data-bits><parity><stop-bits>
    ///
    /// where:
    /// - <path> is the serial port name (e.g. 'COM6' or '/dev/ttyUSB1')
    /// - <data-bits> is one of 7, 8, or 9
    /// - <parity> is one of N, E, O, S, or M for None, Even, odd, Space or Mark
    /// - <stop-bits> is one of 1, 1.5, or 2
    ///
    /// This function will return a nullptr if there are issues decoding the config string or
    /// opening the serial port.
    ///
    /// If the config string does not represent a serial port (e.g. doesn't start with 'serial://') this
    /// function returns a nullptr and sets error to QSerialPort::SerialPortError::NoError.  It the
    /// config string represents invalid serial port parameters (e.g. 10 data bits) the fuction
    /// returns a nullptr and sets 'error' to QSerialPort::SerialPortError::UnknownError.
    ///
    /// \param string port parameters
    /// \param error error
    /// \return QSerialPort pointer, or nullptr on error
    static auto from_string(const QString &string, IOFactory::ErrorKind *error= nullptr) -> QSerialPort *;
    static auto to_string(const QSerialPort *device) -> QString;
    static auto last_error_string() -> QString;
};

#endif //SERIALPORTFACTORY_H
