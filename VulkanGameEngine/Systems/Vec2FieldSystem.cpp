//
// Created by ShaneMonck on 27/03/2024.
//

#include <glm/gtc/constants.hpp>
#include "Vec2FieldSystem.h"

// #region Public Methods
void Vec2FieldSystem::Update(const GravityPhysicsSystem &physicsSystem, std::vector<GameObject> &physicsObjects,
                             std::vector<GameObject> &vectorField) {
    // For each field line we calculate the net gravitational force for that point in space
    for (auto &vf: vectorField) {
        glm::vec2 direction = glm::vec2();
        for (auto &object: physicsObjects) {
            direction += physicsSystem.ComputeForce(object, vf);
        }

        // This scales the length of the field line based on the log of the length
        // values were chosen just through trial and error based on what I liked the look
        // of and then the field line is rotated to point in the direction of the field
        vf.transform2DComponent.Scale.x =
                0.005f + 0.045f * glm::clamp(glm::log(glm::length(direction) + 1) / 3.f, 0.f, 1.f);
        vf.transform2DComponent.Rotation = atan2(direction.y, direction.x);
    }
}


// #endregion Public Methods
