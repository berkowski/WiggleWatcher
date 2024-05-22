#include <core/settings.h>

#include <erbsland/qt/TomlValue.hpp>

#include <QtCore/qstring.h>
#include <QtCore/qpair.h>
#include <QtCore/qtextstream.h>


using namespace Qt::Literals::StringLiterals;
using Value = erbsland::qt::toml::Value;

// required log keys
const auto KEY_LOG_PATH = u"path"_s;
const auto KEY_LOG_INTERVAL = u"interval"_s;

// required sensor keys
const auto KEY_SENSOR_NAME = u"name"_s;
const auto KEY_SENSOR_DESCRIPTION = u"description"_s;
const auto KEY_SENSOR_CONNECTION = u"connection"_s;
const auto KEY_SENSOR_KIND = u"kind"_s;

struct ValidationParams {
    QString section;
    QString key;
    Value::Type type_;
};

const auto LOG_VALIDATION_KEYS = QList<ValidationParams>{
    {"log", KEY_LOG_PATH, Value::Type::String},
    {"log", KEY_LOG_INTERVAL, Value::Type::Integer},
};

const auto SENSOR_VALIDATION_KEYS = QList<ValidationParams>{
        {"sensor", KEY_SENSOR_NAME, Value::Type::String},
        {"sensor", KEY_SENSOR_CONNECTION, Value::Type::String},
        {"sensor", KEY_SENSOR_KIND, Value::Type::String},
};

namespace
{
    inline auto check_toml_key(const std::shared_ptr<Value>& toml, const QString& section, const QString& key, const Value::Type& type_) -> QPair<bool, QString>
    {
        const auto value = toml->value(key);
        if (!value) {
            return {false, QStringLiteral("section [%1] missing required key: %2").arg(section, key)};
        }
        const auto value_type = value->type();
        if (value_type != type_) {
            return {
                false,
                QStringLiteral("section [%1] key '%2' has invalid type.  Expected %3, found %4")
                    .arg(section,
                         key,
                         erbsland::qt::toml::valueTypeToString(type_),
                         erbsland::qt::toml::valueTypeToString(value_type))};
        }

        return {true, {}};
    }
    auto validate_toml(const std::shared_ptr<Value>& toml) -> QPair<bool, QString>
    {
        if (!toml) {
            return {false, QStringLiteral("invalid toml object")};
        }

        const auto log_section = toml->tableValue(QStringLiteral("log"));
        if(!log_section) {
            return {false, QString{"Missing [log] section"}};
        }

        for (auto it = LOG_VALIDATION_KEYS.constBegin(); it != LOG_VALIDATION_KEYS.constEnd(); ++it) {
            const auto result = check_toml_key(log_section, it->section, it->key, it->type_);
            if(!result.first) {
                return result;
            }
        }

        const auto sensors = toml->arrayValue(QStringLiteral("sensor"));

        if(sensors->size() < 1) {
            return {false, QString{"Missing [[sensor]] entry."}};
        }

        for (const auto& sensor: *sensors) {
            for (auto it = SENSOR_VALIDATION_KEYS.constBegin(); it != SENSOR_VALIDATION_KEYS.constEnd(); ++it) {
                const auto result = check_toml_key(sensor, it->section, it->key, it->type_);
                if (!result.first) {
                    return result;
                }
            }
            auto ok = true;
            maggui::fromString(sensor->stringValue(KEY_SENSOR_KIND), &ok);
            if (!ok) {
                return {false, QString("kind = %1 is not a valid sensor type (sensor name = %2)").arg(sensor->stringValue(KEY_SENSOR_KIND), sensor->stringValue(KEY_SENSOR_NAME))};
            }
        }
        return {true, {}};
    }

}

Settings::Settings(std::shared_ptr<Value> &&toml_ptr)
{
    if(!toml_ptr) {
        is_valid = false;
        return;
    }

    const auto log = toml_ptr->tableValue(QStringLiteral("log"));
    log_directory = log->stringValue(KEY_LOG_PATH);
    log_interval = std::chrono::minutes{log->integerValue(KEY_LOG_INTERVAL, -1)};

    const auto sensors = toml_ptr->arrayValue(QStringLiteral("sensor"));
    for(const auto& it: *sensors) {
        auto sensor = std::make_shared<Sensor>();
        sensor->name = it->stringValue(KEY_SENSOR_NAME);
        sensor->description = it->stringValue(KEY_SENSOR_DESCRIPTION);
        sensor->connection = it->stringValue(KEY_SENSOR_CONNECTION);
        sensor->kind = maggui::fromString(it->stringValue(KEY_SENSOR_KIND));
        sensors_.push_back(std::move(sensor));
    }
    is_valid = true;
}

auto Settings::validateString(const QString &toml) -> bool {
    return false;
}

auto Settings::validateFile(const QString &file) -> bool {
    return false;
}

auto Settings::fromFile(const QString &path) -> Settings {
    auto parser = erbsland::qt::toml::Parser{};
    auto toml = parser.parseFile(path);
    if (!toml) {
        qCritical() << "Unable to load settings from " << path << parser.lastError().toString();
        return Settings(nullptr);
    }

    // validate TOML object
    if (const auto result = validate_toml(toml); !result.first) {
        qCritical() << "Unable to load settings from " << path << ". " << result.second;
        return Settings(nullptr);
    }

    return Settings(std::move(toml));
}

auto Settings::fromString(const QString &string) -> Settings {
    auto parser = erbsland::qt::toml::Parser{};
    auto toml = parser.parseString(string);
    if (!toml) {
        qCritical() << "Unable to load settings string: " << parser.lastError().toString();
        return Settings(nullptr);
    }

    // validate TOML object
    if (const auto result = validate_toml(toml); !result.first) {
        qCritical() << "Unable to load settings string. " << result.second;
        return Settings(nullptr);
    }

    return Settings(std::move(toml));
}

auto Settings::isValid() const noexcept -> bool {
    return is_valid;
}

auto Settings::dir() const noexcept -> QString {
    if (!isValid()) {
        return {};
    }
    else {
        return log_directory;
    }
}

auto Settings::setDir(const QString &dir) noexcept -> void {
    log_directory = dir;
}

auto Settings::setInterval(const std::chrono::minutes &minutes) noexcept -> void {
    log_interval = minutes;
}

auto Settings::interval() const noexcept -> std::chrono::minutes {
    if (!isValid())
    {
        return std::chrono::minutes{-1};
    }
    else {
        return log_interval;
    }
}

auto Settings::toString() const noexcept -> QString {
    if (!isValid()) {
        return {};
    }

    auto string = QString{};
    auto stream = QTextStream{&string};

    stream << "[log]" << Qt::endl;
    stream << KEY_LOG_PATH << " = \"" << dir() << "\"" << Qt::endl;
    stream << KEY_LOG_INTERVAL << " = " << interval().count() << Qt::endl;
    stream << Qt::endl;

    return string;
}
auto Settings::sensor(const QString &name) const noexcept -> std::shared_ptr<Sensor>
{
    const auto sensor_ = std::find_if(std::begin(sensors_), std::end(sensors_), [&](const auto& it) {
        if (it) {
            return it->name == name;
        }
        else {
            return false;
        }
    });
    if (sensor_ != std::end(sensors_)) {
        return *sensor_;
    }
    else {
        return {};
    }
}

auto Settings::sensors() const noexcept -> QList<std::shared_ptr<Sensor>>
{
    return sensors_;
}
