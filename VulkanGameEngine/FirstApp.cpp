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
#include "Systems/GravityPhysicsSystem.h"
#include "Systems/Vec2FieldSystem.h"

// #region Private Methods

void FirstApp::loadGameObjects() {
    std::vector<Model::Vertex> vertices = {
            {{0.0f,  -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f,  0.5f},  {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}}
    };

    auto model = std::make_shared<Model>(engineDevice, vertices);

    auto triangle = GameObject::CreateGameObject();
    triangle.model = model;
    triangle.color = glm::vec3(.1f, .8f, .1f);
    triangle.transform2DComponent.Translation.x = .2f;
    triangle.transform2DComponent.Scale = glm::vec2(2.f, .5f);
    triangle.transform2DComponent.Rotation = .25f * glm::two_pi<float>();

    gameObjects.push_back(std::move(triangle));
}

std::unique_ptr<Model> FirstApp::CreateSquareModel(EngineDevice &device, glm::vec2 offset) {
    std::vector<Model::Vertex> vertices = {
            {{-0.5f, -0.5f}},
            {{0.5f,  0.5f}},
            {{-0.5f, 0.5f}},
            {{-0.5f, -0.5f}},
            {{0.5f,  -0.5f}},
            {{0.5f,  0.5f}}
    };

    for (auto &vertex: vertices) {
        vertex.position += offset;
    }

    return std::make_unique<Model>(device, vertices);
}

std::unique_ptr<Model> FirstApp::CreateCircleModel(EngineDevice &device, unsigned int numSides) {
    std::vector<Model::Vertex> uniqueVertices = std::vector<Model::Vertex>();
    for (int i = 0; i < numSides; i++) {
        float angle = i * glm::two_pi<float>() / numSides;
        uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle)}});
    }

    uniqueVertices.push_back({});  // adds center vertex at 0, 0

    std::vector<Model::Vertex> vertices = std::vector<Model::Vertex>();
    for (int i = 0; i < numSides; i++) {
        vertices.push_back(uniqueVertices[i]);
        vertices.push_back(uniqueVertices[(i + 1) % numSides]);
        vertices.push_back(uniqueVertices[numSides]);
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
    // todo
    std::shared_ptr<Model> squareModel = CreateSquareModel(engineDevice, glm::vec2(.0f, .0f));
    std::shared_ptr<Model> circleModel = CreateCircleModel(engineDevice, 64);

    // create physics objects
    std::vector<GameObject> physicsObjects = std::vector<GameObject>();
    auto red = GameObject::CreateGameObject();
    red.transform2DComponent.Scale = glm::vec2{.05f};
    red.transform2DComponent.Translation = glm::vec2(.5f, .5f);
    red.color = glm::vec3(1.f, 0.f, 0.f);
    red.rigidBody2DComponent.velocity = glm::vec2(-.5f, .0f);
    red.model = circleModel;
    physicsObjects.push_back(std::move(red));

    auto blue = GameObject::CreateGameObject();
    blue.transform2DComponent.Scale = glm::vec2{.05f};
    blue.transform2DComponent.Translation = glm::vec2(-.45f, -.25f);
    blue.color = glm::vec3(0.f, 0.f, 1.f);
    blue.rigidBody2DComponent.velocity = glm::vec2(-.5f, .0f);
    blue.model = circleModel;
    physicsObjects.push_back(std::move(blue));


    // create vector field
    std::vector<GameObject> vectorField = std::vector<GameObject>();
    int gridCount = 40;
    for (int i = 0; i < gridCount; i++) {
        for (int j = 0; j < gridCount; j++) {
            auto vf = GameObject::CreateGameObject();
            vf.transform2DComponent.Scale = glm::vec2(0.005f);
            vf.transform2DComponent.Translation = glm::vec2(
                    -1.0f + (i + 0.5f) * 2.0f / gridCount,
                    -1.0f + (j + 0.5f) * 2.0f / gridCount
            );
            vf.color = glm::vec3(1.0f);
            vf.model = squareModel;
            vectorField.push_back(std::move(vf));
        }
    }

    GravityPhysicsSystem gravityPhysicsSystem = GravityPhysicsSystem(0.81f);
    Vec2FieldSystem vec2FieldSystem = Vec2FieldSystem();


    SimpleRenderSystem simpleRenderSystem = SimpleRenderSystem(engineDevice, renderer.GetSwapChainRederPass());

    while (!window.ShouldClose()) {
        // listen for keystrokes or close button
        glfwPollEvents();

        if (auto commandBuffer = renderer.BeginDrawFrame()) {
            gravityPhysicsSystem.Update(physicsObjects, 1.f / 60, 5);
            vec2FieldSystem.Update(gravityPhysicsSystem, physicsObjects, vectorField);

            renderer.BeginSwapChainRenderPass(commandBuffer);
            //simpleRenderSystem.RenderGameObjects(commandBuffer, gameObjects);
            simpleRenderSystem.RenderGameObjects(commandBuffer, physicsObjects);
            simpleRenderSystem.RenderGameObjects(commandBuffer, vectorField);
            renderer.EndSwapChainRenderPass(commandBuffer);
            renderer.EndDrawFrame();
        }

    }

    vkDeviceWaitIdle(engineDevice.device());
}




// #endregion

