//
// Created by ShaneMonck on 20/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_WINDOW_H
#define VULKANGAMEENGINETUTORIAL_WINDOW_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

class Window {

public:
  Window(int width, int height, std::string windowName);

  ~Window();

  // don't accidentally copy Window leading to two pointers to GLFWWindow, and
  // making a dangling pointer
  Window(const Window &) = delete;

  Window &operator=(const Window &) = delete;

  bool ShouldClose();

  VkExtent2D GetExtent();

  void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

  bool WasWindowResized() { return frameBufferResized; };

  void ResetWindowResizedFlag() { frameBufferResized = false; };

  GLFWwindow *GetGLFWWindow() const { return window; };

private:
  static void frameBufferResizedCallback(GLFWwindow *instance, int width,
                                         int height);
  int width;
  int height;
  bool frameBufferResized = false;
  std::string windowName;
  GLFWwindow *window;

  void initWindow();
};

#endif // VULKANGAMEENGINETUTORIAL_WINDOW_H
