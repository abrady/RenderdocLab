#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <optional>
#include <array>
#include <filesystem>

// Global validation and extension lists shared with helpers
extern const std::vector<const char *> validationLayers;
extern const std::vector<const char *> deviceExtensions;
extern const bool enableValidationLayers;

#define VULKANAPP_GETSHADERDIR \
  ((std::filesystem::path{__FILE__}.parent_path() / "shaders").generic_string() + '/')

#define MAX_FRAMES_IN_FLIGHT 2

class VulkanApp
{
public:
  VulkanApp(int width, int height, const std::string &appName, const std::string &shaderDir)
      : windowWidth(width), windowHeight(height), appName(appName), shaderDir(shaderDir)
  {
  }

  void init()
  {
    initWindow();
    initVulkan();
  }
  virtual ~VulkanApp();

  void run();

  // Called on key events. Default implementation does nothing
  virtual void onKey(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {}

  void setTargetFPS(float fps) { targetFPS = fps; }
  float getTargetFPS() const { return targetFPS; }

private:
  const std::string shaderDir;

protected:
  // Window properties
  int windowWidth;
  int windowHeight;
  std::string appName;
  GLFWwindow *window = nullptr;
  // Desired frame rate. Defaults to 30 FPS. When 0, rendering runs without delay.
  float targetFPS = 30.0f;

  // Vulkan objects
  VkInstance instance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device = VK_NULL_HANDLE;
  VkQueue graphicsQueue = VK_NULL_HANDLE;
  VkQueue presentQueue = VK_NULL_HANDLE;
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  VkSwapchainKHR swapChain = VK_NULL_HANDLE;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  std::vector<VkImageView> swapChainImageViews;
  VkRenderPass renderPass = VK_NULL_HANDLE;
  VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
  VkPipeline graphicsPipeline = VK_NULL_HANDLE;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkCommandPool commandPool = VK_NULL_HANDLE;
  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  size_t currentFrame = 0;
  bool framebufferResized = false;

  // Initialization functions
  void initWindow();
  virtual void initVulkan();
  void mainLoop();
  virtual void cleanup();

  // Vulkan setup functions
  void createInstance();
  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSwapChain();
  void createImageViews();
  void createRenderPass();
  virtual void createGraphicsPipeline();
  void createFramebuffers();
  void createCommandPool();
  virtual void createCommandBuffers();
  void createSyncObjects();

  // Called each frame between pipeline bind and render pass end
  virtual void recordRenderCommands(VkCommandBuffer commandBuffer);

  // Helper functions
  bool checkValidationLayerSupport();
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  std::vector<const char *> getRequiredExtensions();
  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  bool isDeviceSuitable(VkPhysicalDevice device);
  VkShaderModule createShaderModule(const std::vector<char> &code);

  // Shader helper functions
  std::string getShaderDir() const
  {
    return shaderDir;
  }
  std::vector<char> readFile(const std::string &filename);
  std::vector<char> compileShader(const std::string &filename, VkShaderStageFlagBits shaderStage);

  // Struct for queue family indices
  struct QueueFamilyIndices
  {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> computeFamily;

    bool isComplete()
    {
      return graphicsFamily.has_value() && presentFamily.has_value();
    }
  };

  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
  {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
      if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
      {
        return i;
      }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
  }

  // Struct for swap chain support details
  struct SwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  // Single-use command helpers
  VkCommandBuffer beginSingleTimeCommands();
  void endSingleTimeCommands(VkCommandBuffer commandBuffer);

  // General image layout transition helper
  void transitionImageLayout(VkImage image, VkFormat format,
                             VkImageLayout oldLayout,
                             VkImageLayout newLayout);

  // Drawing functions
  void drawFrame();
  void recreateSwapChain();
  void cleanupSwapChain();

  // Debug callback
  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
      void *pUserData);

  // Static helper for window resize
  static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
  // Static helper for key input
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
};
