#include "../core/include/core/iofactory.h"

#include <QtCore/qcommandlineparser.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qfile.h>
#include <QtCore/qmath.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qthread.h>

#include <cstdio>

using namespace Qt::Literals::StringLiterals;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("fake-sensor");
    QCoreApplication::setApplicationVersion("0.1");

    auto parser = QCommandLineParser{};
    parser.setApplicationDescription("Generate fake sensor data for testing");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
        {{"q", "quiet"}, QCoreApplication::translate("main", "don't print output to stdout")},
        {{"r", "rate"}, QCoreApplication::translate("main", "update rate"), "HZ", "8.0"},
    });

    parser.addPositionalArgument(
        "output",
        QCoreApplication::translate("main", "output config: [udp://.., serial://..]"),
        "output");

    parser.process(app);

    const auto args = parser.positionalArguments();

    if (args.isEmpty()) {
        std::fputs(qPrintable(u"Missing required argument 'output'\r\n"_s), stderr);
        std::fputs(qPrintable(parser.helpText()), stderr);
        return 1;
    }

    auto io_device = IOFactory::from_string(args.at(0));
    if (!io_device) {
        std::fputs(qPrintable(u"Unable to parse output config: '"_s), stderr);
        std::fputs(qPrintable(args.at(0)), stderr);
        std::fputs(qPrintable(u"'\r\n"_s), stderr);
        std::fputs(qPrintable(parser.helpText()), stderr);
        return 1;
    }

    const auto rate_hz = [&]() -> double {
        auto ok = true;
        const auto value = parser.value("rate").toDouble(&ok);
        if (!ok) {
            qCritical() << "invalid value for rate: " << parser.value("rate");
            return -1.0;
        } else {
            return value;
        }
    }();

    auto stdout_io = static_cast<QFile *>(nullptr);
    if (!parser.isSet("quiet")) {
        stdout_io = new QFile;
        if (!stdout_io->open(stdout, QIODevice::WriteOnly | QIODevice::Unbuffered)) {
            qFatal() << "unable to open stdout for writing";
        }
    }

    auto io = QThread::create([&]() -> void {
        const auto TWO_M_PI = 2 * M_PI;
        //const auto rate_hz = 20.0;
        auto phase_rad = 0.0;
        const auto frequency_rad_s = TWO_M_PI * 1.0 / 60.0;

        const auto A = 1.0;
        const auto xp_offset = 0.0;
        const auto yp_offset = 120.0;
        const auto zp_offset = 240.0;

        auto t_offset = 25.0;

        auto buffer = QString{};
        auto stream = QTextStream{&buffer};
        stream.setNumberFlags(QTextStream::ForceSign);
        stream.setRealNumberNotation(QTextStream::FixedNotation);

        while (1) {
            buffer.clear();
            phase_rad += TWO_M_PI * frequency_rad_s / rate_hz;
            while (phase_rad > TWO_M_PI) {
                phase_rad -= TWO_M_PI;
            }
            const auto x = A * qSin(phase_rad + xp_offset);
            const auto y = A * qSin(phase_rad + yp_offset);
            const auto z = A * qSin(phase_rad + zp_offset);

            const auto t = t_offset + 4 * qSin(phase_rad);

            stream << qSetRealNumberPrecision(8) << x << '\t' << y << '\t' << z << '\t'
                   << qSetRealNumberPrecision(4) << t << "\r\n"
                   << Qt::flush;

            const auto bytes = buffer.toUtf8();
            if (stdout_io) {
                stdout_io->write(bytes);
            }
            io_device->write(bytes);
            QThread::msleep(static_cast<int>(1.0 / rate_hz * 1000.0));
        }
    });

    if (stdout_io) {
        stdout_io->setParent(io);
    }
    io_device->setParent(io);
    io->setObjectName("io_thread");
    io->setParent(&app);
    io->start();

    return QCoreApplication::exec();
}