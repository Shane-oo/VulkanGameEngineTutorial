//
// Created by ShaneMonck on 16/04/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_FRAMEINFO_H
#define VULKANGAMEENGINETUTORIAL_FRAMEINFO_H


#include <vulkan/vulkan_core.h>
#include "Camera.h"

struct FrameInfo {
   int frameIndex; 
   float frameTime;
   VkCommandBuffer commandBuffer;
   Camera &camera;
   VkDescriptorSet globalDescriptorSet;
   GameObject::Map &gameObjects;
};

#endif //VULKANGAMEENGINETUTORIAL_FRAMEINFO_H
