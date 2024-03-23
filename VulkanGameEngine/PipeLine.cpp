//
// Created by ShaneMonck on 20/03/2024.
//

#include <fstream>
#include <iostream>
#include "PipeLine.h"
#include <cstdio>
#include <cassert>
#include "Model.h"

// #region Private Methods
std::vector<char> PipeLine::ReadFile(const std::string &filePath) {
    std::ifstream file = std::ifstream(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + filePath);
    }

    std::streamsize fileSize = static_cast<std::streamsize>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

void PipeLine::CreateGraphicsPipeline(const std::string &vertexFilePath, const std::string &fragmentFilePath,
                                      const PipeLineConfigInfo &configInfo) {

    assert(configInfo.pipelineLayout != VK_NULL_HANDLE &&
           "Cannot create graphics pipeline:: no pipelineLayout provided in configInfo");

    assert(configInfo.renderPass != VK_NULL_HANDLE &&
           "Cannot create graphics pipeline:: no renderPass provided in configInfo");

    auto vertexCode = ReadFile(vertexFilePath);
    auto fragmentCode = ReadFile(fragmentFilePath);

    CreateShaderModule(vertexCode, &vertexShaderModule);
    CreateShaderModule(fragmentCode, &fragmentShaderModule);

    VkPipelineShaderStageCreateInfo shaderStages[2];
    // Vertex
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertexShaderModule;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;
    // Fragment
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragmentShaderModule;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    auto bindingDescriptions = Model::Vertex::GetBindingDescriptions();
    auto attributeDescriptions = Model::Vertex::GetAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = VkPipelineVertexInputStateCreateInfo();
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

    VkPipelineViewportStateCreateInfo viewportInfo = VkPipelineViewportStateCreateInfo();
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.viewportCount = 1;
    viewportInfo.pViewports = &configInfo.viewport;
    viewportInfo.scissorCount = 1;
    viewportInfo.pScissors = &configInfo.scissor;

    VkGraphicsPipelineCreateInfo pipelineInfo = VkGraphicsPipelineCreateInfo();
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportInfo;
    pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
    pipelineInfo.pMultisampleState = &configInfo.multiSampleInfo;
    pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
    pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
    pipelineInfo.pDynamicState = nullptr;

    pipelineInfo.layout = configInfo.pipelineLayout;
    pipelineInfo.renderPass = configInfo.renderPass;
    pipelineInfo.subpass = configInfo.subPass;

    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(engineDevice.device(),
                                  VK_NULL_HANDLE,
                                  1,
                                  &pipelineInfo,
                                  nullptr,
                                  &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create graphics pipeline");
    }
}


void PipeLine::CreateShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule) {
    VkShaderModuleCreateInfo createInfo = VkShaderModuleCreateInfo();
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    if (vkCreateShaderModule(engineDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module");
    }
}

// #endRegion

// #region Constructors
PipeLine::PipeLine(EngineDevice &engineDevice, const std::string &vertexFilePath, const std::string &fragmentFilePath,
                   const PipeLineConfigInfo &configInfo) : engineDevice(engineDevice) {

    CreateGraphicsPipeline(vertexFilePath, fragmentFilePath, configInfo);
}


PipeLine::~PipeLine() {
    vkDestroyShaderModule(engineDevice.device(), vertexShaderModule, nullptr);
    vkDestroyShaderModule(engineDevice.device(), fragmentShaderModule, nullptr);
    vkDestroyPipeline(engineDevice.device(), graphicsPipeline, nullptr);
}

// #endRegion

// #region Public Methods

PipeLineConfigInfo PipeLine::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
    PipeLineConfigInfo configInfo = PipeLineConfigInfo();

    // 3 vertices in a list per triangle 
    configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    // ViewPort
    // Transformation between pipeline output and our target image
    configInfo.viewport.x = 0.0f;
    configInfo.viewport.y = 0.0f;
    configInfo.viewport.width = static_cast<float>(width);
    configInfo.viewport.height = static_cast<float>(height);
    configInfo.viewport.minDepth = 0.0f;
    configInfo.viewport.maxDepth = 1.0f;
    // any pixels outside our viewport width and height will be cut
    configInfo.scissor.offset = {0, 0};
    configInfo.scissor.extent = {width, height};

    // Rasterization
    configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    configInfo.rasterizationInfo.lineWidth = 1.0f;
    configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE; // Double Sided triangles 
    configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
    configInfo.rasterizationInfo.depthBiasClamp = 0.0f; // Optional
    configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

    // Aliasing
    configInfo.multiSampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    configInfo.multiSampleInfo.sampleShadingEnable = VK_FALSE;
    configInfo.multiSampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    configInfo.multiSampleInfo.minSampleShading = 1.0f; // Optional
    configInfo.multiSampleInfo.pSampleMask = nullptr; // Optional
    configInfo.multiSampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
    configInfo.multiSampleInfo.alphaToOneEnable = VK_FALSE;

    configInfo.colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT
            | VK_COLOR_COMPONENT_A_BIT;
    configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
    configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
    configInfo.colorBlendInfo.attachmentCount = 1;
    configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
    configInfo.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
    configInfo.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
    configInfo.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
    configInfo.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

    configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.minDepthBounds = 0.0f; // Optional
    configInfo.depthStencilInfo.maxDepthBounds = 1.0f; // Optional
    configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.front = {}; // Optional
    configInfo.depthStencilInfo.back = {}; // Optional

    return configInfo;
}

void PipeLine::Bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
}

// #endRegion


