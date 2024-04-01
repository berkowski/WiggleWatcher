#include <core/settings.h>

#include <erbsland/qt/TomlValue.hpp>

#include <QtCore/qstring.h>
#include <QtCore/qpair.h>

using namespace Qt::Literals::StringLiterals;
using Value = erbsland::qt::toml::Value;

// required keys
const auto KEY_LOG_DIRECTORY = u"log.root"_s;

namespace
{
    auto validate_toml(const std::shared_ptr<Value>& toml) -> QPair<bool, QString>
    {
        if (!toml) {
            return {false, QStringLiteral("invalid toml object")};
        }

        if (!toml->hasValue(KEY_LOG_DIRECTORY)) {
            return {false, QStringLiteral("missing required key: %1").arg(KEY_LOG_DIRECTORY)};
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
    return toml->stringValue(KEY_LOG_DIRECTORY);
}

auto Settings::setDir(const QString &dir) noexcept -> void {
    if (isValid()) {
        toml->setValue(KEY_LOG_DIRECTORY, Value::createString(dir));
    }
}
