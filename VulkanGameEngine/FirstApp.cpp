//
// Created by ShaneMonck on 20/03/2024.
//
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>


#include "FirstApp.h"
#include "KeyboardMovementController.h"
#include "SimpleRenderSystem.h"
#include <array>
#include <chrono>


// #region structs

struct GlobalUbo {
    glm::mat4 projectionView = glm::mat4(1.f);
    glm::vec3 lightDirection = glm::normalize(glm::vec3(1.f, -3.f, -1.f));
};

// #endregion

// #region Private Methods

void FirstApp::loadGameObjects() {
    std::shared_ptr<Model> model = Model::CreateModelFromFile(engineDevice, "models/flat_vase.obj");

    auto flatVase = GameObject::createGameObject();
    flatVase.model = model;
    flatVase.transformComponent.Translation = glm::vec3(-.5f, .5f, 2.5f);
    flatVase.transformComponent.Scale = glm::vec3(3.f, 1.5f, 3.f);
    gameObjects.push_back(std::move(flatVase));

    model = Model::CreateModelFromFile(engineDevice, "models/smooth_vase.obj");

    auto smoothVase = GameObject::createGameObject();
    smoothVase.model = model;
    smoothVase.transformComponent.Translation = glm::vec3(.5f, .5f, 2.5f);
    smoothVase.transformComponent.Scale = glm::vec3(3.f, 1.5f, 3.f);

    gameObjects.push_back(std::move(smoothVase));
}

// #endregion

// #region Constructors

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

// #endregion

// #region Public Methods
void FirstApp::Run() {

    Buffer globalUboBuffer = Buffer(
            engineDevice,
            sizeof(GlobalUbo),
            EngineSwapChain::MAX_FRAMES_IN_FLIGHT,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            engineDevice.properties.limits.minUniformBufferOffsetAlignment
    );

    globalUboBuffer.map();


    SimpleRenderSystem simpleRenderSystem =
            SimpleRenderSystem(engineDevice, renderer.GetSwapChainRederPass());
    Camera camera = Camera();
    glm::vec3 position = glm::vec3(-1.f, -2.f, -7.f);
    glm::vec3 direction = glm::vec3(0.5f, 0.f, 1.f);
    glm::vec3 target = glm::vec3(0.f, 0.f, 2.5f);
    camera.SetViewTarget(position, target);

    auto viewerObject = GameObject::createGameObject();
    KeyboardMovementController cameraController = KeyboardMovementController();

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!window.ShouldClose()) {
        // listen for keystrokes or close button
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime -
                                                                           currentTime)
                        .count();
        currentTime = newTime;

        cameraController.MoveInPlaneXZ(window.GetGLFWWindow(), frameTime,
                                       viewerObject);
        camera.SetViewYXZ(viewerObject.transformComponent.Translation,
                          viewerObject.transformComponent.Rotation);

        float aspect = renderer.GetAspectRatio();
        camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

        if (auto commandBuffer = renderer.BeginDrawFrame()) {
            int frameIndex = renderer.GetFrameIndex();
            auto frameInfo = FrameInfo(frameIndex, frameTime, commandBuffer, camera);

            // update
            GlobalUbo ubo = GlobalUbo();
            ubo.projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
            globalUboBuffer.writeToIndex(&ubo, frameIndex);
            globalUboBuffer.flushIndex(frameIndex);

            // render
            renderer.BeginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.RenderGameObjects(frameInfo, gameObjects);
            renderer.EndSwapChainRenderPass(commandBuffer);
            renderer.EndDrawFrame();
        }
    }

    vkDeviceWaitIdle(engineDevice.device());
}

// #endregion
