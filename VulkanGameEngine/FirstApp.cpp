//
// Created by ShaneMonck on 20/03/2024.
//
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>


#include "FirstApp.h"
#include "KeyboardMovementController.h"
#include "Systems/SimpleRenderSystem.h"
#include "Descriptors/DescriptorSetLayout.h"
#include "Descriptors/DescriptorWriter.h"
#include "Systems/PointLightSystem.h"
#include <array>
#include <chrono>


// #region structs

// #endregion

// #region Private Methods

void FirstApp::loadGameObjects() {
    std::shared_ptr<Model> model = Model::CreateModelFromFile(engineDevice, "models/flat_vase.obj");

    auto flatVase = GameObject::createGameObject();
    flatVase.model = model;
    flatVase.transformComponent.Translation = glm::vec3(-.5f, .5f, 0.f);
    flatVase.transformComponent.Scale = glm::vec3(3.f, 1.5f, 3.f);
    gameObjects.emplace(flatVase.getId(), std::move(flatVase));

    model = Model::CreateModelFromFile(engineDevice, "models/smooth_vase.obj");

    auto smoothVase = GameObject::createGameObject();
    smoothVase.model = model;
    smoothVase.transformComponent.Translation = glm::vec3(.5f, .5f, 0.f);
    smoothVase.transformComponent.Scale = glm::vec3(3.f, 1.5f, 3.f);

    gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

    model = Model::CreateModelFromFile(engineDevice, "models/quad.obj");

    auto floor = GameObject::createGameObject();
    floor.model = model;
    floor.transformComponent.Translation = glm::vec3(.0f, .5f, 0.f);
    floor.transformComponent.Scale = glm::vec3(3.f, 1.f, 3.f);

    gameObjects.emplace(floor.getId(), std::move(floor));


    std::vector<glm::vec3> lightColors{
            {1.f,  .1f,  .1f},
            {.1f,  .1f,  1.f},
            {.1f,  1.f,  .1f},
            {1.f,  1.f,  .1f},
            {.1f,  1.f,  1.f},
            {1.f,  1.f,  1.f},
            {0.5f, 0.5f, 0.5f}
    };

    for (int i = 0; i < lightColors.size(); i++) {
        {
            auto pointLight = GameObject::createPointLight(0.2f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(glm::mat4(1.f), (i * glm::two_pi<float>()) / lightColors.size(),
                                           {0.f, -1.f, 0.f});
            pointLight.transformComponent.Translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
            gameObjects.emplace(pointLight.getId(), std::move(pointLight));
        }
    }
}

// #endregion

// #region Constructors

FirstApp::FirstApp() {
    globalPool = DescriptorPool::Builder(engineDevice)
            .setMaxSets(EngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, EngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();


    loadGameObjects();
}

FirstApp::~FirstApp() {}

// #endregion

// #region Public Methods
void FirstApp::Run() {

    std::vector<std::unique_ptr<Buffer>> uboBuffers = std::vector<std::unique_ptr<Buffer>>(
            EngineSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (auto &uboBuffer: uboBuffers) {
        uboBuffer = std::make_unique<Buffer>(
                engineDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        );
        uboBuffer->map();
    }

    auto globalSetLayout = DescriptorSetLayout::Builder(engineDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                        VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

    std::vector<VkDescriptorSet> globalDescriptorSets = std::vector<VkDescriptorSet>(
            EngineSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++) {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        DescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);

    }

    SimpleRenderSystem simpleRenderSystem = SimpleRenderSystem(
            engineDevice,
            renderer.GetSwapChainRederPass(),
            globalSetLayout->getDescriptorSetLayout());
    PointLightSystem pointLightRenderSystem = PointLightSystem(
            engineDevice,
            renderer.GetSwapChainRederPass(),
            globalSetLayout->getDescriptorSetLayout());

    Camera camera = Camera();
    glm::vec3 position = glm::vec3(-1.f, -2.f, -7.f);
    glm::vec3 direction = glm::vec3(0.5f, 0.f, 1.f);
    glm::vec3 target = glm::vec3(0.f, 0.f, 2.5f);
    camera.SetViewTarget(position, target);

    auto viewerObject = GameObject::createGameObject();
    viewerObject.transformComponent.Translation.z = -2.5f;
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
        camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

        if (auto commandBuffer = renderer.BeginDrawFrame()) {
            int frameIndex = renderer.GetFrameIndex();
            auto frameInfo = FrameInfo(frameIndex,
                                       frameTime,
                                       commandBuffer,
                                       camera,
                                       globalDescriptorSets[frameIndex],
                                       gameObjects);

            // update
            GlobalUbo ubo = GlobalUbo();
            ubo.projection = camera.GetProjectionMatrix();
            ubo.view = camera.GetViewMatrix();
            ubo.inverseView = camera.GetInverseViewMatrix();
            pointLightRenderSystem.Update(frameInfo, ubo);

            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            // render
            renderer.BeginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.RenderGameObjects(frameInfo);
            pointLightRenderSystem.Render(frameInfo);
            renderer.EndSwapChainRenderPass(commandBuffer);
            renderer.EndDrawFrame();
        }
    }

    vkDeviceWaitIdle(engineDevice.device());
}

// #endregion
