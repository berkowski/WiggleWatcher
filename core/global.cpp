#include <core/global.h>
#include <QMetaEnum>

namespace maggui
{
auto toString(SensorKind t) -> QString
{
    const auto meta = QMetaEnum::fromType<SensorKind>();
    return QString{meta.valueToKey(t)};
}

auto fromString(const QString &string, bool *ok) -> SensorKind
{
    const auto meta = QMetaEnum::fromType<SensorKind>();
    const auto index = meta.keyToValue(string.toUpper().toUtf8().data());
    if (index < 0) {
        if (ok) {
            *ok = false;
        }
        return SensorKind::APS1540;
    } else {
        if (ok) {
            *ok = true;
        }
        return static_cast<SensorKind>(index);
    }
}
} // namespace maggui
