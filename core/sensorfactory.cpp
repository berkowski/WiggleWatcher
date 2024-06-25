#include "core/sensorfactory.h"
#include <core/aps1540magnetometer.h>
#include <core/iofactory.h>

#include <erbsland/qt/all_toml.hpp>
#include <memory>

#include <QMetaType>

extern const QString KEY_SENSOR_TYPE;
extern const QString KEY_SENSOR_CONNECTION;
extern const QString KEY_SENSOR_NAME;

namespace {
    auto error_string = QString{};
}

auto SensorFactory::create(maggui::SensorKind kind, const QString &name, const QString &connection,
                           const QString &description, const QVariantMap &options) -> std::unique_ptr<SensorBase> {

    auto io = IOFactory::from_string(connection, nullptr);
    if(!io) {
        error_string = QStringLiteral("Unable to connect to sensor %1\nerror: %2").arg(name, IOFactory::last_error_string());
        qCritical() << error_string;
        return {};
    }

    auto sensor = std::unique_ptr<SensorBase>{};

    switch(kind) {
        case maggui::SensorKind::APS1540:
            sensor = std::make_unique<Aps1540Magnetometer>(io);
            break;
        case maggui::SensorKind::HMR2300:
            Q_UNIMPLEMENTED();
            return {};
        default:
            Q_UNREACHABLE();
    }

    Q_ASSERT(sensor);

    sensor->setDescription(description);
    sensor->setName(name);

    return sensor;
}

auto SensorFactory::last_error_string() -> QString {
    const auto s = error_string;
    error_string.clear();
    return s;
}
#if 0
auto SensorFactory::to_toml(const SensorBase *sensor) -> std::shared_ptr<erbsland::qt::toml::Value> {
    using namespace erbsland::qt::toml;

    Value
    return std::shared_ptr<erbsland::qt::toml::Value>();
}
#endif