//
// Created by ShaneMonck on 27/03/2024.
//

//
// Created by ShaneMonck on 20/03/2024.
//
#include "SimpleRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>

struct SimplePushConstantData {
  glm::mat4 transform = glm::mat4(1.f); // default identity matrix
  alignas(16) glm::vec3 color;
};

// #region Private Methods
void SimpleRenderSystem::createPipelineLayout() {
  VkPushConstantRange pushConstantRange = VkPushConstantRange();
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo = VkPipelineLayoutCreateInfo();
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

  if (vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo,
                             nullptr, &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout != nullptr &&
         "Cannot create pipeline before pipeline layout");

  PipeLineConfigInfo pipeLineConfig{};
  PipeLine::defaultPipelineConfigInfo(pipeLineConfig);
  pipeLineConfig.renderPass = renderPass;
  pipeLineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<PipeLine>(
      engineDevice, "shaders/simple_shader_vert.glsl.spv",
      "shaders/simple_shader_frag.glsl.spv", pipeLineConfig);
}

// #endregion

// #region Constructors

SimpleRenderSystem::SimpleRenderSystem(EngineDevice &engineDevice,
                                       VkRenderPass renderPass)
    : engineDevice{engineDevice} {
  createPipelineLayout();
  createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem() {
  vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr);
}

// #endregion

// #region Public Methods

void SimpleRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer,
                                           std::vector<GameObject> &gameObjects,
                                           const Camera &camera) {
  pipeline->Bind(commandBuffer);

  auto projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
  
  for (auto &obj : gameObjects) {
    SimplePushConstantData pushConstantData = SimplePushConstantData();
    pushConstantData.color = obj.color;
    pushConstantData.transform = projectionView * obj.transformComponent.mat4();

    vkCmdPushConstants(commandBuffer, pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(SimplePushConstantData), &pushConstantData);
    obj.model->Bind(commandBuffer);
    obj.model->Draw(commandBuffer);
  }
}

// #endregion
