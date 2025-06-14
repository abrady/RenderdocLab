#pragma once
#include "vulkan_app.h"

class VulkanComputeApp : public VulkanApp {
public:
    VulkanComputeApp(int width, int height, const std::string& appName, const std::string& shaderDir)
        : VulkanApp(width, height, appName, shaderDir) {}
    virtual ~VulkanComputeApp();

protected:
    VkQueue computeQueue = VK_NULL_HANDLE;
    VkCommandPool computeCommandPool = VK_NULL_HANDLE;

    virtual void initVulkan() override;
    virtual void createLogicalDevice();
    void createComputeCommandPool();

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                      VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkQueue queue, VkCommandPool pool);
};
