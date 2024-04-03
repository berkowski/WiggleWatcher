#include <core/settings.h>

#include <erbsland/qt/TomlValue.hpp>

#include <QtCore/qstring.h>
#include <QtCore/qpair.h>
#include <QtCore/qtextstream.h>


using namespace Qt::Literals::StringLiterals;
using Value = erbsland::qt::toml::Value;

// required keys
const auto KEY_LOG_PATH = u"path"_s;
const auto KEY_LOG_INTERVAL = u"interval"_s;

struct ValidationParams {
    QString section;
    QString key;
    Value::Type type_;
};

const auto VALIDATION_KEYS = QList<ValidationParams>{
    {"log", KEY_LOG_PATH, Value::Type::String},
    {"log", KEY_LOG_INTERVAL, Value::Type::Integer}
};

namespace
{
    inline auto dotted_toml_key(const QString& section, const QString& key) -> QString
    {
        return QStringLiteral("%1.%2").arg(section, key);
    }

    inline auto check_toml_key(const std::shared_ptr<Value>& toml, const ValidationParams& params) -> QPair<bool, QString>
    {
        const auto dotted_key = dotted_toml_key(params.section, params.key);
        const auto value = toml->value(dotted_key);
        if (!value) {
            return {false, QStringLiteral("section [%1] missing required key: %2").arg(params.section, params.key)};
        }
        const auto value_type = value->type();
        if (value_type != params.type_) {
            return {
                false,
                QStringLiteral("section [%1] key '%2' has invalid type.  Expected %3, found %4")
                .arg(params.section,
                     params.key,
                     erbsland::qt::toml::valueTypeToString(params.type_),
                     erbsland::qt::toml::valueTypeToString(value_type))};
        }

        return {true, {}};
    }

    auto validate_toml(const std::shared_ptr<Value>& toml) -> QPair<bool, QString>
    {
        if (!toml) {
            return {false, QStringLiteral("invalid toml object")};
        }

        for (auto it = VALIDATION_KEYS.constBegin(); it != VALIDATION_KEYS.constEnd(); ++it) {
            const auto result = check_toml_key(toml, *it);
            if(!result.first) {
                return result;
            }
        }
        return {true, {}};
    }

}

Settings::Settings(std::shared_ptr<Value> &&toml_ptr)
        : toml(std::move(toml_ptr)) {
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
    if (toml) {
        return true;
    } else {
        return false;
    }
}

auto Settings::dir() const noexcept -> QString {
    if (!isValid()) {
        return {};
    }
    return toml->stringValue(dotted_toml_key("log", KEY_LOG_PATH));
}

auto Settings::setDir(const QString &dir) noexcept -> void {
    if (isValid()) {
        toml->setValue(dotted_toml_key("log", KEY_LOG_PATH), Value::createString(dir));
    }
}

auto Settings::setInterval(const std::chrono::minutes &minutes) noexcept -> void {
    if (isValid()) {
        toml->setValue(dotted_toml_key("log", KEY_LOG_INTERVAL), Value::createInteger(minutes.count()));
    }
}

auto Settings::interval() const noexcept -> std::chrono::minutes {
    if (isValid())
    {
        return std::chrono::minutes{toml->integerValue(dotted_toml_key("log", KEY_LOG_INTERVAL), -1)};
    }
    else {
        return {};
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
