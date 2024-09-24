#include <libqio/iofactory.h>

#include <QtCore/qcommandlineparser.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qfile.h>
#include <QtCore/qmath.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qthread.h>
#include <QRandomGenerator>

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
	{{"n", "noise"}, QCoreApplication::translate("main", "add noise"), "nT", "0.0"},
	{{"o", "offset"}, QCoreApplication::translate("main", "add constant offset to data"), "nT", "0.0"},
	{{"a", "amplitude"}, QCoreApplication::translate("main", "signal amplitude"), "nT", "1.0"},
    });

    parser.addPositionalArgument(
        "output",
        QCoreApplication::translate("main", "output config: [udp://.., serial://..]"),
        "output");

    parser.process(app);

    const auto args = parser.positionalArguments();

    #if 0
    if (args.isEmpty()) {
        std::fputs(qPrintable(u"Missing required argument 'output'\r\n"_s), stderr);
        std::fputs(qPrintable(parser.helpText()), stderr);
        return 1;
    }
    #endif
    
    auto io_device = static_cast<QIODevice*>(nullptr); //IOFactory::from_string(args.at(0), nullptr);
    if (!args.isEmpty()) {
      io_device = IOFactory::from_string(args.at(0), nullptr);
      if (!io_device) {
        std::fputs(qPrintable(u"Unable to parse output config: '"_s), stderr);
        std::fputs(qPrintable(args.at(0)), stderr);
        std::fputs(qPrintable(u"'\r\n"_s), stderr);
        std::fputs(qPrintable(parser.helpText()), stderr);
        return 1;
      }
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

    const auto offset = [&]() -> double {
      auto ok = true;
      const auto value = parser.value("offset").toDouble(&ok);
      if (!ok) {
	qCritical() << "invalid value for offset: " << parser.value("offset");
	return -1;
      }
      else {
	return value;
      }
    }();

    const auto noise = [&]() -> double {
      auto ok = true;
      const auto value = parser.value("noise").toDouble(&ok);
      if (!ok) {
	qCritical() << "invalid value for noise: " << parser.value("noise");
	return -1;
      }
      else {
	return qAbs(value);
      }
    }();

    const auto amplitude = [&]() -> double {
      auto ok = true;
      const auto value = parser.value("amplitude").toDouble(&ok);
      if (!ok) {
	qCritical() << "invalid value for amplitude: " << parser.value("amplitude");
	return -1;
      }
      else {
	return qAbs(value);
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

        const auto xp_offset = 0.0 * M_PI / 180.0;
        const auto yp_offset = 120.0 * M_PI / 180.0;
        const auto zp_offset = 240.0 * M_PI / 180.0;

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
	    auto noise_x = 0.0;
	    auto noise_y = 0.0;
	    auto noise_z = 0.0;
	    if (parser.isSet("noise")) {
	      noise_x = (QRandomGenerator::global()->generateDouble() - 0.5) * noise;
	      noise_y = (QRandomGenerator::global()->generateDouble() - 0.5) * noise;
	      noise_z = (QRandomGenerator::global()->generateDouble() - 0.5) * noise;
	    }
            const auto x = amplitude * qSin(phase_rad + xp_offset) + noise_x + offset;
            const auto y = amplitude * qSin(phase_rad + yp_offset) + noise_y + offset;
            const auto z = amplitude * qSin(phase_rad + zp_offset) + noise_z + offset;

            const auto t = t_offset + 4 * qSin(phase_rad);

            stream << qSetRealNumberPrecision(8) << x << '\t' << y << '\t' << z << '\t'
                   << qSetRealNumberPrecision(4) << t << Qt::endl;

            const auto bytes = buffer.toUtf8();
            if (stdout_io) {
                stdout_io->write(bytes);
            }
	    if (io_device) {
	      io_device->write(bytes);
	    }
            QThread::msleep(static_cast<int>(1.0 / rate_hz * 1000.0));
        }
    });

    if (stdout_io) {
        stdout_io->setParent(io);
    }
    if (io_device) {
      io_device->setParent(io);
    }
    io->setObjectName("io_thread");
    io->setParent(&app);
    io->start();

    return QCoreApplication::exec();
}
