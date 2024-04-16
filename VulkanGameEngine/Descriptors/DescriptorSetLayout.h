//
// Created by ShaneMonck on 16/04/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_DESCRIPTORSETLAYOUT_H
#define VULKANGAMEENGINETUTORIAL_DESCRIPTORSETLAYOUT_H


#include <unordered_map>
#include <memory>
#include "../EngineDevice.h"

class DescriptorSetLayout {
public:
    class Builder {
    public:
        Builder(EngineDevice &engineDevice) : device{engineDevice} {}

        Builder &addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);

        std::unique_ptr<DescriptorSetLayout> build() const;

    private:
        EngineDevice &device;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
    };

    DescriptorSetLayout(
            EngineDevice &engineDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);

    ~DescriptorSetLayout();

    DescriptorSetLayout(const DescriptorSetLayout &) = delete;

    DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

    VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

private:
    EngineDevice &engineDevice;
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

    friend class DescriptorWriter;
};


#endif //VULKANGAMEENGINETUTORIAL_DESCRIPTORSETLAYOUT_H
