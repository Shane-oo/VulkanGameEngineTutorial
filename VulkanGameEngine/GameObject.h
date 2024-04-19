//
// Created by ShaneMonck on 26/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_GAMEOBJECT_H
#define VULKANGAMEENGINETUTORIAL_GAMEOBJECT_H

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>
#include "Model.h"


struct TransformComponent {
    glm::vec3 Translation = glm::vec3();
    glm::vec3 Scale = glm::vec3(1.f, 1.f, 1.f);
    glm::vec3 Rotation = glm::vec3();

    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    glm::mat4 mat4();
    glm::mat3 normalMatrix();
};

class GameObject {
public:
    using id_t = unsigned int;
    using Map = std::unordered_map<id_t, GameObject>;

    static GameObject createGameObject() {
        static id_t currentId = 0;
        return GameObject(currentId++);
    }

    GameObject(const GameObject &) = delete;

    GameObject &operator=(const GameObject &) = delete;

    GameObject(GameObject &&) = default;

    GameObject &operator=(GameObject &&) = default;

    id_t getId() const { return id; }

    std::shared_ptr<Model> model = std::shared_ptr<Model>();
    glm::vec3 color = glm::vec3();
    TransformComponent transformComponent = TransformComponent();

private:
    explicit GameObject(id_t id) {
        this->id = id;
    };
    id_t id;
};


#endif //VULKANGAMEENGINETUTORIAL_GAMEOBJECT_H
