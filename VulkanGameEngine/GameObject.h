//
// Created by ShaneMonck on 26/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_GAMEOBJECT_H
#define VULKANGAMEENGINETUTORIAL_GAMEOBJECT_H

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include "Model.h"


struct TransformComponent {
    glm::vec3 Translation = glm::vec3();
    glm::vec3 Scale = glm::vec3(1.f, 1.f, 1.f);
    glm::vec3 Rotation = glm::vec3();

    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    glm::mat4 mat4() {
        const float c3 = glm::cos(Rotation.z);
        const float s3 = glm::sin(Rotation.z);
        const float c2 = glm::cos(Rotation.x);
        const float s2 = glm::sin(Rotation.x);
        const float c1 = glm::cos(Rotation.y);
        const float s1 = glm::sin(Rotation.y);
        return glm::mat4{
                {Scale.x * (c1 * c3 + s1 * s2 * s3), Scale.x * (c2 * s3), Scale.x * (c1 * s2 * s3 - c3 * s1), 0.0f},
                {Scale.y * (c3 * s1 * s2 - c1 * s3), Scale.y * (c2 * c3), Scale.y * (c1 * c3 * s2 + s1 * s3), 0.0f},
                {Scale.z * (c2 * s1),Scale.z * (-s2),Scale.z * (c1 * c2),0.0f,},
                {Translation.x, Translation.y, Translation.z,1.0f}
        };
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
