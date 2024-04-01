//
// Created by ShaneMonck on 23/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_MODEL_H
#define VULKANGAMEENGINETUTORIAL_MODEL_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <vector>

#include "EngineDevice.h"

class Model {

public:
  struct Vertex {
    glm::vec3 position;
    glm::vec3 color;

    static std::vector<VkVertexInputBindingDescription>
    GetBindingDescriptions();

    static std::vector<VkVertexInputAttributeDescription>
    GetAttributeDescriptions();
  };

  struct Builder {
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<uint32_t> indices = std::vector<uint32_t>();
  };

  Model(EngineDevice &device, const Model::Builder &builder);

  ~Model();

  Model(const Model &) = delete;

  Model &operator=(const Model &) = delete;

  void Bind(VkCommandBuffer commandBuffer);

  void Draw(VkCommandBuffer commandBuffer);

private:
  EngineDevice &engineDevice;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;

  bool hasIndexBuffer = false;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  uint32_t indexCount;

  void createVertexBuffers(const std::vector<Vertex> &vertices);
  void createIndexBuffers(const std::vector<uint32_t> &indices);
};

#endif // VULKANGAMEENGINETUTORIAL_MODEL_H
