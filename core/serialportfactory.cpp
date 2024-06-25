#include "serialportfactory.h"

#include <QPointer>
#include <QRegularExpression>
#include <QSerialPort>

namespace {
    auto error_string = QString{};
    auto to_string(QSerialPort::SerialPortError error) -> QString {
        switch (error) {
            case QSerialPort::SerialPortError::NoError:
                return QStringLiteral("NoError");
            case QSerialPort::SerialPortError::DeviceNotFoundError:
                return QStringLiteral("DeviceNotFound");
            case QSerialPort::SerialPortError::PermissionError:
                return QStringLiteral("PermissionError");
            case QSerialPort::SerialPortError::OpenError:
                return QStringLiteral("OpenError (device already opened)");
            case QSerialPort::SerialPortError::NotOpenError:
                return QStringLiteral("NotOpenError");
            case QSerialPort::SerialPortError::WriteError:
                return QStringLiteral("WriteError");
            case QSerialPort::SerialPortError::ReadError:
                return QStringLiteral("ReadError");
            case QSerialPort::SerialPortError::ResourceError:
                return QStringLiteral("ResourceError (device unavailable)");
            case QSerialPort::SerialPortError::UnsupportedOperationError:
                return QStringLiteral("UnsupportedOperationError");
            case QSerialPort::SerialPortError::TimeoutError:
                return QStringLiteral("TimeoutError");
            case QSerialPort::SerialPortError::UnknownError:
                return QStringLiteral("UnknownError");
            default:
                Q_UNREACHABLE();
        }
    }
}

const QRegularExpression SerialPortFactory::RE
    = QRegularExpression("serial://([^:]+):([5678])([NEOSM])(1|1\\.5|2)@(\\d+)",
                         QRegularExpression::CaseInsensitiveOption);

auto SerialPortFactory::from_string(const QString &string, IOFactory::ErrorKind *error) -> QSerialPort *
{
    if (!string.startsWith("serial", Qt::CaseInsensitive)) {
        if (error) {
            *error = IOFactory::ErrorKind::IncorrectKind;
        }
        error_string = QStringLiteral("");
        return nullptr;
    };

    const auto match = SerialPortFactory::RE.match(string);
    if (!match.hasMatch()) {
        if (error) {
            *error = IOFactory::ErrorKind::ConfigParseError;
        }
        error_string = QStringLiteral("Serial config string '%1' is not valid").arg(string);
        qCritical() << error_string;
        return nullptr;
    }

    const auto path = match.captured(1);
    auto ok = true;

    auto data_bits = QSerialPort::DataBits::Data8;
    switch (match.captured(2).toInt(&ok)) {
    case 8:
        data_bits = QSerialPort::DataBits::Data8;
        break;
    case 5:
        data_bits = QSerialPort::DataBits::Data5;
        break;
    case 6:
        data_bits = QSerialPort::DataBits::Data6;
        break;
    case 7:
        data_bits = QSerialPort::DataBits::Data7;
        break;
    default:
        qFatal("BUG: Unexpected serial port data size: %s", qUtf8Printable(match.captured(2)));
    }

    if (!ok) {
        qFatal("BUG: Invalid serial port data size: %s", qUtf8Printable(match.captured(2)));
    }

    const auto parity = [&](bool &ok) {
        const auto str = match.captured(3).toUpper();
        ok = true;
        if (str == QStringLiteral("N")) {
            return QSerialPort::Parity::NoParity;
        } else if (str == QStringLiteral("E")) {
            return QSerialPort::Parity::EvenParity;
        } else if (str == QStringLiteral("O")) {
            return QSerialPort::Parity::OddParity;
        } else if (str == QStringLiteral("S")) {
            return QSerialPort::Parity::SpaceParity;
        } else if (str == QStringLiteral("M")) {
            return QSerialPort::Parity::MarkParity;
        } else {
            ok = false;
            return QSerialPort::Parity::NoParity;
        }
    }(ok);

    if (!ok) {
        qFatal("BUG: Unexpected serial port parity: %s", qUtf8Printable(match.captured(3)));
    }

    const auto stop_bits = [&](bool &ok) {
        const auto str = match.captured(4);
        ok = true;
        if (str == QStringLiteral("1")) {
            return QSerialPort::StopBits::OneStop;
        } else if (str == QStringLiteral("1.5")) {
            return QSerialPort::StopBits::OneAndHalfStop;
        } else if (str == QStringLiteral("2")) {
            return QSerialPort::StopBits::TwoStop;
        } else {
            ok = false;
            return QSerialPort::StopBits::OneStop;
        }
    }(ok);
    if (!ok) {
        qFatal("BUG: Unexpected serial port stop bits: %s", qUtf8Printable(match.captured(4)));
    }

    const auto baud = match.captured(5).toInt(&ok);
    if (!ok || baud <= 0) {
        if (error) {
            *error = IOFactory::ErrorKind::ConfigParseError;
        }
        error_string = QStringLiteral("invalid baud rate: %1 in config string: %2").arg(match.captured(2), string);
        qCritical() << error_string;
        return nullptr;
    }

    auto port = QPointer<QSerialPort>{new QSerialPort};
    port->setPortName(path);

    if (!port->setParity(parity)) {
        if (error) {
            *error = IOFactory::ErrorKind::IoError;
        }
        error_string = QStringLiteral("unable to set parity to %1 on %2, %3")
                .arg(parity).arg(path).arg(::to_string(port->error()));
        qCritical() << error_string;
        return nullptr;
    }

    if (!port->setDataBits(data_bits)) {
        if (error) {
            *error = IOFactory::ErrorKind::IoError;
        }
        error_string = QStringLiteral("unable to set parity to %1 on %2, %3")
                .arg(parity)
                .arg(path)
                .arg(::to_string(port->error()));
        qCritical() << error_string;
        return nullptr;
    }

    if (!port->setStopBits(stop_bits)) {
        if (error) {
            *error = IOFactory::ErrorKind::IoError;
        }
        error_string = QStringLiteral("unable to stop bits to %1 on %2, %3")
                .arg(stop_bits)
                .arg(path)
                .arg(::to_string(port->error()));
        qCritical() << error_string;
        return nullptr;
    }

    if (!port->setBaudRate(baud)) {
        if (error) {
            *error = IOFactory::ErrorKind::IoError;
        }
        error_string = QStringLiteral("unable to change baud rate to %1 on %2, %3")
                .arg(baud)
                .arg(path)
                .arg(::to_string(port->error()));
        qCritical() << error_string;
        return nullptr;
    }

    if (!port->open(QIODeviceBase::ReadWrite)) {
        if (error) {
            *error = IOFactory::ErrorKind::IoError;
        }
        error_string = QStringLiteral("unable to open serial port %1, %2")
                .arg(path, ::to_string(port->error()));
        return nullptr;
    }

    const auto raw = port.data();
    port.clear();
    if (error) {
        *error = IOFactory::ErrorKind::NoError;
    }
    return raw;
}

