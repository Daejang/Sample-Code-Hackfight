/*!****************************************************************************
\file   RaycastHitData.h
\author Kalby Jang
\brief  Data class for when raycasts hit something
//
// Copyright 2020 DigiPen (USA) Corporation.
//
 *****************************************************************************/

#pragma once

#include <glm/vec2.hpp>
#include "Collider.h"

typedef class ColliderData ColliderData, *ColliderDataPtr;
typedef class Hitbox Hitbox, *HitboxPtr;
typedef class RigidBody RigidBody, *RigidBodyPtr;
typedef class Transform Transform, *TransformPtr;

typedef class RaycastHitData RaycastHitData, *RaycastInfoPtr;


class RaycastHitData
{
public:

  RaycastHitData() = default;

  ColliderDataPtr collider   = nullptr;
  HitboxPtr       hitbox     = nullptr;
  GameObjectPtr   gameobject = nullptr;
  TransformPtr    transform  = nullptr;
  RigidBodyPtr    rigidBody  = nullptr;

  float distance = 0.0f;
  glm::vec2 normal = glm::vec2(0.0f);
  glm::vec2 point  = glm::vec2(0.0f);

};
