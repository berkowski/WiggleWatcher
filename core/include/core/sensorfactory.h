#ifndef WIGGLEWATCHER_SENSORFACTORY_H
#define WIGGLEWATCHER_SENSORFACTORY_H

#include "global.h"
#include "sensorbase.h"

#include <QString>
#include <QVariantMap>

#include <memory>

class SensorBase;

class WIGGLEWATCHER_CORE_EXPORT SensorFactory
{
public:

    static auto create(wigglewatcher::SensorKind kind, const QString &name, const QString &connection,
                       const QString &description = {}, const QVariantMap &options = {}) -> std::unique_ptr<SensorBase>;

    static auto last_error_string() -> QString;
    //static auto to_toml(const SensorBase *sensor) -> std::shared_ptr<erbsland::qt::toml::Value>;
};


#endif //WIGGLEWATCHER_SENSORFACTORY_H
