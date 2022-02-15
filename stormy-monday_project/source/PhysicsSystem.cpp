/*!****************************************************************************
\file   PhysicsSystem.cpp
\author James Milton, Kalby Jang
\brief  Contains functions that deal with the physics system which updates
        the physics components and checks colliders
//
// Copyright 2020 DigiPen (USA) Corporation.
//
******************************************************************************/

#include "Hitbox.h"

#include "Cheats.h"
#include "PhysicsSystem.h"
#include <iostream>
#include <iomanip>
#include "Collision.h"
#include "CollisionHandler.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "TraceLog.h"
#include "Transform.h"
#include "Deserialization.h"
#include "SceneManager.h"

#include "Affine.h"

// used for determining the area around an object where collisions will be checked
#define DISTANCE_VAL 1.5

  // a constructor for the physics system
Physics::Physics()
{
	  // sets up the list of rigid bodies

	physicsList = new std::vector<RigidBodyPtr>;
	collisionSubsystem = new Collision();
	collisionRecord = new CollisionRecord();
}


// a destructor for the physics system
Physics::~Physics()
{
	if (physicsList != NULL)
	{
		delete physicsList;

		physicsList = NULL;
	}

	if (collisionSubsystem != NULL)
	{
		delete collisionSubsystem;

		collisionSubsystem = NULL;
	}

	if (collisionRecord != NULL)
	{
		delete collisionRecord;

		collisionRecord = NULL;
	}

	//static_hitboxes.clear();
}

  // adds a rigid body to the physics system vector
void Physics::Add(RigidBodyPtr rigidBody)
{
	  // if the passed-in rigid body exists
	if (rigidBody != NULL)
	{
		  // pushes back the rigid body to the end of the vector
		physicsList->push_back(rigidBody);
	}
}

void Physics::Remove(RigidBodyPtr rigidBody)
{
	if (rigidBody != NULL)
	{
		for (physicsListIter = physicsList->begin(); physicsListIter != physicsList->end(); ++physicsListIter)
		{
			if (*physicsListIter == rigidBody)
			{
				physicsList->erase(physicsListIter);
		    return;  // Need to exit when found
			}
		}
	}
}


unsigned int Physics::GetVectorSize() const
{
	if (physicsList != NULL)
	{
		return physicsList->size();
	}

	return 0;
}

const std::vector<RigidBodyPtr> * Physics::GetVector() const
{
	return physicsList;
}

const CollisionPtr Physics::GetCollisionSubsystem()
{
	return collisionSubsystem;
}

void Physics::SetGravity(float cgravity)
{
	gravity = cgravity;
}

void Physics::SetTerminalVelocity(float cterminalVelocity)
{
	terminal_velocity = cterminalVelocity;
}

float Physics::GetGravity()
{
	return gravity;
}

float Physics::SetTerminalVelocity() const
{
	return terminal_velocity;
}

void Physics::SetComponentGravity(RigidBodyPtr rigidbody)
{
	if (rigidbody->IsAffectedByGravity())
	{
		vec3 acceleration = *(rigidbody->GetAcceleration());
		acceleration.y = -gravity * rigidbody->GetGravityEffectMultiplier();
		rigidbody->SetAcceleration(&acceleration);

	}
}

void Deserialization::DeserializePhysicsSystemInformation(float * cgravity, float * cterminal_velocity)
{
	SetDocument("data/in/physics_system.json");

	assert(document_->IsObject());
	assert(document_->HasMember("PhysicsSystem"));

	if ((*document_)["PhysicsSystem"].HasMember("Gravity") && (*document_)["PhysicsSystem"].HasMember("TerminalVelocity"))
	{
		rapidjson::Value::MemberIterator gravity = (*document_)["PhysicsSystem"].FindMember("Gravity");
		rapidjson::Value::MemberIterator terminal_velocity = (*document_)["PhysicsSystem"].FindMember("TerminalVelocity");

		*cgravity = (*gravity).value.GetFloat();
		*cterminal_velocity = (*terminal_velocity).value.GetFloat();
	}
	else
	{
		TraceLog::Message("Physics System JSON file does not have the required information");
	}
}

  // starts up the physics system
void Physics::Start()
{
	  // constructs the physics system

	SetPauseStatus(true);

	float cgravity = 0.0f;
	float cterminal_velocity = 0.0f;

	Deserialization::Inst()->DeserializePhysicsSystemInformation(&cgravity, &cterminal_velocity);

	SetGravity(cgravity);
	SetTerminalVelocity(cterminal_velocity);

	//LoadStaticHitboxes();
}

