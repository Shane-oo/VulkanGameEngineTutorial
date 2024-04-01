//
// Created by ShaneMonck on 20/03/2024.
//
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "FirstApp.h"
#include "KeyboardMovementController.h"
#include "SimpleRenderSystem.h"
#include <array>
#include <chrono>
#include <stdexcept>

// #region Private Methods

void FirstApp::loadGameObjects() {
  std::shared_ptr<Model> model =
      createCubeModel(engineDevice, glm::vec3(.0f, .0f, .0f));

  auto cube = GameObject::createGameObject();
  cube.model = model;
  cube.transformComponent.Translation = glm::vec3(.0f, .0f, 2.5f);
  cube.transformComponent.Scale = glm::vec3(.5f, .5f, .5f);

  gameObjects.push_back(std::move(cube));
}

// temporary helper function, creates a 1x1x1 cube centered at offset
std::unique_ptr<Model> FirstApp::createCubeModel(EngineDevice &device,
                                                 glm::vec3 offset) {
  std::vector<Model::Vertex> vertices{

      // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

      // top face (orange, remember y-axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

  };
  for (auto &v : vertices) {
    v.position += offset;
  }
  return std::make_unique<Model>(device, vertices);
}

// #endregion

// #region Constructors

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

// #endregion

// #region Public Methods
void FirstApp::Run() {
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

    cameraController.MoveInPlaneXZ(window.GetGLFWWindow(), frameTime, viewerObject);
    camera.SetViewYXZ(viewerObject.transformComponent.Translation, viewerObject.transformComponent.Rotation);

    float aspect = renderer.GetAspectRatio();
    camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

    if (auto commandBuffer = renderer.BeginDrawFrame()) {
      renderer.BeginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.RenderGameObjects(commandBuffer, gameObjects, camera);
      renderer.EndSwapChainRenderPass(commandBuffer);
      renderer.EndDrawFrame();
    }
  }

  vkDeviceWaitIdle(engineDevice.device());
}

// #endregion
