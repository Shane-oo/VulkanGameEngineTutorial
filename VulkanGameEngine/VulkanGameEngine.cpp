// VulkanGameEngine.cpp : Defines the entry point for the application.
//

#include "VulkanGameEngine.h"
#include "FirstApp.h"

using namespace std;

int main() {
    try {
        FirstApp app = FirstApp();
        app.Run();
    }
    catch (const std::exception &e) {
        printf("%s\n", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
