#ifndef MAGGUI_GLOBAL_H
#define MAGGUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MAGGUI_CORE_LIBRARY)
#define MAGGUI_CORE_EXPORT Q_DECL_EXPORT
#else
#define MAGGUI_CORE_EXPORT Q_DECL_IMPORT
#endif
#endif //MAGGUI_GLOBAL_H
