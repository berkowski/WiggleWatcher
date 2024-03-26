#include <core/aps1540magnetometer.h>
#include <core/filetimer.h>
#include <core/iofactory.h>
#include <core/textfilesink.h>

#include <QtCore/qcoreapplication.h>

#include <chrono>

int main(int argc, char *argv[])
{
    auto app = QCoreApplication{argc, argv};

    auto rollover_timer = FileTimer{};
    rollover_timer.setInterval(std::chrono::seconds(10));

    const auto log_root = QDir{"/tmp"};
    auto raw_logger = TextFileSink{log_root, QStringLiteral("raw"), QStringLiteral(".txt")};

    auto io = IOFactory::from_string("udp://127.0.0.1:5001:5000");
    if (!io) {
        qFatal() << "Unable to open udp socket";
    }

    auto aps1540 = Aps1540Magnetometer{io};

    //    QObject::connect(&aps1540, &Aps1540Magnetometer::bytesRead, &raw_logger, &TextFileSink::write);
    QObject::connect(&aps1540, &Aps1540Magnetometer::valueChanged, [&](const auto &value) {
        raw_logger.write(value.toDslFormat().toUtf8());
    });
    QObject::connect(&rollover_timer, &FileTimer::timeout, [&]() {
        const auto seconds = qRound(
            static_cast<double>(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch()) / 1000.0);
        const auto now = QDateTime::fromSecsSinceEpoch(seconds);
        qInfo() << "Time until next rollover: " << rollover_timer.remainingTime() << "ms @"
                << now.toString(Qt::ISODateWithMs);
        raw_logger.rollover(now);
    });

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &raw_logger, &TextFileSink::flush);
    //    QObject::connect(&aps1540, &Aps1540Magnetometer::bytesRead, [&](const auto &bytes) {
    //        qInfo() << QString::fromUtf8(bytes);
    //    });

    rollover_timer.start();
    return QCoreApplication::exec();
}