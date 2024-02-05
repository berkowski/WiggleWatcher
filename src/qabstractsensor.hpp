#ifndef QABSTRACTSENSOR_HPP
#define QABSTRACTSENSOR_HPP

#include <QtCore/qobject.h>

class QIODevice;
class QAbstractSink;

class QAbstractSensor: public QObject {
  Q_OBJECT

public:
  QAbstractSensor(QIODevice* source = nullptr);

private:
  QIODevice* m_source = nullptr;
  QList<QAbstractSink>* m_sinks = nullptr;
};
#endif