std::vector<HitboxPtr> Physics::GetRestOfHitboxes(HitboxPtr hitbox)
{
	std::vector<HitboxPtr> hitboxes;

	hitboxes.push_back(hitbox);

	if (hitbox->GetRelatedHitboxesNum() > 0)
	{
		std::vector<HitboxPtr> restOfHitboxes = hitbox->GetRelatedHitboxes();

		for (unsigned int hitbox_pos = 0; hitbox_pos < restOfHitboxes.size(); hitbox_pos++)
		{
			hitboxes.push_back(restOfHitboxes[hitbox_pos]);
		}
	}

	return hitboxes;
}

  // updates all of the physics-related components
void Physics::Update(float dt)
{
	collisionSubsystem->collide_data_stack->moveToNextFrame();

	  // for each rigid body stored in the vector
	for (physicsListIter = physicsList->begin(); physicsListIter != physicsList->end(); physicsListIter++)
	{
		// if the rigid body exists
		if (*physicsListIter != NULL)
		{
			if ((*physicsListIter)->Parent() == NULL)
			{
				(*physicsListIter)->~RigidBody();
				(*physicsListIter) = NULL;
				continue;
			}

			SetComponentGravity(*physicsListIter);


			// updates the rigid body (translation, velocity, acceleration)
			(*physicsListIter)->UpdatePosition(dt);



		}
	}

	for (physicsListIter = physicsList->begin(); physicsListIter != physicsList->end(); physicsListIter++)
	{
		if (*physicsListIter != NULL)
		{
			  // retrieves the parent game object
			GameObjectPtr parentObject = (*physicsListIter)->Parent();
			// retrieves the hitbox
			HitboxPtr hitbox = parentObject->GetComponent<Hitbox>(Component::cHitbox);
			// retrieves the transform component
//			TransformPtr transform = parentObject->GetComponent<Transform>(Component::cTransform);
			Transform transform = parentObject->GetTransform();

			std::vector<HitboxPtr> hitboxes;

			if (hitbox == NULL)
			{
				continue;
			}
			else
			{
				hitboxes = GetRestOfHitboxes(hitbox);
			}

			CheckCollisions(dt, physicsListIter, physicsListIter + 1, hitboxes, transform);
		}
	}

	collisionSubsystem->collisionHandler->CollisionHandlerCheck(collisionSubsystem->collide_data_stack);

	UpdateTransforms(dt);
}

