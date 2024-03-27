//
// Created by ShaneMonck on 20/03/2024.
//
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include "FirstApp.h"
#include "SimpleRenderSystem.h"

// #region Private Methods

void FirstApp::loadGameObjects() {
    std::vector<Model::Vertex> vertices = {
            {{0.0f,  -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f,  0.5f},  {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}}
    };

    auto model = std::make_shared<Model>(engineDevice, vertices);

    auto triangle = GameObject::createGameObject();
    triangle.model = model;
    triangle.color = glm::vec3(.1f, .8f, .1f);
    triangle.transform2DComponent.Translation.x = .2f;
    triangle.transform2DComponent.Scale = glm::vec2(2.f, .5f);
    triangle.transform2DComponent.Rotation = .25f * glm::two_pi<float>();

    gameObjects.push_back(std::move(triangle));
}



// #endregion

// #region Constructors

FirstApp::FirstApp() {
    loadGameObjects();
}

FirstApp::~FirstApp() {
}

// #endregion

// #region Public Methods
void FirstApp::Run() {
    SimpleRenderSystem simpleRenderSystem = SimpleRenderSystem(engineDevice, renderer.GetSwapChainRederPass());

    while (!window.ShouldClose()) {
        // listen for keystrokes or close button
        glfwPollEvents();

        if (auto commandBuffer = renderer.BeginDrawFrame()) {
            renderer.BeginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.RenderGameObjects(commandBuffer, gameObjects);
            renderer.EndSwapChainRenderPass(commandBuffer);
            renderer.EndDrawFrame();
        }

    }

    vkDeviceWaitIdle(engineDevice.device());
}




// #endregion

