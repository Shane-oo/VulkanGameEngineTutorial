//
// Created by Shane on 1/04/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_KEYBOARDMOVEMENTCONTROLLER_H
#define VULKANGAMEENGINETUTORIAL_KEYBOARDMOVEMENTCONTROLLER_H

#include "GameObject.h"
#include <GLFW/glfw3.h>
class KeyboardMovementController {
public:
  struct KeyMappings {
    int MoveLeft = GLFW_KEY_A;
    int MoveRight = GLFW_KEY_D;
    int MoveForward = GLFW_KEY_W;
    int MoveBackward = GLFW_KEY_S;
    int MoveUp = GLFW_KEY_E;
    int MoveDown = GLFW_KEY_Q;
    int LookLeft = GLFW_KEY_LEFT;
    int LookRight = GLFW_KEY_RIGHT;
    int LookUp = GLFW_KEY_UP;
    int LookDown = GLFW_KEY_DOWN;
  };
  
  KeyMappings Keys = KeyMappings();
  float MoveSpeed = 3.f;
  float LookSpeed = 1.5f;
  
  void MoveInPlaneXZ(GLFWwindow* window, float deltaTime, GameObject& gameObject);

  void HandleRotation(GLFWwindow *window, float deltaTime,
                  GameObject &gameObject) const;
  void HandleTranslation(GLFWwindow *window, float deltaTime,
                      GameObject &gameObject) const;
};

#endif // VULKANGAMEENGINETUTORIAL_KEYBOARDMOVEMENTCONTROLLER_H
