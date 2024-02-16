#ifndef VECTORMAGNETOMETERDATA_H
#define VECTORMAGNETOMETERDATA_H

#include <QtCore/qmetatype.h>
#include <QtCore/qdatetime.h>

struct VectorMagnetometerData {
  //! Timestamp of data
  QDateTime timestamp;
  //! instrument-frame X component of magnetic field in nano-tesla
  int x;
  //! instrument-frame Y component of magnetic field in nano-tesla
  int y;
  //! instrument-frame Z component of magnetic field in nano-tesla
  int z;
  //! instrument temperature in celcius
  double temperature;
};

Q_DECLARE_METATYPE(VectorMagnetometerData)
#endif
