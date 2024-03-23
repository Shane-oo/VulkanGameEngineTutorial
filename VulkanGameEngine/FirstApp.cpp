//
// Created by ShaneMonck on 20/03/2024.
//

#include <stdexcept>
#include <array>
#include "FirstApp.h"

// #region Private Methods
void FirstApp::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = VkPipelineLayoutCreateInfo();
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void FirstApp::createPipeline() {
    auto pipelineConfig = PipeLine::defaultPipelineConfigInfo(engineSwapChain.width(), engineSwapChain.height());
    pipelineConfig.renderPass = engineSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<PipeLine>(engineDevice,
                                          "shaders/simple_shader_vert.glsl.spv",
                                          "shaders/simple_shader_frag.glsl.spv",
                                          pipelineConfig);
}

void FirstApp::createCommandBuffers() {
    commandBuffers.resize(engineSwapChain.imageCount());

    VkCommandBufferAllocateInfo allocateInfo = VkCommandBufferAllocateInfo();
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool = engineDevice.getCommandPool();
    allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(engineDevice.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (int i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo = VkCommandBufferBeginInfo();
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to being recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassBeginInfo = VkRenderPassBeginInfo();
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = engineSwapChain.getRenderPass();
        renderPassBeginInfo.framebuffer = engineSwapChain.getFrameBuffer(i);

        renderPassBeginInfo.renderArea.offset = VkOffset2D(0, 0);
        renderPassBeginInfo.renderArea.extent = engineSwapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues = std::array<VkClearValue, 2>();
        clearValues[0].color = VkClearColorValue{0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = VkClearDepthStencilValue{1.0f, 0};
        renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassBeginInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        pipeline->Bind(commandBuffers[i]);
        model->Bind(commandBuffers[i]);
        model->Draw(commandBuffers[i]);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void FirstApp::drawFame() {
    uint32_t imageIndex;
    auto result = engineSwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    result = engineSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image");
    }
}

void FirstApp::loadModels() {
    std::vector<Model::Vertex> vertices = {
            {{-0.25f,  -0.75f}},
            {{0.25f,  0.25f}},
            {{-0.75f, 0.25f}},

            {{0.25f,  0.75f}},
            {{-0.25f,  -0.25f}},
            {{0.75f, -0.25f}},
    };
    
    model = std::make_unique<Model>(engineDevice, vertices);
}

// #endregion

// #region Constructors

FirstApp::FirstApp() {
    loadModels();
    createPipelineLayout();
    createPipeline();
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

