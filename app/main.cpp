#include <core/iofactory.h>
#include <core/aps1540magnetometer.h>
#include <core/textfilesink.h>

#include <QtCore/qtimer.h>
#include <QtCore/qcoreapplication.h>

#include <chrono>

int main(int argc, char* argv[])
{
    auto app = QCoreApplication{argc, argv};

    auto rollover_timer = QTimer{};
    rollover_timer.setInterval(std::chrono::seconds(5));

    const auto log_root = QDir{"/tmp"};
    auto raw_logger = TextFileSink{log_root, QStringLiteral("raw"), QStringLiteral(".txt")};

    auto io = IOFactory::from_string("udp://127.0.0.1:5001:5000");
    if (!io) {
        qFatal() << "Unable to open udp socket";
    }

    auto aps1540 = Aps1540Magnetometer{io};

    QObject::connect(&aps1540, &Aps1540Magnetometer::bytesRead, &raw_logger, &TextFileSink::write);
    QObject::connect(&rollover_timer, &QTimer::timeout, [&]() {
        qInfo() << "Rollover";
        raw_logger.rollover(QDateTime::currentDateTimeUtc());
    });

    QObject::connect(&aps1540, &Aps1540Magnetometer::bytesRead, [&](const auto& bytes) {
        qInfo() << QString::fromUtf8(bytes);
    });


    rollover_timer.start();
    return QCoreApplication::exec();
}