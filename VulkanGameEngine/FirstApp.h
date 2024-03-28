//
// Created by ShaneMonck on 20/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_FIRSTAPP_H
#define VULKANGAMEENGINETUTORIAL_FIRSTAPP_H


#include <memory>
#include "Window.h"
#include "GameObject.h"
#include "Renderer.h"

class FirstApp {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();

    ~FirstApp();

    FirstApp(const FirstApp &) = delete;

    FirstApp &operator=(const FirstApp &) = delete;

    void Run();

private:
    Window window = Window(WIDTH, HEIGHT, "Hello Vulkan!");
    EngineDevice engineDevice = EngineDevice(window);
    Renderer renderer = Renderer(window, engineDevice);

    std::vector<GameObject> gameObjects;
    
    void loadGameObjects();

    std::unique_ptr<Model> CreateSquareModel(EngineDevice& device, glm::vec2 offset);

    std::unique_ptr<Model> CreateCircleModel(EngineDevice& device, unsigned int numSides);
};


#endif //VULKANGAMEENGINETUTORIAL_FIRSTAPP_H
