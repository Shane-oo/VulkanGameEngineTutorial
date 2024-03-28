//
// Created by ShaneMonck on 27/03/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_VEC2FIELDSYSTEM_H
#define VULKANGAMEENGINETUTORIAL_VEC2FIELDSYSTEM_H


#include "GravityPhysicsSystem.h"

class Vec2FieldSystem {
public:
    void Update(const GravityPhysicsSystem& physicsSystem,std::vector<GameObject>& physicsObjects, std::vector<GameObject>& vectorField);
};


#endif //VULKANGAMEENGINETUTORIAL_VEC2FIELDSYSTEM_H
