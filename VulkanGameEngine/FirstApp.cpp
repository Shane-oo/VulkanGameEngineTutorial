//
// Created by ShaneMonck on 20/03/2024.
//

#include <stdexcept>
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

}

void FirstApp::drawFame() {

}

// #endregion

// #region Constructors

FirstApp::FirstApp() {
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
    }
}

// #endregion

