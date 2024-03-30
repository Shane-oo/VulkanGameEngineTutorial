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

  void SetViewDirection(glm::vec3 position, glm::vec3 direction,
                        glm::vec3 up = glm::vec3(0.f, -1.f, 0.f));

  void SetViewTarget(glm::vec3 position, glm::vec3 target,
                     glm::vec3 up = glm::vec3(0.f, -1.f, 0.f));

  void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

  const glm::mat4 &GetProjectionMatrix() const { return projectionMatrix; };

  const glm::mat4 &GetViewMatrix() const { return viewMatrix; };

private:
  glm::mat4 projectionMatrix = glm::mat4(1.f);
  glm::mat4 viewMatrix = glm::mat4(1.f);
};

#endif // VULKANGAMEENGINETUTORIAL_CAMERA_H
