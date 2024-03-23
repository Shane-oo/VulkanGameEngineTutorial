//
// Created by ShaneMonck on 20/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_FIRSTAPP_H
#define VULKANGAMEENGINETUTORIAL_FIRSTAPP_H


#include <memory>
#include "Window.h"
#include "PipeLine.h"
#include "EngineSwapChain.hpp"

class FirstApp {
private:
    Window window = Window(WIDTH, HEIGHT, "Hello Vulkan!");
    EngineDevice engineDevice = EngineDevice(window);
    EngineSwapChain engineSwapChain = EngineSwapChain(engineDevice, window.GetExtent());
    std::unique_ptr<PipeLine> pipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;

    void createPipelineLayout();

    void createPipeline();

    void createCommandBuffers();

    void drawFame();

public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();

    ~FirstApp();

    FirstApp(const FirstApp &) = delete;

    FirstApp &operator=(const FirstApp &) = delete;

    void Run();
};


#endif //VULKANGAMEENGINETUTORIAL_FIRSTAPP_H
