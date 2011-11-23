#ifndef FACE_MGR_H
#define FACE_MGR_H

#include <Arduino.h>
#include "ClockFace.h"

enum {
FACE_COUNT = 2
};

struct FaceManager {
  FaceManager();
  ClockFace* nextFace();
  ClockFace* currentFace() const;

private:
  ClockFace* faces[FACE_COUNT];
  byte currentFaceIdx;
};

#endif
