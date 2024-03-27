//
// Created by ShaneMonck on 27/03/2024.
//

#include <stdexcept>
#include <array>
#include "Renderer.h"


// #region Private Methods


void Renderer::createCommandBuffers() {
    commandBuffers.resize(EngineSwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocateInfo = VkCommandBufferAllocateInfo();
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool = engineDevice.getCommandPool();
    allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(engineDevice.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

}

void Renderer::freeCommandBuffers() {
    vkFreeCommandBuffers(engineDevice.device(),
                         engineDevice.getCommandPool(),
                         static_cast<uint32_t>( commandBuffers.size()),
                         commandBuffers.data());

    commandBuffers.clear();
}


void Renderer::recreateSwapChain() {
    auto extent = window.GetExtent();
    // wait for minimization to finish                              C
    while (extent.width == 0 || extent.height == 0) {
        extent = window.GetExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(engineDevice.device());

    if (engineSwapChain == nullptr) {
        engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent);
    } else {
        std::shared_ptr<EngineSwapChain> oldSwapChain = std::move(engineSwapChain);
        engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent, oldSwapChain);

        if (!oldSwapChain->CompareSwapFormats(*engineSwapChain)) {
            throw std::runtime_error("Swap chain image (or depth) format has changed!");
        }
    }

    // todo

}

// #endregion

// #region Constructors

Renderer::Renderer(Window &window, EngineDevice &engineDevice) : window{window}, engineDevice{engineDevice} {
    recreateSwapChain();
    createCommandBuffers();
}

Renderer::~Renderer() {
    freeCommandBuffers();
}

// #endregion

// #region Public Methods

VkCommandBuffer Renderer::BeginDrawFrame() {
    assert(!isDrawFrameStarted && "Can't call BeginDrawFrame While already in progress");

    auto result = engineSwapChain->acquireNextImage(&currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    isDrawFrameStarted = true;

    auto commandBuffer = GetCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo = VkCommandBufferBeginInfo();
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to being recording command buffer!");
    }

    return commandBuffer;
}

void Renderer::EndDrawFrame() {
    assert(isDrawFrameStarted && "Can't call EndDrawFrame while frame is not in progress");

    auto commandBuffer = GetCurrentCommandBuffer();
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    auto result = engineSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.WasWindowResized()) {
        window.ResetWindowResizedFlag();
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image");
    }

    isDrawFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1 ) % EngineSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isDrawFrameStarted && "Can't call BeginSwapChainRenderPass if frame is not in progress");
    assert(commandBuffer == GetCurrentCommandBuffer() &&
           "Can't begin render pass on command buffer from a different frame");

    VkRenderPassBeginInfo renderPassBeginInfo = VkRenderPassBeginInfo();
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = engineSwapChain->getRenderPass();
    renderPassBeginInfo.framebuffer = engineSwapChain->getFrameBuffer(currentImageIndex);

    renderPassBeginInfo.renderArea.offset = VkOffset2D(0, 0);
    renderPassBeginInfo.renderArea.extent = engineSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues = std::array<VkClearValue, 2>();
    clearValues[0].color = VkClearColorValue{0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = VkClearDepthStencilValue{1.0f, 0};
    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassBeginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport = VkViewport();
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    VkExtent2D extent = engineSwapChain->getSwapChainExtent();
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    auto scissor = VkRect2D(VkOffset2D(0, 0), extent);
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isDrawFrameStarted && "Can't call EndSwapChainRenderPass if frame is not in progress");
    assert(commandBuffer == GetCurrentCommandBuffer() &&
           "Can't end render pass on command buffer from a different frame");

    vkCmdEndRenderPass(commandBuffer);
}


// #endregion

