//
// Created by ShaneMonck on 16/04/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_FRAMEINFO_H
#define VULKANGAMEENGINETUTORIAL_FRAMEINFO_H

#define MAX_LIGHTS 10

#include <vulkan/vulkan_core.h>
#include "Camera.h"

struct PointLight {
    glm::vec4 position = glm::vec4();
    glm::vec4 color = glm::vec4();
};

struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera &camera;
    VkDescriptorSet globalDescriptorSet;
    GameObject::Map &gameObjects;
};

// keep in mind alignment rules
struct GlobalUbo {
    glm::mat4 projection = glm::mat4(1.f);
    glm::mat4 view = glm::mat4(1.f);
    glm::mat4 inverseView = glm::mat4(1.f);
    glm::vec4 ambientLightColour = glm::vec4(1.f, 1.f, 1.f, 0.0f);
    PointLight pointLights[MAX_LIGHTS];
    int numLight;
};


#endif //VULKANGAMEENGINETUTORIAL_FRAMEINFO_H
