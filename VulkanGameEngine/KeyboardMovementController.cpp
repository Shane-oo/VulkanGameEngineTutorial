//
// Created by Shane on 1/04/2024.
//

#include "KeyboardMovementController.h"

// #region Private Methods

void KeyboardMovementController::HandleRotation(GLFWwindow *window,
                                                float deltaTime,
                                                GameObject &gameObject) const {
  glm::vec3 rotate = glm::vec3(0);
  if (glfwGetKey(window, Keys.LookRight) == GLFW_PRESS) {
    rotate.y += 1.f;
  }
  if (glfwGetKey(window, Keys.LookLeft) == GLFW_PRESS) {
    rotate.y -= 1.f;
  }
  if (glfwGetKey(window, Keys.LookUp) == GLFW_PRESS) {
    rotate.x += 1.f;
  }
  if (glfwGetKey(window, Keys.LookDown) == GLFW_PRESS) {
    rotate.x -= 1.f;
  }

  if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
    gameObject.transformComponent.Rotation +=
        LookSpeed * deltaTime * glm::normalize(rotate);
  }

  // limit pitch values between about +/- 85 degrees
  gameObject.transformComponent.Rotation.x =
      glm::clamp(gameObject.transformComponent.Rotation.x, -1.5f, 1.5f);
  gameObject.transformComponent.Rotation.y =
      glm::mod(gameObject.transformComponent.Rotation.y, glm::two_pi<float>());
}

void KeyboardMovementController::HandleTranslation(
    GLFWwindow *window, float deltaTime, GameObject &gameObject) const {
  float yaw = gameObject.transformComponent.Rotation.y;
  const glm::vec3 forwardDirection = glm::vec3(sin(yaw), 0.f, cos(yaw));
  const glm::vec3 rightDirection =
      glm::vec3(forwardDirection.z, 0.f, -forwardDirection.x);
  const glm::vec3 upDirection = glm::vec3(0.f, -1.f, 0.f);

  glm::vec3 moveDirection = glm::vec3(0.f);
  if (glfwGetKey(window, Keys.MoveForward) == GLFW_PRESS) {
    moveDirection += forwardDirection;
  }
  if (glfwGetKey(window, Keys.MoveBackward) == GLFW_PRESS) {
    moveDirection -= forwardDirection;
  }
  if (glfwGetKey(window, Keys.MoveRight) == GLFW_PRESS) {
    moveDirection += rightDirection;
  }
  if (glfwGetKey(window, Keys.MoveLeft) == GLFW_PRESS) {
    moveDirection -= rightDirection;
  }
  if (glfwGetKey(window, Keys.MoveUp) == GLFW_PRESS) {
    moveDirection += upDirection;
  }
  if (glfwGetKey(window, Keys.MoveDown) == GLFW_PRESS) {
    moveDirection -= upDirection;
  }

  if (glm::dot(moveDirection, moveDirection) >
      std::numeric_limits<float>::epsilon()) {
    gameObject.transformComponent.Translation +=
        MoveSpeed * deltaTime * glm::normalize(moveDirection);
  }
}

// #endregion

// #region Public Methods
void KeyboardMovementController::MoveInPlaneXZ(GLFWwindow *window,
                                               float deltaTime,
                                               GameObject &gameObject) {
  HandleRotation(window, deltaTime, gameObject);

  HandleTranslation(window, deltaTime, gameObject);
}

// #endregion