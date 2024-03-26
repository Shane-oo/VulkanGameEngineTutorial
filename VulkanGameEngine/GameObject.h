//
// Created by ShaneMonck on 26/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_GAMEOBJECT_H
#define VULKANGAMEENGINETUTORIAL_GAMEOBJECT_H


#include <memory>
#include "Model.h"

struct Transform2DComponent {
    glm::vec2 Translation = glm::vec2();
    glm::vec2 Scale = glm::vec2(1.f, 1.f);
    float Rotation;

    glm::mat2 mat2() const {
        const float s = glm::sin(Rotation);
        const float c = glm::cos(Rotation);
        glm::mat2 rotationMatrix = glm::mat2(glm::vec2(c, s), glm::vec2(-s, c));

        glm::mat2 scaleMat = glm::mat2(glm::vec2(Scale.x, .0f), glm::vec2(.0f, Scale.y));
        return rotationMatrix * scaleMat;
    }
};

class GameObject {
public:
    using id_t = unsigned int;

    static GameObject createGameObject() {
        static id_t currentId = 0;
        return GameObject(currentId++);
    }

    GameObject(const GameObject &) = delete;

    GameObject &operator=(const GameObject &) = delete;

    GameObject(GameObject &&) = default;

    GameObject &operator=(GameObject &&) = default;

    id_t getId() const { return id; }

    std::shared_ptr<Model> model = std::shared_ptr<Model>();              // hmm he removed the initalisers here
    glm::vec3 color = glm::vec3();
    Transform2DComponent transform2DComponent = Transform2DComponent();

private:
    explicit GameObject(id_t id) {
        this->id = id;
    };
    id_t id;
};


#endif //VULKANGAMEENGINETUTORIAL_GAMEOBJECT_H
