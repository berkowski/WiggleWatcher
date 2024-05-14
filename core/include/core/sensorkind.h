#ifndef MAGGUI_SENSORKIND_H
#define MAGGUI_SENSORKIND_H

#include <core/export.h>

#ifndef MAGGUI_GLOBAL_H
namespace maggui
{
#endif

enum MAGGUI_CORE_EXPORT SensorKind {
    APS1540,
    HMR2300,
};

MAGGUI_CORE_EXPORT auto toString(SensorKind t) -> QString;
MAGGUI_CORE_EXPORT auto fromString(const QString &string, bool* ok=nullptr) -> SensorKind;

#ifndef MAGGUI_GLOBAL_H
}
#endif

#endif //MAGGUI_SENSORKIND_H
