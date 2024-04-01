#ifndef MAGGUI_SETTINGS_H
#define MAGGUI_SETTINGS_H


#include <core/global.h>

#include <QtCore/qmetatype.h>

#include <memory>

namespace erbsland::qt::toml
{
class Value;
}

class MAGGUI_CORE_EXPORT Settings
{
public:
    ~Settings() = default;

    static auto fromFile(const QString &path) -> Settings;
    static auto fromString(const QString &string) -> Settings;

    static auto validateString(const QString& toml) -> bool;
    static auto validateFile(const QString& file) -> bool;

    [[nodiscard]] auto isValid() const noexcept -> bool;

    auto setDir(const QString &dir) noexcept -> void;
    [[nodiscard]] auto dir() const noexcept -> QString;


private:
    explicit Settings(std::shared_ptr<erbsland::qt::toml::Value>&& toml);
    std::shared_ptr<erbsland::qt::toml::Value> toml;
};

Q_DECLARE_METATYPE(Settings)

#endif