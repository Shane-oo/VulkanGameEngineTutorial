//
// Created by ShaneMonck on 20/03/2024.
//

#include <fstream>
#include <iostream>
#include "PipeLine.h"
#include <cstdio>

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
    auto vertexCode = ReadFile(vertexFilePath);
    auto fragmentCode = ReadFile(fragmentFilePath);

    printf("Vertex Shader Code Size %zu\n", vertexCode.size());
    printf("Fragment Shader Code Size %zu\n", fragmentCode.size());
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

}

// #endRegion

// #region Public Methods

PipeLineConfigInfo PipeLine::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
    PipeLineConfigInfo configInfo = PipeLineConfigInfo();
    
    return configInfo;
}

// #endRegion


