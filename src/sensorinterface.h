#ifndef MAGGUI_SENSORINTERFACE_H
#define MAGGUI_SENSORINTERFACE_H

#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>

class SensorInterface
{
public:
signals:
    /// Signal emitted when the sensor has a new data record
    virtual auto recordChanged(QVariant record) -> void = 0;
};

Q_DECLARE_INTERFACE(SensorInterface, "maggui.sensorinterface/1.0")
#endif //MAGGUI_SENSORINTERFACE_H
