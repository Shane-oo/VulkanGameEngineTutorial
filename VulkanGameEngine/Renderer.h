//
// Created by ShaneMonck on 27/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_RENDERER_H
#define VULKANGAMEENGINETUTORIAL_RENDERER_H


#include <memory>
#include <cassert>
#include "Window.h"
#include "EngineSwapChain.hpp"


class Renderer {
public:
    Renderer(Window &window, EngineDevice &device);

    ~Renderer();

    Renderer(const Renderer &) = delete;

    Renderer &operator=(const Renderer &) = delete;

    VkRenderPass GetSwapChainRederPass() const { return engineSwapChain->getRenderPass(); }

    bool IsDrawFrameInProgress() const { return isDrawFrameStarted; }

    VkCommandBuffer GetCurrentCommandBuffer() const {
        assert(isDrawFrameStarted && "Cannot get command buffer when draw frame not in progress");
        return commandBuffers[currentFrameIndex];
    }

    VkCommandBuffer BeginDrawFrame();

    void EndDrawFrame();

    void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);

    void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

    int GetFrameIndex() const {
        assert(isDrawFrameStarted && "Cannot GetFrameIndex when draw frame not in progress");
        return currentFrameIndex;
    }

private:
    Window &window;
    EngineDevice &engineDevice;
    std::unique_ptr<EngineSwapChain> engineSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;
    uint32_t currentImageIndex;
    int currentFrameIndex = 0;
    bool isDrawFrameStarted = false;

    void createCommandBuffers();

    void freeCommandBuffers();

    void recreateSwapChain();

};


#endif //VULKANGAMEENGINETUTORIAL_RENDERER_H
