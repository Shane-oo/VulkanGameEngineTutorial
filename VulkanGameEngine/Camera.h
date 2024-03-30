//
// Created by Shane on 30/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_CAMERA_H
#define VULKANGAMEENGINETUTORIAL_CAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>

class Camera {
public:
  void SetOrthographicProjection(float left, float right, float top,
                                 float bottom, float near, float far);

  void SetPerspectiveProjection(float fovY, float aspect, float near,
                                float far);

  const glm::mat4 &GetProjectionMatrix() const { return projectionMatrix; };

private:
  glm::mat4 projectionMatrix = glm::mat4(1.f);
};

#endif // VULKANGAMEENGINETUTORIAL_CAMERA_H
