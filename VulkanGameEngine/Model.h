//
// Created by ShaneMonck on 23/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_MODEL_H
#define VULKANGAMEENGINETUTORIAL_MODEL_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "EngineDevice.h"
#include "Buffer.h"

class Model {

public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal;
        glm::vec2 uv;


        static std::vector<VkVertexInputBindingDescription>
        GetBindingDescriptions();

        static std::vector<VkVertexInputAttributeDescription>
        GetAttributeDescriptions();

        bool operator==(const Vertex &other) const {
            return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
        }
    };

    struct Builder {
        std::vector<Vertex> vertices = std::vector<Vertex>();
        std::vector<uint32_t> indices = std::vector<uint32_t>();

        void loadModel(const std::string &filePath);
    };

    Model(EngineDevice &device, const Model::Builder &builder);

    ~Model();

    static std::unique_ptr<Model> CreateModelFromFile(EngineDevice &device, const std::string &filePath);

    Model(const Model &) = delete;

    Model &operator=(const Model &) = delete;

    void Bind(VkCommandBuffer commandBuffer);

    void Draw(VkCommandBuffer commandBuffer);

private:
    EngineDevice &engineDevice;
    
    std::unique_ptr<Buffer> vertexBuffer;
    uint32_t vertexCount;

    bool hasIndexBuffer = false;
    std::unique_ptr<Buffer> indexBuffer;
    uint32_t indexCount;

    void createVertexBuffers(const std::vector<Vertex> &vertices);

    void createIndexBuffers(const std::vector<uint32_t> &indices);
};

#endif // VULKANGAMEENGINETUTORIAL_MODEL_H
