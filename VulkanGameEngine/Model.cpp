//
// Created by ShaneMonck on 23/03/2024.
//


#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>

#include <stdexcept>
#include "Model.h"
#include "Utils.h"
#include <unordered_map>

namespace std {
    template<>
    struct hash<Model::Vertex> {
        size_t operator()(Model::Vertex const &vertex) const {
            size_t seed = 0;
            hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
}

// #region Private Methods

void Model::createVertexBuffers(const std::vector<Vertex> &vertices) {
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be at least 3");

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;


    uint32_t vertexSize = sizeof(vertices[0]);
    Buffer stagingBuffer = Buffer(engineDevice,
                                  vertexSize,
                                  vertexCount,
                                  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                  | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT // cpu | gpu
    );

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void *) vertices.data());

    vertexBuffer = std::make_unique<Buffer>(engineDevice,
                                            vertexSize,
                                            vertexCount,
                                            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    engineDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
}

void Model::createIndexBuffers(const std::vector<uint32_t> &indices) {
    indexCount = static_cast<uint32_t>(indices.size());
    hasIndexBuffer = indexCount > 0;

    if (!hasIndexBuffer) {
        return;
    }

    VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

    uint32_t indexSize = sizeof(indices[0]);
    Buffer stagingBuffer = Buffer(engineDevice,
                                  indexSize,
                                  indexCount,
                                  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT // cpu | gpu 
    );

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void *) indices.data());

    indexBuffer = std::make_unique<Buffer>(
            engineDevice,
            indexSize,
            indexCount,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    engineDevice.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
}

// #endregion

// #region Constructors
Model::Model(EngineDevice &device, const Model::Builder &builder)
        : engineDevice(device) {
    createVertexBuffers(builder.vertices);
    createIndexBuffers(builder.indices);
}

Model::~Model() {
}

// #endregion

// #region Public Methods

std::vector<VkVertexInputBindingDescription>
Model::Vertex::GetBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription>
Model::Vertex::GetAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions = std::vector<VkVertexInputAttributeDescription>();

    attributeDescriptions.push_back({0,
                                     0,
                                     VK_FORMAT_R32G32B32_SFLOAT,
                                     static_cast<uint32_t>(offsetof(Vertex, position))
                                    });
    attributeDescriptions.push_back({1,
                                     0,
                                     VK_FORMAT_R32G32B32_SFLOAT,
                                     static_cast<uint32_t>(offsetof(Vertex, color))
                                    });
    attributeDescriptions.push_back({2,
                                     0,
                                     VK_FORMAT_R32G32B32_SFLOAT,
                                     static_cast<uint32_t>(offsetof(Vertex, normal))
                                    });
    attributeDescriptions.push_back({3,
                                     0,
                                     VK_FORMAT_R32G32_SFLOAT,
                                     static_cast<uint32_t>(offsetof(Vertex, uv))
                                    });

    return attributeDescriptions;
}

void Model::Draw(VkCommandBuffer commandBuffer) {
    if (hasIndexBuffer) {
        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    } else {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }
}

void Model::Bind(VkCommandBuffer commandBuffer) {
    VkBuffer buffers[] = {vertexBuffer->getBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

    if (hasIndexBuffer) {
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
}

std::unique_ptr<Model> Model::CreateModelFromFile(EngineDevice &device, const std::string &filePath) {
    Builder builder = Builder();
    builder.loadModel(filePath);

    printf("Vertex count: %zu", builder.vertices.size());

    return std::make_unique<Model>(device, builder);
}


void Model::Builder::loadModel(const std::string &filePath) {
    tinyobj::attrib_t attrib = tinyobj::attrib_t();
    std::vector<tinyobj::shape_t> shapes = std::vector<tinyobj::shape_t>();
    std::vector<tinyobj::material_t> materials = std::vector<tinyobj::material_t>();
    std::string warn, err = std::string();

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices = std::unordered_map<Vertex, uint32_t>();

    for (const auto &shape: shapes) {
        for (const auto &index: shape.mesh.indices) {
            Vertex vertex = Vertex();

            if (index.vertex_index >= 0) {
                vertex.position = glm::vec3(attrib.vertices[3 * index.vertex_index + 0],
                                            attrib.vertices[3 * index.vertex_index + 1],
                                            attrib.vertices[3 * index.vertex_index + 2]);

                vertex.color = glm::vec3(attrib.colors[3 * index.vertex_index + 0],
                                         attrib.colors[3 * index.vertex_index + 1],
                                         attrib.colors[3 * index.vertex_index + 2]);

            }
            if (index.normal_index >= 0) {
                vertex.normal = glm::vec3(attrib.normals[3 * index.normal_index + 0],
                                          attrib.normals[3 * index.normal_index + 1],
                                          attrib.normals[3 * index.normal_index + 2]);

            }
            if (index.texcoord_index >= 0) {
                vertex.uv = glm::vec2(attrib.texcoords[2 * index.texcoord_index + 0],
                                      attrib.texcoords[2 * index.texcoord_index + 1]);
            }

            // index vertices
            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

// #endregion