auto SerialPortFactory::to_string(const QSerialPort *device) -> QString
{
    if (!device) {
        return {};
    }
    const auto data_bits = [&]() {
        switch (device->dataBits()) {
        case QSerialPort::DataBits::Data5:
            return QChar{'5'};
        case QSerialPort::DataBits::Data6:
            return QChar{'6'};
        case QSerialPort::DataBits::Data7:
            return QChar{'7'};
        case QSerialPort::DataBits::Data8:
            return QChar{'8'};
        default:
            Q_UNREACHABLE();
        }
    }();

    const auto parity = [&]() {
        switch (device->parity()) {
        case QSerialPort::Parity::NoParity:
            return QChar{'N'};
        case QSerialPort::Parity::EvenParity:
            return QChar{'E'};
        case QSerialPort::Parity::OddParity:
            return QChar{'O'};
        case QSerialPort::Parity::SpaceParity:
            return QChar{'S'};
        case QSerialPort::Parity::MarkParity:
            return QChar{'M'};
        default:
            Q_UNREACHABLE();
        }
    }();

    const auto stop_bits = [&]() {
        switch (device->stopBits()) {
        case QSerialPort::StopBits::OneStop:
            return QString{"1"};
        case QSerialPort::StopBits::OneAndHalfStop:
            return QString{"1.5"};
        case QSerialPort::StopBits::TwoStop:
            return QString{"2"};
        default:
            Q_UNREACHABLE();
        }
    }();

    return QStringLiteral("serial://%1:%2%3%4@%5")
        .arg(device->portName())
        .arg(data_bits)
        .arg(parity)
        .arg(stop_bits)
        .arg(device->baudRate());
}

auto SerialPortFactory::last_error_string() -> QString {
    const auto s = error_string;
    error_string.clear();
    return s;
}
