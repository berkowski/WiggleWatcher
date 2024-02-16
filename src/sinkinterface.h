#ifndef SINKINTERFACE_H
#define SINKINTERFACE_H

#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>

class SinkInterface {

public:
  virtual auto write(const QVariant& record) -> void = 0;
};

Q_DECLARE_INTERFACE(SinkInterface, "maggui.sinkinterface/1.0")
#endif
