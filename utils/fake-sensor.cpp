#include <QtCore/qcoreapplication.h>
#include <QtCore/qthread.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qmath.h>
#include <QtCore/qfile.h>
#include <QtCore/qcommandlineparser.h>

#include <cstdio>
#include <csignal>


int main(int argc, char* argv[])
{

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("fake-sensor");
    QCoreApplication::setApplicationVersion("0.1");

    auto parser = QCommandLineParser{};
    parser.setApplicationDescription("Generate fake sensor data for testing");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
        {{"o", "output"}, QCoreApplication::translate("main", "Output type.")},
        {{"r", "rate"}, QCoreApplication::translate("main", "update rate"), "HZ", "8.0"},
    });

    parser.process(app);

    const auto args = parser.positionalArguments();

    auto io_device = [&]() {
        if (parser.isSet("output")) {
            return static_cast<QIODevice*>(nullptr);
        } else {
            auto device =  new QFile;
            if (!device->open(stdout, QIODevice::WriteOnly)) {
                qFatal() << "unable to open stdout for writing";
            }
            return qobject_cast<QIODevice*>(device);
        }
    }();

    const auto rate_hz = [&]() -> double {
        auto ok = true;
        const auto value = parser.value("rate").toDouble(&ok);
        if (!ok) {
            qCritical() << "invalid value for rate: " << parser.value("rate");
            return -1.0;
        }
        else {
            return value;
        }
    }();

    auto io = QThread::create([&]() -> void {

        const auto TWO_M_PI = 2 * M_PI;
        //const auto rate_hz = 20.0;
        auto phase_rad = 0.0;
        const auto frequency_rad_s = TWO_M_PI * 1.0/60.0;

        const auto A = 1.0;
        const auto xp_offset = 0.0;
        const auto yp_offset = 120.0;
        const auto zp_offset = 240.0;

        auto t_offset = 25.0;

        auto stream = QTextStream{io_device};
        stream.setNumberFlags(QTextStream::ForceSign);
        stream.setRealNumberNotation(QTextStream::FixedNotation);

        while (1)
        {
            phase_rad += TWO_M_PI * frequency_rad_s / rate_hz;
            while (phase_rad > TWO_M_PI) {
                phase_rad -= TWO_M_PI;
            }
            const auto x = A * qSin(phase_rad + xp_offset);
            const auto y = A * qSin(phase_rad + yp_offset);
            const auto z = A * qSin(phase_rad + zp_offset);

            const auto t = t_offset + 4 * qSin(phase_rad);

            stream << qSetRealNumberPrecision(8)
                   << x << '\t'
                   << y << '\t'
                   << z << '\t'
                   << qSetRealNumberPrecision(4) << t << "\r\n"
                   << Qt::flush;

            QThread::msleep(static_cast<int>(1.0/rate_hz * 1000.0));
        }
    });

    io_device->setParent(io);
    io->setObjectName("io_thread");
    io->setParent(&app);
    io->start();

    return QCoreApplication::exec();
}