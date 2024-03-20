//
// Created by ShaneMonck on 20/03/2024.
//

#include "Window.h"

#include <utility>

void Window::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

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

bool Window::ShouldClose() {
    return glfwWindowShouldClose(window);
}
