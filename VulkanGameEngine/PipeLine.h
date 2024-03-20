//
// Created by ShaneMonck on 20/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_PIPELINE_H
#define VULKANGAMEENGINETUTORIAL_PIPELINE_H


#include <string>
#include <vector>
#include "EngineDevice.h"

struct PipeLineConfigInfo {

};

class PipeLine {
private:
    static std::vector<char> ReadFile(const std::string &filePath);

    void CreateGraphicsPipeline(const std::string &vertexFilePath, const std::string &fragmentFilePath,
                                const PipeLineConfigInfo &configInfo);

    void CreateShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

    EngineDevice &engineDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertexShaderModule;
    VkShaderModule fragmentShaderModule;

public:
    PipeLine(EngineDevice &engineDevice, const std::string &vertexFilePath, const std::string &fragmentFilePath,
             const PipeLineConfigInfo &configInfo);

    ~PipeLine();

    PipeLine(const PipeLine &) = delete;

    PipeLine operator=(const PipeLine &) = delete;

    static PipeLineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
};


#endif //VULKANGAMEENGINETUTORIAL_PIPELINE_H
