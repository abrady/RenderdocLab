#include "vulkan_app.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

int main() {
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const std::string APP_NAME = "Vulkan Hello Triangle";

    VulkanApp app(WIDTH, HEIGHT, APP_NAME);

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
