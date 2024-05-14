#ifndef MAGGUI_SENSORFACTORY_H
#define MAGGUI_SENSORFACTORY_H

#include <core/global.h>
#include <core/sensorbase.h>

#include <QtCore/qstring.h>
#include <QtCore/qvariantmap.h>

#include <memory>

class SensorBase;

class MAGGUI_CORE_EXPORT SensorFactory
{
public:

    static auto create(maggui::SensorKind kind, const QString &name, const QString &connection,
                       const QString &description = {}, const QVariantMap &options = {}) -> std::unique_ptr<SensorBase>;

    //static auto to_toml(const SensorBase *sensor) -> std::shared_ptr<erbsland::qt::toml::Value>;
};


#endif //MAGGUI_SENSORFACTORY_H
