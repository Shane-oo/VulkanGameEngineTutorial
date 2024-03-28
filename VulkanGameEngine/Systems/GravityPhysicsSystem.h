//
// Created by ShaneMonck on 27/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_GRAVITYPHYSICSSYSTEM_H
#define VULKANGAMEENGINETUTORIAL_GRAVITYPHYSICSSYSTEM_H


#include <vector>
#include "../GameObject.h"

class GravityPhysicsSystem {
public:
    GravityPhysicsSystem(float strength);
    
    void Update(std::vector<GameObject>& objects, float deltaTime, unsigned int subSteps = 1);

    glm::vec2 ComputeForce(GameObject& fromObject, GameObject& toObject) const;

private:
    float gravityStrength;
    
    void stepSimulation(std::vector<GameObject>& physicsObjects, float stepDeltaTime);
    
};


#endif //VULKANGAMEENGINETUTORIAL_GRAVITYPHYSICSSYSTEM_H
