//
// Created by ShaneMonck on 30/04/2024.
//

#include "PointLightSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>

// #region Private Methods
void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
/*    VkPushConstantRange pushConstantRange = VkPushConstantRange();
    pushConstantRange.stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);*/

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};


    VkPipelineLayoutCreateInfo pipelineLayoutInfo = VkPipelineLayoutCreateInfo();
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo,
                               nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void PointLightSystem::createPipeline(VkRenderPass renderPass) {
    assert(pipelineLayout != nullptr &&
           "Cannot create pipeline before pipeline layout");

    PipeLineConfigInfo pipeLineConfig{};
    PipeLine::defaultPipelineConfigInfo(pipeLineConfig);
    pipeLineConfig.attributeDescriptions.clear();
    pipeLineConfig.bindingDescriptions.clear();
    pipeLineConfig.renderPass = renderPass;
    pipeLineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<PipeLine>(
            engineDevice, "shaders/point_light_vert.glsl.spv",
            "shaders/point_light_frag.glsl.spv", pipeLineConfig);
}

// #endregion

// #region Constructors

PointLightSystem::PointLightSystem(EngineDevice &engineDevice,
                                   VkRenderPass renderPass,
                                   VkDescriptorSetLayout globalSetLayout)
        : engineDevice{engineDevice} {
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

PointLightSystem::~PointLightSystem() {
    vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr);
}

// #endregion

// #region Public Methods

void PointLightSystem::Render(FrameInfo &frameInfo) {
    pipeline->Bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            pipelineLayout,
                            0,
                            1,
                            &frameInfo.globalDescriptorSet,
                            0,
                            nullptr);

    vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    
}

// #endregion
