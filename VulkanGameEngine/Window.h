//
// Created by ShaneMonck on 20/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_WINDOW_H
#define VULKANGAMEENGINETUTORIAL_WINDOW_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

class Window {
private:
    int width;
    int height;
    std::string windowName;
    GLFWwindow *window;

    void initWindow();

public:
    Window(int width, int height, std::string windowName);

    ~Window();

    // don't accidentally copy Window leading to two pointers to GLFWWindow, and making a dangling pointer
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    
    bool ShouldClose();
};


#endif //VULKANGAMEENGINETUTORIAL_WINDOW_H
