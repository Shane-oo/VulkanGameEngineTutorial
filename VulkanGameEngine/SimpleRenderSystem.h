//
// Created by ShaneMonck on 27/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_SIMPLERENDERSYSTEM_H
#define VULKANGAMEENGINETUTORIAL_SIMPLERENDERSYSTEM_H

#include "Camera.h"
#include "EngineDevice.h"
#include "GameObject.h"
#include "PipeLine.h"
#include "FrameInfo.h"

class SimpleRenderSystem {
public:

    SimpleRenderSystem(EngineDevice &engineDevice, VkRenderPass renderPass);

    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem &) = delete;

    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void RenderGameObjects(FrameInfo &frameInfo,std::vector<GameObject> &gameObjects);

private:
    EngineDevice &engineDevice;
    std::unique_ptr<PipeLine> pipeline;
    VkPipelineLayout pipelineLayout;


    void createPipelineLayout();

    void createPipeline(VkRenderPass renderPass);

};


#endif //VULKANGAMEENGINETUTORIAL_SIMPLERENDERSYSTEM_H
