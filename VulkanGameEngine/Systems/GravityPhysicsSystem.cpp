//
// Created by ShaneMonck on 27/03/2024.
//

#include "GravityPhysicsSystem.h"

// #region Private Methods

void GravityPhysicsSystem::stepSimulation(std::vector<GameObject> &physicsObjects, float stepDeltaTime) {
    // Loops through all pairs of objects and applies attractive force between them
    for (auto iterA = physicsObjects.begin(); iterA != physicsObjects.end(); ++iterA) {
        auto &objectA = *iterA;
        for (auto iterB = iterA; iterB != physicsObjects.end(); ++iterB) {
            if (iterA == iterB) {
                continue;
            }
            auto &objectB = *iterB;

            auto force = ComputeForce(objectA, objectB);
            objectA.rigidBody2DComponent.velocity += stepDeltaTime * -force / objectA.rigidBody2DComponent.mass;
            objectB.rigidBody2DComponent.velocity += stepDeltaTime * -force / objectB.rigidBody2DComponent.mass;
        }
    }

    for (auto &object: physicsObjects) {
        object.transform2DComponent.Translation += stepDeltaTime * object.rigidBody2DComponent.velocity;
    }
}
// #endregion

// #region Construction
GravityPhysicsSystem::GravityPhysicsSystem(float strength) {
    this->gravityStrength = strength;
}

// #endregion

// #region Public Methods

// deltaTime specifies the amount of time to advance the simulation
// subSteps is how many intervals to divide the forward time step in. More subSteps result in a
// more stable simulation, but takes longer to compute
void GravityPhysicsSystem::Update(std::vector<GameObject> &objects, float deltaTime, unsigned int subSteps) {
    const float stepDelta = deltaTime / subSteps;
    for (int i = 0; i < subSteps; i++) {
        stepSimulation(objects, stepDelta);
    }

}

glm::vec2 GravityPhysicsSystem::ComputeForce(GameObject &fromObject, GameObject &toObject) const {
    auto offset = fromObject.transform2DComponent.Translation - toObject.transform2DComponent.Translation;
    float distanceSquared = glm::dot(offset, offset);

    // just return 0 if objects are too close together...
    if (glm::abs(distanceSquared) < 1e-10f) {
        return glm::vec2(.0f, .0f);
    }

    float force = gravityStrength * toObject.rigidBody2DComponent.mass * fromObject.rigidBody2DComponent.mass /
                  distanceSquared;

    return force * offset / glm::sqrt(distanceSquared);
}



// #endregion
