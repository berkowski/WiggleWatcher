#ifndef MAGGUI_GLOBAL_H
#define MAGGUI_GLOBAL_H
#include <core/export.h>

#include <QObject>
#include <QString>
namespace maggui
{
MAGGUI_CORE_EXPORT
Q_NAMESPACE

enum MAGGUI_CORE_EXPORT SensorKind {
    APS1540,
    HMR2300,
};
Q_ENUM_NS(SensorKind)

MAGGUI_CORE_EXPORT auto toString(SensorKind t) -> QString;
MAGGUI_CORE_EXPORT auto fromString(const QString &string, bool* ok=nullptr) -> SensorKind;

// #include "sensorkind.h"

}
#endif //MAGGUI_GLOBAL_H