void Physics::CheckCollisions(float dt, std::vector<RigidBodyPtr>::iterator physicsIter, std::vector<RigidBodyPtr>::iterator starting_point, std::vector<HitboxPtr> hitboxes, Transform transform)
{
	float min_dist = INFINITY;

	collisionRecord->EmptyCollisionRecord();

		// creates an iterator for going through the rest of the rigid bodies succeeding the current rigid body
	std::vector<RigidBodyPtr>::iterator restIter;
		// for each rigid body left in the vector after the current one
	for (restIter = starting_point; restIter != physicsList->end(); restIter++)
	{
		if (*restIter != NULL)
		{

			// sets up a value for the scale of the first rigid body
			float scaleA = 0.0f;
			// sets up a value for the scale of the second rigid body being tested against the first rigid body
			float scaleB = 0.0f;

			// retrieves the parent object of the second rigid body
			GameObjectPtr restObject = (*restIter)->Parent();
			// retireves the hitbox associated with the second rigid body
			HitboxPtr restHitbox = restObject->GetComponent<Hitbox>(Component::cHitbox);

			// retrieves the transform associated with the second rigid body
//				TransformPtr restTransform = restObject->GetComponent<Transform>(Component::cTransform);
			Transform restTransform = restObject->GetTransform();

			std::vector<HitboxPtr> restHitboxes;

			if (restHitbox == NULL)
			{
				continue;
			}
			else
			{
				restHitboxes = GetRestOfHitboxes(restHitbox);
			}

			vec3 old_pos = *((*physicsIter)->GetTranslation());
			vec3 velocity = *((*physicsIter)->GetVelocity());
			vec3 acceleration = *((*physicsIter)->GetAcceleration());
			vec3 vel_accel = (velocity + acceleration * dt) * dt;
			float vel_accel_len = length(vel_accel);

			vec3 rest_old_pos = *((*restIter)->GetTranslation());
			vec3 restVelocity = *((*restIter)->GetVelocity());
			vec3 restAcceleration = *((*restIter)->GetAcceleration());
			vec3 rest_vel_accel = (restVelocity + restAcceleration * dt) * dt;
			float rest_vel_accel_len = length(rest_vel_accel);

			bool saveHitboxes = false;

			for (unsigned int hitbox_pos = 0; hitbox_pos < hitboxes.size(); hitbox_pos++)
			{
				for (unsigned int restHitbox_pos = 0; restHitbox_pos < restHitboxes.size(); restHitbox_pos++)
				{
					HitboxPtr local_hitbox = hitboxes[hitbox_pos];
					HitboxPtr local_restHitbox = restHitboxes[restHitbox_pos];

					// if the hitbox associated with the first rigid body is polygonal
					if (local_hitbox->GetShape() == Hitbox::cBoxlike)
					{
						// retrieves the general scale of the polygonal hitbox
						scaleA = local_hitbox->DetermineBoxScale();
					}
					// if the first hitbox is actually circular
					else
					{
						// retrieves the radius of the circular hitbox
						scaleA = local_hitbox->GetRadius();
					}

					// if the hitbox associated with the second rigid body is polygonal
					if (local_restHitbox->GetShape() == Hitbox::cBoxlike)
					{
						// retrieves the general scale of the polygonal hitbox
						scaleB = local_restHitbox->DetermineBoxScale();
					}
					// if the second hitbox is actually circular
					else
					{
						// retrieves the radius of the circular hitbox
						scaleB = local_restHitbox->GetRadius();
					}

					// need to make it so that each of the trigger boxes are placed in the collision record regardless of
					// distance and make it so that the min_dist is based on hitboxes that need to be resolved (afterwards,
					// we will have to go through the recorded trigger boxes and eliminate those that have a greater distance
					// than the resolution with the least distance)

					float areaLimit = (scaleA + vel_accel_len) + (scaleB + rest_vel_accel_len);

					// if the distance between the positions of the two rigid bodies is near enough to one another to justify checking collision
					//if (distance(transform.Position(), restTransform.Position()) < (scaleA + vel_accel_len) + (scaleB + rest_vel_accel_len))
					//{
						// detects and resolves collision between the two rigid bodies
					if (collisionSubsystem->CollisionDetectionManager(local_hitbox, local_restHitbox, areaLimit, transform.Position(), restTransform.Position(), old_pos, rest_old_pos, collisionRecord))
					{
						saveHitboxes = true;
					}

				}
			}

			if (saveHitboxes == true)
			{
				if (collisionRecord->DetermineIfResolution() == false)
				{
					collisionRecord->SortCollisionInfos();

					triggerRecord.push_back(*collisionRecord);

					collisionRecord->EmptyCollisionRecord();
				}
				else
				{
					float collisionRecordSmallestDist = collisionRecord->GetSmallestResolutionDistance();

					if (collisionRecordSmallestDist < min_dist)
					{
						collisionRecord->SortCollisionInfos();

						min_dist = collisionRecordSmallestDist;
					}
				}
			}
		}
	}



	if (collisionRecord->GetSmallestResolutionDistance() == INFINITY)
	{
		collisionRecord->EmptyCollisionRecord();
	}

	if (triggerRecord.empty() == false)
	{
		SortAndAdjustTriggerRecord(min_dist);

		for (std::vector<CollisionRecord>::iterator trigger_record_it = triggerRecord.begin(); trigger_record_it != triggerRecord.end(); trigger_record_it++)
		{
			std::vector<CollisionInfo> triggerInfos = (*trigger_record_it).GetCollisionInfos();

			for (std::vector<CollisionInfo>::iterator trigger_info_it = triggerInfos.begin(); trigger_info_it != triggerInfos.end(); trigger_info_it++)
			{
				if ((*trigger_info_it).hitbox_a->Parent()->Name() == "1" && (*trigger_info_it).hitbox_b->Parent()->Name() == "Anchor")
				{
					if (1)
					{

					}
				}

				if ((*trigger_info_it).dist != INFINITY)
				{
					collisionSubsystem->CollisionResolutionManager((*trigger_info_it).hitbox_a, (*trigger_info_it).hitbox_b, (*trigger_info_it).collideEvent, (*trigger_info_it).line);
				}
			}
		}

		triggerRecord.clear();
	}

	if (collisionRecord->RecordIsEmpty() == false)
	{
		std::vector<CollisionInfo> collideInfos = collisionRecord->GetCollisionInfos();

		for (std::vector<CollisionInfo>::iterator collide_info_it = collideInfos.begin(); collide_info_it != collideInfos.end(); collide_info_it++)
		{
			if ((*collide_info_it).dist != INFINITY)
			{
				collisionSubsystem->CollisionResolutionManager((*collide_info_it).hitbox_a, (*collide_info_it).hitbox_b, (*collide_info_it).collideEvent, (*collide_info_it).line);

				if ((*collide_info_it).hitbox_a->GetResolutionType() == Hitbox::cCollider && (*collide_info_it).hitbox_b->GetResolutionType() == Hitbox::cCollider)
				{
					break;
				}
			}
		}

		collisionRecord->EmptyCollisionRecord();
	}
}

