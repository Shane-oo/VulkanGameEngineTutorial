//
// Created by ShaneMonck on 20/03/2024.
//

#include "Window.h"

#include <utility>
#include <stdexcept>

// #region Private Methods

void Window::frameBufferResizedCallback(GLFWwindow *instance, int width, int height) {
    auto window = reinterpret_cast<class Window *>(glfwGetWindowUserPointer(instance));
    window->frameBufferResized = true;
    window->width = width;
    window->height = height;
}

void Window::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
}

// #endregion

// #region Constructors
Window::Window(int width, int height, std::string windowName) {
    this->width = width;
    this->height = height;
    this->windowName = std::move(windowName);

    initWindow();
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// #endregion

// #region Public Methods

bool Window::ShouldClose() {
    return glfwWindowShouldClose(window);
}

void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Window surface");
    }
}

VkExtent2D Window::GetExtent() {
    return VkExtent2D(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}


// #endregion