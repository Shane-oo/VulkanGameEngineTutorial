# CMakeList.txt : CMake project for VulkanGameEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (VulkanGameEngine "VulkanGameEngine.cpp" "VulkanGameEngine.h"
        Window.cpp
        Window.h
        FirstApp.cpp
        FirstApp.h)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VulkanGameEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

#Find Vulkan
find_package(Vulkan REQUIRED)
# Link Vulkan
target_link_libraries(VulkanGameEngine PRIVATE Vulkan::Vulkan)
# Find GLFW package
find_package(glfw3 REQUIRED)
# Link GLFW Library
target_link_libraries(VulkanGameEngine PRIVATE glfw)

# Find GLM package
find_package(glm REQUIRED)
# Link GLM Library
target_link_directories(VulkanGameEngine PRIVATE glm)