//void Physics::DoubleCheckCollisions()

void Physics::SortAndAdjustTriggerRecord(float min_dist)
{
	std::sort(triggerRecord.begin(), triggerRecord.end(), CollisionRecordComp());

	for (std::vector<CollisionRecord>::iterator col_rec_it = triggerRecord.begin(); col_rec_it != triggerRecord.end(); col_rec_it++)
	{
		if ((*col_rec_it).GetSmallestDistance() > min_dist)
		{
			triggerRecord.erase(col_rec_it, triggerRecord.end());

			break;
		}
	}
}

void Physics::UpdateTransforms(float dt)
{
	std::vector<GameObjectPtr> * gameObjects = SceneManager::Inst()->GetActiveScene()->GetGameObjects();
	// std::list<GameObjectPtr> * gameObjects = SceneManager::Inst()->GetActiveScene()->GetGameObjects();

	if (gameObjects != NULL)
	{
		std::vector<GameObjectPtr>::iterator gameObjectIter;
		// std::list<GameObjectPtr>::iterator gameObjectIter;

		for (gameObjectIter = gameObjects->begin(); gameObjectIter != gameObjects->end(); gameObjectIter++)
		{
			Transform & gameObjectTransform = (*gameObjectIter)->GetTransform();

			gameObjectTransform.UpdateTransform();
		}
	}
}

void Physics::UpdateObjectHitboxes(GameObjectPtr object, float dt)
{
	if (object != NULL)
	{
		std::vector<HitboxPtr> hitboxes = object->GetComponents<Hitbox>();
		std::vector<HitboxPtr>::iterator hitboxIter;

		for (hitboxIter = hitboxes.begin(); hitboxIter != hitboxes.end(); hitboxIter++)
		{
			//HitboxPtr hitbox = dynamic_cast<HitboxPtr>(*hitboxIter);

			(*hitboxIter)->UpdateHitbox(dt);
		}
	}
}

  // shuts down the physics system
void Physics::Shutdown()
{
	  // calls the destructor to erase the system
	this->~Physics();
}

bool Physics::PlayerIsInvinsible()
{
	return Cheats::godMode;
}

bool Physics::PlayerIsInvinsibleLua()
{
	return Inst()->PlayerIsInvinsible();
}

// ----------------------------------------------------------------------------
// Static Methods
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Class Raycast
// ----------------------------------------------------------------------------
RaycastHitData Physics::Raycast( glm::vec2 origin, glm::vec2 direction, float distance, GameObjectPtr skipObject)
{
  RaycastHitData tempRaycastHit;
  RaycastHitData returnRaycastHit;
  Ray tempRay;
  float closestCollider = INFINITY;

  std::vector<HitboxPtr> hitboxes;
  std::vector<HitboxPtr>::iterator HBIter;

  tempRay._origin    = origin;
  tempRay._direction = direction;


  // Getting all the hitboxes
  for (GameObjectPtr GO : *SceneManager::GetActiveScene()->GetGameObjects())
  {
	  // if (GO != skipObject)
	  // {
		  for (auto comp : GO->GetComponents<Hitbox>())
			  hitboxes.push_back(comp);
	  // }
  }

  // Check all hitboxes
  for (HBIter = hitboxes.begin(); HBIter != hitboxes.end(); ++HBIter)
  {
      // if (CastTo( tempRay, distance, tempRaycastHit, *HBIter ))
      //   return tempRaycastHit;

    if (CastTo( tempRay, distance, tempRaycastHit, *HBIter ))
    {
      // Getting the closest hit
      if (tempRaycastHit.distance < closestCollider)
      {
        closestCollider = tempRaycastHit.distance;
        returnRaycastHit = tempRaycastHit;
      }
    }
  }

  return returnRaycastHit;
}


