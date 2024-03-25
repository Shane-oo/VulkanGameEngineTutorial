//
// Created by ShaneMonck on 20/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_FIRSTAPP_H
#define VULKANGAMEENGINETUTORIAL_FIRSTAPP_H


#include <memory>
#include "Window.h"
#include "PipeLine.h"
#include "EngineSwapChain.hpp"
#include "Model.h"

class FirstApp {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();

    ~FirstApp();

    FirstApp(const FirstApp &) = delete;

    FirstApp &operator=(const FirstApp &) = delete;

    void Run();

private:
    Window window = Window(WIDTH, HEIGHT, "Hello Vulkan!");
    EngineDevice engineDevice = EngineDevice(window);
    std::unique_ptr<EngineSwapChain> engineSwapChain;
    std::unique_ptr<PipeLine> pipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::unique_ptr<Model> model;


    void createPipelineLayout();

    void createPipeline();

    void createCommandBuffers();

    void freeCommandBuffers();
    
    void drawFame();
    
    void loadModels();
    
    void recreateSwapChain();
    void recordCommandBuffer(int imageIndex);
};


#endif //VULKANGAMEENGINETUTORIAL_FIRSTAPP_H
