//
// Created by ShaneMonck on 30/04/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_POINTLIGHTSYSTEM_H
#define VULKANGAMEENGINETUTORIAL_POINTLIGHTSYSTEM_H


#include "../Camera.h"
#include "../EngineDevice.h"
#include "../GameObject.h"
#include "../PipeLine.h"
#include "../FrameInfo.h"

class PointLightSystem {
public:

    PointLightSystem(EngineDevice &engineDevice, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

    ~PointLightSystem();

    PointLightSystem(const PointLightSystem &) = delete;

    PointLightSystem &operator=(const PointLightSystem &) = delete;

    void Render(FrameInfo &frameInfo);
    
    void Update(FrameInfo &frameInfo, GlobalUbo &ubo);

private:
    EngineDevice &engineDevice;
    std::unique_ptr<PipeLine> pipeline;
    VkPipelineLayout pipelineLayout;

    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);

    void createPipeline(VkRenderPass renderPass);

};



#endif //VULKANGAMEENGINETUTORIAL_POINTLIGHTSYSTEM_H
