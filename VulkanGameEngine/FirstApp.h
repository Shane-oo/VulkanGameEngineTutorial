//
// Created by ShaneMonck on 20/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_FIRSTAPP_H
#define VULKANGAMEENGINETUTORIAL_FIRSTAPP_H


#include "Window.h"

class FirstApp {
private:
    Window window = Window(WIDTH, HEIGHT, "Hello Vulkan!");

public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void Run();
};


#endif //VULKANGAMEENGINETUTORIAL_FIRSTAPP_H
