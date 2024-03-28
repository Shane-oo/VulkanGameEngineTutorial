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
    std::shared_ptr<Model> model = createCubeModel(engineDevice, glm::vec3(.0f, .0f, .0f));

    auto cube = GameObject::createGameObject();
    cube.model = model;
    cube.transformComponent.Translation = glm::vec3(.0f, .0f, .5f);
    cube.transformComponent.Scale = glm::vec3(.5f, .5f, .5f);
    
    gameObjects.push_back(std::move(cube));
}

// temporary helper function, creates a 1x1x1 cube centered at offset
std::unique_ptr<Model> FirstApp::createCubeModel(EngineDevice &device, glm::vec3 offset) {
    std::vector<Model::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f},  {.9f, .9f, .9f}},
            {{-.5f, .5f,  .5f},   {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f},   {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f},  {.9f, .9f, .9f}},
            {{-.5f, .5f,  -.5f},  {.9f, .9f, .9f}},
            {{-.5f, .5f,  .5f},   {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f,  -.5f, -.5f},  {.8f, .8f, .1f}},
            {{.5f,  .5f,  .5f},   {.8f, .8f, .1f}},
            {{.5f,  -.5f, .5f},   {.8f, .8f, .1f}},
            {{.5f,  -.5f, -.5f},  {.8f, .8f, .1f}},
            {{.5f,  .5f,  -.5f},  {.8f, .8f, .1f}},
            {{.5f,  .5f,  .5f},   {.8f, .8f, .1f}},

            // top face (orange, remember y-axis points down)
            {{-.5f, -.5f, -.5f},  {.9f, .6f, .1f}},
            {{.5f,  -.5f, .5f},   {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f},   {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f},  {.9f, .6f, .1f}},
            {{.5f,  -.5f, -.5f},  {.9f, .6f, .1f}},
            {{.5f,  -.5f, .5f},   {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f,  -.5f},  {.8f, .1f, .1f}},
            {{.5f,  .5f,  .5f},   {.8f, .1f, .1f}},
            {{-.5f, .5f,  .5f},   {.8f, .1f, .1f}},
            {{-.5f, .5f,  -.5f},  {.8f, .1f, .1f}},
            {{.5f,  .5f,  -.5f},  {.8f, .1f, .1f}},
            {{.5f,  .5f,  .5f},   {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f},  {.1f, .1f, .8f}},
            {{.5f,  .5f,  0.5f},  {.1f, .1f, .8f}},
            {{-.5f, .5f,  0.5f},  {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f},  {.1f, .1f, .8f}},
            {{.5f,  -.5f, 0.5f},  {.1f, .1f, .8f}},
            {{.5f,  .5f,  0.5f},  {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f,  .5f,  -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f,  -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f,  -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f,  .5f,  -0.5f}, {.1f, .8f, .1f}},

    };
    for (auto &v: vertices) {
        v.position += offset;
    }
    return std::make_unique<Model>(device, vertices);
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

