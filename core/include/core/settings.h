#ifndef WIGGLEWATCHER_SETTINGS_H
#define WIGGLEWATCHER_SETTINGS_H


#include <core/global.h>

#include <QMetaType>

#include <chrono>
#include <memory>

namespace erbsland::qt::toml
{
class Value;
}

class SensorBase;

class WIGGLEWATCHER_CORE_EXPORT Settings
{
public:
    struct Sensor {
        QString name;
        QString description;
        QString connection;
        wigglewatcher::SensorKind kind;
    };

    ~Settings() = default;
    static auto fromFile(const QString &path) -> Settings;
    static auto fromString(const QString &string) -> Settings;
    [[nodiscard]] auto toString() const noexcept -> QString;

    static auto validateString(const QString& toml) -> bool;
    static auto validateFile(const QString& file) -> bool;

    [[nodiscard]] auto isValid() const noexcept -> bool;

    auto setDir(const QString &dir) noexcept -> void;
    [[nodiscard]] auto dir() const noexcept -> QString;

    auto setInterval(const std::chrono::minutes& minutes) noexcept -> void;
    [[nodiscard]] auto interval() const noexcept -> std::chrono::minutes;

    auto sensor(const QString& name) const noexcept -> std::shared_ptr<Sensor>;
    auto sensors() const noexcept -> QList<std::shared_ptr<Sensor>>;

private:
    explicit Settings(std::shared_ptr<erbsland::qt::toml::Value>&& toml);
    bool is_valid = false;

    QString log_directory;
    std::chrono::minutes log_interval{};

    QList<std::shared_ptr<Sensor>> sensors_;
    //std::shared_ptr<erbsland::qt::toml::Value> toml;
};

Q_DECLARE_METATYPE(Settings)

#endif