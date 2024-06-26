//
// Created by ShaneMonck on 26/03/2024.
//

#include "GameObject.h"

// #region TransformComponent struct

glm::mat4 TransformComponent::mat4() {
    const float c3 = glm::cos(Rotation.z);
    const float s3 = glm::sin(Rotation.z);
    const float c2 = glm::cos(Rotation.x);
    const float s2 = glm::sin(Rotation.x);
    const float c1 = glm::cos(Rotation.y);
    const float s1 = glm::sin(Rotation.y);
    return glm::mat4{
            {Scale.x * (c1 * c3 + s1 * s2 * s3), Scale.x * (c2 * s3), Scale.x * (c1 * s2 * s3 - c3 * s1), 0.0f},
            {Scale.y * (c3 * s1 * s2 - c1 * s3), Scale.y * (c2 * c3), Scale.y * (c1 * c3 * s2 + s1 * s3), 0.0f},
            {Scale.z * (c2 * s1),                Scale.z * (-s2),     Scale.z * (c1 * c2),                0.0f},
            {Translation.x,                      Translation.y,       Translation.z,                      1.0f}
    };
}

glm::mat3 TransformComponent::normalMatrix() {
    const float c3 = glm::cos(Rotation.z);
    const float s3 = glm::sin(Rotation.z);
    const float c2 = glm::cos(Rotation.x);
    const float s2 = glm::sin(Rotation.x);
    const float c1 = glm::cos(Rotation.y);
    const float s1 = glm::sin(Rotation.y);

    const glm::vec3 inverseScale = 1.0f / Scale;

    return glm::mat3{
            {
                    inverseScale.x * (c1 * c3 + s1 * s2 * s3),
                    inverseScale.x * (c2 * s3),
                    inverseScale.x * (c1 * s2 * s3 - c3 * s1)
            },
            {
                    inverseScale.y * (c3 * s1 * s2 - c1 * s3),
                    inverseScale.y * (c2 * c3),
                    inverseScale.y * (c1 * c3 * s2 + s1 * s3)
            },
            {
                    inverseScale.z * (c2 * s1),
                    inverseScale.z * (-s2),
                    inverseScale.z * (c1 * c2)
            }
    };
}

// #endregion


// #region Public Methods

GameObject GameObject::createPointLight(float intensity, float radius, glm::vec3 color) {
    GameObject gameObject = GameObject::createGameObject();
    gameObject.color = color;
    gameObject.transformComponent.Scale.x = radius;
    gameObject.pointLight = std::make_unique<PointLightComponent>();
    gameObject.pointLight->lightIntensity = intensity;
    return gameObject;
}

// #endregion