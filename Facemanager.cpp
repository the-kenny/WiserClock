#include "FaceManager.h"

#include "SimpleFace.h"
#include "PongFace.h"

//Here we instantiate all views
SimpleFace simpleFace;
PongFace pongFace;

FaceManager::FaceManager() {
  faces[0] = &simpleFace;
  faces[1] = &pongFace;
}

ClockFace* FaceManager::nextFace() {
  if(currentFaceIdx+1 >= FACE_COUNT)
    currentFaceIdx = 0;
  else
    ++currentFaceIdx;

  return currentFace();
}

ClockFace* FaceManager::currentFace() const {
  if(currentFaceIdx >= FACE_COUNT) {
    Serial.println("Houston, we have problem: currentFaceIdx >= faceCount");
    return faces[0];
  }

  return faces[currentFaceIdx];
}

