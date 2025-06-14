#include "vulkan_app.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class HelloTriangleApp : public VulkanApp {
  public:
    HelloTriangleApp(int width, int height, const char* title) : VulkanApp(width, height, title, VULKANAPP_GETSHADERDIR) {
    }
};

int main() {
    const int WIDTH = 800;
    const int HEIGHT = 600;

    HelloTriangleApp app(WIDTH, HEIGHT, "Vulkan Hello Triangle");

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
