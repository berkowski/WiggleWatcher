#include "serialportfactory.h"

#include <QtCore/qpointer.h>
#include <QtCore/qregularexpression.h>
#include <QtSerialPort/qserialport.h>

const QRegularExpression SerialPortFactory::RE
    = QRegularExpression("serial://([^:]+):([5678])([NEOSM])(1|1\\.5|2)@(\\d+)",
                         QRegularExpression::CaseInsensitiveOption);

auto SerialPortFactory::from_string(const QString &string) -> QSerialPort *
{
    const auto match = SerialPortFactory::RE.match(string);
    if (!match.hasMatch()) {
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
        qCritical("invalid baud rate: %s in config string: %s",
                  qUtf8Printable(match.captured(2)),
                  qUtf8Printable(string));
        return nullptr;
    }

    auto port = QPointer<QSerialPort>{new QSerialPort};
    port->setPortName(path);

    if (!port->setParity(parity)) {
        const auto error = port->error();
        qCritical() << "unable to set parity to " << parity << " on " << path
                    << ". error: " << error;
        return nullptr;
    }

    if (!port->setDataBits(data_bits)) {
        const auto error = port->error();
        qCritical() << "unable to set data bits to " << data_bits << " on " << path
                    << ". error: " << error;
        return nullptr;
    }

    if (!port->setStopBits(stop_bits)) {
        const auto error = port->error();
        qCritical() << "unable to set stop bits to " << stop_bits << " on " << path
                    << ". error: " << error;
        return nullptr;
    }

    if (!port->setBaudRate(baud)) {
        const auto error = port->error();
        qCritical() << "unable to set baud rate to " << baud << " on " << path
                    << ". error: " << error;
        return nullptr;
    }

    if (!port->open(QIODeviceBase::ReadWrite)) {
        const auto error = port->error();
        qCritical() << "unable to open serial port " << path << ". error: " << error;
        return nullptr;
    }

    const auto raw = port.data();
    port.clear();
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
