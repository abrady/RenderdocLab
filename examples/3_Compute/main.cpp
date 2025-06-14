#include "vulkan_compute_app.h"
#include <iostream>
#include <vector>
#include <cstring>

class ComputeExample : public VulkanComputeApp
{
public:
    ComputeExample() : VulkanComputeApp(1, 1, "Compute Example", VULKANAPP_GETSHADERDIR) {}

    void runExample()
    {
        const uint32_t NUM_ELEMENTS = 16;
        std::vector<float> data(NUM_ELEMENTS);
        std::cout << "Running compute shader example with " << NUM_ELEMENTS << " elements." << std::endl;
        for (uint32_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            data[i] = static_cast<float>(i);
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;

        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
        createBuffer(sizeof(float) * NUM_ELEMENTS,
                     VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     buffer, bufferMemory);

        void *mapped;
        vkMapMemory(device, bufferMemory, 0, VK_WHOLE_SIZE, 0, &mapped);
        memcpy(mapped, data.data(), sizeof(float) * NUM_ELEMENTS);
        vkUnmapMemory(device, bufferMemory);

        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = 0;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &layoutBinding;
        VkDescriptorSetLayout descriptorSetLayout;
        vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);

        auto code = compileShader("comp.comp", VK_SHADER_STAGE_COMPUTE_BIT);
        VkShaderModuleCreateInfo moduleInfo{};
        moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleInfo.codeSize = code.size();
        moduleInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
        VkShaderModule shaderModule;
        vkCreateShaderModule(device, &moduleInfo, nullptr, &shaderModule);

        VkPipelineShaderStageCreateInfo stage{};
        stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        stage.module = shaderModule;
        stage.pName = "main";

        VkComputePipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipelineInfo.stage = stage;
        pipelineInfo.layout = pipelineLayout;
        VkPipeline pipeline;
        vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);

        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        poolSize.descriptorCount = 1;
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = 1;
        VkDescriptorPool descriptorPool;
        vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descriptorSetLayout;
        VkDescriptorSet descriptorSet;
        vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet);

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(float) * NUM_ELEMENTS;
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = descriptorSet;
        write.dstBinding = 0;
        write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        write.descriptorCount = 1;
        write.pBufferInfo = &bufferInfo;
        vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);

        VkCommandBuffer commandBuffer = beginSingleTimeCommands();
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
        vkCmdDispatch(commandBuffer, NUM_ELEMENTS, 1, 1);
        endSingleTimeCommands(commandBuffer);

        vkMapMemory(device, bufferMemory, 0, VK_WHOLE_SIZE, 0, &mapped);
        memcpy(data.data(), mapped, sizeof(float) * NUM_ELEMENTS);
        vkUnmapMemory(device, bufferMemory);

        std::cout << "Data after compute shader execution:" << std::endl;
        for (float f : data)
        {
            std::cout << f << " ";
        }
        std::cout << std::endl;

        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        vkDestroyPipeline(device, pipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyShaderModule(device, shaderModule, nullptr);
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
        vkDestroyBuffer(device, buffer, nullptr);
        vkFreeMemory(device, bufferMemory, nullptr);
    }

    void init()
    {
        // Initialize GLFW and create a hidden window so that the surface
        // extension is enabled. This avoids validation errors when the
        // base helpers query for presentation support.
        initWindow();
        glfwHideWindow(window);
        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createComputeCommandPool();
    }
};

int main()
{
    ComputeExample app;
    app.init();
    app.runExample();
    return 0;
}
