//
// Created by ShaneMonck on 20/03/2024.
//
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include "FirstApp.h"


struct SimplePushConstantData {
    glm::mat2 transform = glm::mat2(1.f); // default identity matrix
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

// #region Private Methods
void FirstApp::createPipelineLayout() {
    VkPushConstantRange pushConstantRange = VkPushConstantRange();
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = VkPipelineLayoutCreateInfo();
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void FirstApp::createPipeline() {
    assert(engineSwapChain != nullptr && "Cannot create pipeline before swap chain");
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipeLineConfigInfo pipeLineConfig{};
    PipeLine::defaultPipelineConfigInfo(pipeLineConfig);
    pipeLineConfig.renderPass = engineSwapChain->getRenderPass();
    pipeLineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<PipeLine>(engineDevice,
                                          "shaders/simple_shader_vert.glsl.spv",
                                          "shaders/simple_shader_frag.glsl.spv",
                                          pipeLineConfig);
}

void FirstApp::createCommandBuffers() {
    commandBuffers.resize(engineSwapChain->imageCount());

    VkCommandBufferAllocateInfo allocateInfo = VkCommandBufferAllocateInfo();
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool = engineDevice.getCommandPool();
    allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(engineDevice.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

}

void FirstApp::freeCommandBuffers() {
    vkFreeCommandBuffers(engineDevice.device(),
                         engineDevice.getCommandPool(),
                         static_cast<uint32_t>( commandBuffers.size()),
                         commandBuffers.data());

    commandBuffers.clear();
}


void FirstApp::drawFame() {
    uint32_t imageIndex;
    auto result = engineSwapChain->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    recordCommandBuffer(imageIndex);
    result = engineSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.WasWindowResized()) {
        window.ResetWindowResizedFlag();
        recreateSwapChain();
        return;
    }

    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image");
    }
}

void FirstApp::loadGameObjects() {
    std::vector<Model::Vertex> vertices = {
            {{0.0f,  -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f,  0.5f},  {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}}
    };

    auto model = std::make_shared<Model>(engineDevice, vertices);

    auto triangle = GameObject::createGameObject();
    triangle.model = model;
    triangle.color = glm::vec3(.1f, .8f, .1f);
    triangle.transform2DComponent.Translation.x = .2f;
    triangle.transform2DComponent.Scale = glm::vec2(2.f, .5f);
    triangle.transform2DComponent.Rotation = .25f * glm::two_pi<float>();

    gameObjects.push_back(std::move(triangle));
}

void FirstApp::recordCommandBuffer(int imageIndex) {
    VkCommandBufferBeginInfo beginInfo = VkCommandBufferBeginInfo();
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to being recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassBeginInfo = VkRenderPassBeginInfo();
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = engineSwapChain->getRenderPass();
    renderPassBeginInfo.framebuffer = engineSwapChain->getFrameBuffer(imageIndex);

    renderPassBeginInfo.renderArea.offset = VkOffset2D(0, 0);
    renderPassBeginInfo.renderArea.extent = engineSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues = std::array<VkClearValue, 2>();
    clearValues[0].color = VkClearColorValue{0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = VkClearDepthStencilValue{1.0f, 0};
    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassBeginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport = VkViewport();
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    VkExtent2D extent = engineSwapChain->getSwapChainExtent();
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    auto scissor = VkRect2D(VkOffset2D(0, 0), extent);
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

    renderGameObjects(commandBuffers[imageIndex]);

    vkCmdEndRenderPass(commandBuffers[imageIndex]);

    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void FirstApp::recreateSwapChain() {
    auto extent = window.GetExtent();
    // wait for minimization to finish
    while (extent.width == 0 || extent.height == 0) {
        extent = window.GetExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(engineDevice.device());

    if (engineSwapChain == nullptr) {
        engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent);
    } else {
        engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent, std::move(engineSwapChain));
        if (engineSwapChain->imageCount() != commandBuffers.size()) {
            freeCommandBuffers();
            createCommandBuffers();
        }
    }

    //engineSwapChain = nullptr;


    // if render pass compatible do nothing else
    createPipeline();
}

void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer) {
    pipeline->Bind(commandBuffer);

    for (auto &obj: gameObjects) {
        obj.transform2DComponent.Rotation = glm::mod(obj.transform2DComponent.Rotation + 0.001f, glm::two_pi<float>());

        SimplePushConstantData pushConstantData = SimplePushConstantData();
        pushConstantData.offset = obj.transform2DComponent.Translation;
        pushConstantData.color = obj.color;
        pushConstantData.transform = obj.transform2DComponent.mat2();

        vkCmdPushConstants(commandBuffer,
                           pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                           0,
                           sizeof(SimplePushConstantData),
                           &pushConstantData);
        obj.model->Bind(commandBuffer);
        obj.model->Draw(commandBuffer);
    }
}


// #endregion

// #region Constructors

FirstApp::FirstApp() {
    loadGameObjects();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
}

FirstApp::~FirstApp() {
    vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr);
}

// #endregion

// #region Public Methods
void FirstApp::Run() {
    while (!window.ShouldClose()) {
        // listen for keystrokes or close button
        glfwPollEvents();
        drawFame();
    }

    vkDeviceWaitIdle(engineDevice.device());
}




// #endregion

