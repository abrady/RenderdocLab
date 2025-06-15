#include "vulkan_compute_app.h"
#include <iostream>
#include <vector>
#include <cstring>

#ifdef ENABLE_RENDERDOC_CAPTURE
#include <renderdoc_app.h>
#ifdef _WIN32
#include <windows.h>
#endif
#endif

class ComputeExample : public VulkanComputeApp
{
public:
    ComputeExample() : VulkanComputeApp(1, 1, "Compute Example", VULKANAPP_GETSHADERDIR) {}

    void runExample()
    {

#ifdef ENABLE_RENDERDOC_CAPTURE
        static RENDERDOC_API_1_6_0 *rdoc_api = nullptr;
        if (rdoc_api == nullptr)
        {
            // this will be loaded if this is run from renderdoc UI
            HMODULE mod = GetModuleHandleA("renderdoc.dll");
            if (mod)
            {
                pRENDERDOC_GetAPI RENDERDOC_GetAPI =
                    (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
                int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&rdoc_api);
                assert(ret == 1);
            }
        }
        if (rdoc_api)
        {
            rdoc_api->StartFrameCapture(nullptr, nullptr);
        }
#endif
        const uint32_t NUM_ELEMENTS = 16;
        std::vector<float> inData(NUM_ELEMENTS);
        std::vector<float> outData(NUM_ELEMENTS, 0.f);
        std::cout << "Running compute shader example with " << NUM_ELEMENTS << " elements." << std::endl;
        for (uint32_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            inData[i] = static_cast<float>(i);
            std::cout << inData[i] << " ";
        }
        std::cout << std::endl;

        VkBuffer inBuffer;
        VkDeviceMemory inBufferMemory;
        createBuffer(sizeof(float) * NUM_ELEMENTS,
                     VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     inBuffer, inBufferMemory);

        VkBuffer outBuffer;
        VkDeviceMemory outBufferMemory;
        createBuffer(sizeof(float) * NUM_ELEMENTS,
                     VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     outBuffer, outBufferMemory);

        void *mapped;
        vkMapMemory(device, inBufferMemory, 0, VK_WHOLE_SIZE, 0, &mapped);
        memcpy(mapped, inData.data(), sizeof(float) * NUM_ELEMENTS);
        vkUnmapMemory(device, inBufferMemory);

        VkDescriptorSetLayoutBinding bindings[2]{};
        bindings[0].binding = 0;
        bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[0].descriptorCount = 1;
        bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

        bindings[1].binding = 1;
        bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[1].descriptorCount = 1;
        bindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 2;
        layoutInfo.pBindings = bindings;
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
        poolSize.descriptorCount = 2;
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

        VkDescriptorBufferInfo inBufferInfo{};
        inBufferInfo.buffer = inBuffer;
        inBufferInfo.offset = 0;
        inBufferInfo.range = sizeof(float) * NUM_ELEMENTS;

        VkDescriptorBufferInfo outBufferInfo{};
        outBufferInfo.buffer = outBuffer;
        outBufferInfo.offset = 0;
        outBufferInfo.range = sizeof(float) * NUM_ELEMENTS;

        VkWriteDescriptorSet writes[2]{};
        writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[0].dstSet = descriptorSet;
        writes[0].dstBinding = 0;
        writes[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        writes[0].descriptorCount = 1;
        writes[0].pBufferInfo = &inBufferInfo;

        writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[1].dstSet = descriptorSet;
        writes[1].dstBinding = 1;
        writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        writes[1].descriptorCount = 1;
        writes[1].pBufferInfo = &outBufferInfo;

        vkUpdateDescriptorSets(device, 2, writes, 0, nullptr);

        VkCommandBuffer commandBuffer = beginSingleTimeCommands();
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
        vkCmdDispatch(commandBuffer, NUM_ELEMENTS, 1, 1);
        endSingleTimeCommands(commandBuffer);

        vkMapMemory(device, outBufferMemory, 0, VK_WHOLE_SIZE, 0, &mapped);
        memcpy(outData.data(), mapped, sizeof(float) * NUM_ELEMENTS);
        vkUnmapMemory(device, outBufferMemory);

        std::cout << "Data after compute shader execution:" << std::endl;
        for (float f : outData)
        {
            std::cout << f << " ";
        }
        std::cout << std::endl;

        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        vkDestroyPipeline(device, pipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyShaderModule(device, shaderModule, nullptr);
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
        vkDestroyBuffer(device, inBuffer, nullptr);
        vkFreeMemory(device, inBufferMemory, nullptr);
        vkDestroyBuffer(device, outBuffer, nullptr);
        vkFreeMemory(device, outBufferMemory, nullptr);
#ifdef ENABLE_RENDERDOC_CAPTURE
        if (rdoc_api)
        {
            rdoc_api->EndFrameCapture(nullptr, nullptr);

            char filename[512] = {};
            uint32_t len = 512;
            rdoc_api->GetCapture(0, filename, &len, nullptr);
            printf("RenderDoc wrote %s\n", filename);
        }
#endif
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
