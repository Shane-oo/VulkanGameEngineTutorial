//
// Created by ShaneMonck on 20/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_FIRSTAPP_H
#define VULKANGAMEENGINETUTORIAL_FIRSTAPP_H


#include "Window.h"
#include "PipeLine.h"

class FirstApp {
private:
    Window window = Window(WIDTH, HEIGHT, "Hello Vulkan!");
    EngineDevice engineDevice = EngineDevice(window);
    PipeLine pipeLine = PipeLine(engineDevice, "shaders/simple_shader_vert.glsl.spv",
                                 "shaders/simple_shader_frag.glsl.spv",
                                 PipeLine::defaultPipelineConfigInfo(WIDTH, HEIGHT));

public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void Run();
};


#endif //VULKANGAMEENGINETUTORIAL_FIRSTAPP_H
