//
// Created by ShaneMonck on 16/04/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_DESCRIPTORWRITER_H
#define VULKANGAMEENGINETUTORIAL_DESCRIPTORWRITER_H


#include "DescriptorSetLayout.h"
#include "DescriptorPool.h"

class DescriptorWriter {
public:
    DescriptorWriter(DescriptorSetLayout &setLayout, DescriptorPool &pool);

    DescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);

    DescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

    bool build(VkDescriptorSet &set);

    void overwrite(VkDescriptorSet &set);

private:
    DescriptorSetLayout &setLayout;
    DescriptorPool &pool;
    std::vector<VkWriteDescriptorSet> writes;
};



#endif //VULKANGAMEENGINETUTORIAL_DESCRIPTORWRITER_H
