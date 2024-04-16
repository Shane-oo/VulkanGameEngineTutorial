//
// Created by ShaneMonck on 16/04/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_DESCRIPTORPOOL_H
#define VULKANGAMEENGINETUTORIAL_DESCRIPTORPOOL_H


#include <memory>
#include "../EngineDevice.h"

class DescriptorPool {
public:
    class Builder {
    public:
        Builder(EngineDevice &engineDevice) : engineDevice{engineDevice} {}

        Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);

        Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);

        Builder &setMaxSets(uint32_t count);

        std::unique_ptr<DescriptorPool> build() const;

    private:
        EngineDevice &engineDevice;
        std::vector<VkDescriptorPoolSize> poolSizes{};
        uint32_t maxSets = 1000;
        VkDescriptorPoolCreateFlags poolFlags = 0;
    };

    DescriptorPool(
            EngineDevice &engineDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize> &poolSizes);

    ~DescriptorPool();

    DescriptorPool(const DescriptorPool &) = delete;

    DescriptorPool &operator=(const DescriptorPool &) = delete;

    bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

    void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

    void resetPool();

private:
    EngineDevice &engineDevice;
    VkDescriptorPool descriptorPool;

    friend class DescriptorWriter;
};

#endif //VULKANGAMEENGINETUTORIAL_DESCRIPTORPOOL_H