bool Physics::CastTo( Ray& ray, float& maxDistance, RaycastHitData& data, HitboxPtr hitbox )
{
  vec2 rayUnit = ray._origin + ray._direction;
  data = RaycastHitData();
  float closestDist =  maxDistance;



  // For now, only check polygon colliders
  // if (hitbox->GetShape() != Hitbox::HitboxShape::cBoxlike) return false;

  switch (hitbox->GetShape())
  {
  case Hitbox::HitboxShape::cCircle:
	  return RayCircle( ray, maxDistance, data, hitbox );


	case Hitbox::HitboxShape::cBoxlike:
	  return RayPolygon( ray, maxDistance, data, hitbox );

  }


  // return data.gameobject != nullptr ? true : false;
  return false;
}

bool Physics::RayCircle(Ray& ray, const float& maxDistance, RaycastHitData& data, HitboxPtr circle)
{
  vec2 rayDir = ray._origin + ray._direction;
  vec2 center = circle->GetCenter();
  float radius = circle->GetRadius();

  /*if (circle->Parent()->Tag() == "Anchor")
  {
	  radius = circle->GetRadius();
  }*/

  // Check if ray starts inside of circle
  float boundDist = glm::distance( ray._origin, center );

  if (boundDist < radius) return false;

  // Begin the detection
  float t = glm::dot(center - ray._origin, rayDir);
  glm::vec2 p = ray._origin + ray._direction * t;

  // Inside circle calc
  float y = glm::length( center - p );

  // Exit on miss
  if (y > radius) return false;

  // Finish the calc
  float x = std::sqrtf(radius * radius - y * y);
  float t1 = t - x;

  vec2  point = ray._origin + ray._direction * t1;
  float dist  = glm::distance( data.point, ray._origin );

  // Exit if too long
  if (dist > maxDistance) return false;

  // Apply to data
  data.point    = point;
  data.distance = dist;
  data.normal   = glm::normalize( point - center );

  data.gameobject = circle->Parent();
  data.transform  = circle->Parent()->transform;
  data.rigidBody  = circle->Parent()->GetRigidbody();
  data.hitbox     = circle;

  return true;
}

bool Physics::RayPolygon( Ray& ray, const float& maxDistance, RaycastHitData& data, HitboxPtr polygon )
{
  vec2 rayVec = ray._origin + ray._direction;
  vec2 colliderLine;
  vec2 closestPoint;
  float closestDist =  maxDistance;
  bool found = false;


  // Checking all lines on the polygon collider
  for (const Hitbox::HitboxLine& line : polygon->GetLines())
  {
    // Get the denominator
    float den = (line.p0.x - line.p1.x) * (ray._origin.y - rayVec.y) - (line.p0.y - line.p1.y) * (ray._origin.x - rayVec.x);

    // Exit if line and ray are parallel
    if (Affine::near(den, 0.0f)) continue;

    // Hitbox line
    float l = ((line.p0.x - ray._origin.x) * (ray._origin.y - rayVec.y) - (line.p0.y - ray._origin.y) * (ray._origin.x - rayVec.x)) / den;
    // Ray line
    float r = -((line.p0.x - line.p1.x) * (line.p0.y - ray._origin.y) - (line.p0.y - line.p1.y) * (line.p0.x - ray._origin.x)) / den;

    // A hit
    // if (l > 0.0f && l < 1.0f && r > 0.0f) return true;
    // A miss
    if (l < 0.0f || l > 1.0f || r < 0.0f) continue;

    // Getting the contact point
    vec2 point = vec2( line.p0.x + l * (line.p1.x - line.p0.x), line.p0.y + l * (line.p1.y - line.p0.y));

    float dist = glm::distance( point, ray._origin );

    // If distance is too long
    if (dist > maxDistance) continue;

	  // Found detection
  	found = true;

	  // Check if it closest distance
	  if (dist <= closestDist)
	  {
		  colliderLine = glm::normalize( line.p1 - line.p0 );
		  closestDist = dist;
		  closestPoint = point;
	  }
   }

  if ( !found ) return false;

  // Record data
  data.point    = closestPoint;
	data.distance = closestDist;
  data.normal   = vec2(colliderLine.y , colliderLine.x );

  data.gameobject = polygon->Parent();
  data.transform  = polygon->Parent()->transform;
  data.rigidBody  = polygon->Parent()->GetRigidbody();
  data.distance   = glm::distance( ray._origin, data.point );
	data.hitbox     = polygon;

  return true;
}
