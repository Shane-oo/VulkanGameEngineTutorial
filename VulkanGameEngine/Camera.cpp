//
// Created by Shane on 30/03/2024.
//

#include "Camera.h"

// #region Private Methods

// #endregion

// #region Constructors

// #endregion

// #region Public Methods

void Camera::SetOrthographicProjection(float left, float right, float top,
                                       float bottom, float near, float far) {
  projectionMatrix[0][0] = 2.f / (right - left);
  projectionMatrix[1][1] = 2.f / (bottom - top);
  projectionMatrix[2][2] = 1.f / (far - near);
  projectionMatrix[3][0] = -(right + left) / (right - left);
  projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
  projectionMatrix[3][2] = -near / (far - near);
}
void Camera::SetPerspectiveProjection(float fovY, float aspect, float near,
                                      float far) {
  assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
  const float tanHalfFovY = tan(fovY / 2.f);
  projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovY);
  projectionMatrix[1][1] = 1.f / (tanHalfFovY);
  projectionMatrix[2][2] = far / (far - near);
  projectionMatrix[2][3] = 1.f;
  projectionMatrix[3][2] = -(far * near) / (far - near);
}

// #endregion
