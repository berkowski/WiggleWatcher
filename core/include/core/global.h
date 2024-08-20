#ifndef WIGGLEWATCHER_GLOBAL_H
#define WIGGLEWATCHER_GLOBAL_H
#include <core/export.h>

#include <QObject>
#include <QString>
namespace wigglewatcher
{
WIGGLEWATCHER_CORE_EXPORT
Q_NAMESPACE

enum WIGGLEWATCHER_CORE_EXPORT SensorKind {
    APS1540,
    HMR2300,
};
Q_ENUM_NS(SensorKind)

WIGGLEWATCHER_CORE_EXPORT auto toString(SensorKind t) -> QString;
WIGGLEWATCHER_CORE_EXPORT auto fromString(const QString &string, bool* ok=nullptr) -> SensorKind;

// #include "sensorkind.h"

}
#endif //WIGGLEWATCHER_GLOBAL_H
