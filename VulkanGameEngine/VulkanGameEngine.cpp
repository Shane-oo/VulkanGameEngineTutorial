// VulkanGameEngine.cpp : Defines the entry point for the application.
//

#include "VulkanGameEngine.h"
#include "FirstApp.h"

using namespace std;

int main() {
    FirstApp app = FirstApp();

    try {
        app.Run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
