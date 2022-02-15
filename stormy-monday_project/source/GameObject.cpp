//-----------------------------------------------------------------------------
// File Name:    GameObject.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------

#include "sol.hpp"

#include "GameObject.h"
#include "GameObjectManager.h"
#include "RigidBody.h"
#include "PhysicsSystem.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteRenderer.h"
#include "ParticleSystem.h"
#include <string>
#include <algorithm>
#include "TraceLog.h"


// Constructor & Destructor
GameObject::GameObject(const std::string name)
{
  _name = name;
  _tag = "";

  // Every Gameobject comes with a default transform component
  transform = new Transform();

  AddComponent(transform);
   
  
  // SceneManager::Inst()->GetActiveScene()->GetGameObjectManager()->Add(this);
  // parentScene = SceneManager::Inst()->GetActiveScene();  
}

// Copy constructor
GameObject::GameObject(const GameObject& gameObject)
{
	_name = gameObject.Name();
	_tag  = gameObject.Tag();

   //components = std::vector<ComponentPtr>();
   //for (auto compIt = gameObject.components.begin(); compIt != gameObject.components.end(); ++compIt)
	  // AddComponent((*compIt)->Clone());

    // Copying components
    for (auto compIter : gameObject.components)
      AddComponent( compIter.second->Clone() );

    // TODO: Check if this works
    // components = gameObject.components;

//	GameObjectManager::Inst()->Add(this);
   // SceneManager::Inst()->GetActiveScene()->GetGameObjectManager()->Add(this);
   // parentScene = SceneManager::Inst()->GetActiveScene();

}

GameObject::~GameObject()
{
  DeleteAllComponents();
}

// Static Class Methods
GameObjectPtr GameObject::FindByName(const std::string& name)
{
   return SceneManager::Inst()->GetActiveScene()->GetGameObjectManager()->FindByName(name);  // Will only search one active scene for now
}

GameObjectPtr GameObject::FindGameObjectByTag( const std::string& tag )
{
    return nullptr;
}

std::vector<GameObjectPtr> GameObject::FindGameObjectsByTag(const std::string& tag)
{
  return SceneManager::Inst()->GetActiveScene()->GetGameObjectManager()->FindGameObjectsByTag(tag);
}

//-----------------------------------------------------------------------------
// Public Class Methods
//-----------------------------------------------------------------------------
GameObjectPtr GameObject::Clone()
{
  return new GameObject(*this);
}

//
//ComponentPtr GameObject::AddComponent( ComponentPtr component )
//{
//  if (!component) return nullptr;
//
//  // Set the component's parent
//  component->Parent(this);
//
//  // Adding component to list
//  components.push_back(component);
//
//  // Sorting for consistence updating
//  std::sort(components.begin(), components.end(), ComponentSorter() );
//
//  return component;
//}

Transform& GameObject::GetTransform()
{
  return *transform;
}


void GameObject::DeleteAllComponents()
{
  for (ComponentIterator it = components.begin(); it != components.end(); ++it)
  {
    // std::cout << "Deleting component " << (*it).first.name() << " from gameobject " << _name << std::endl;

    (*it).second->Shutdown();
    // strcmp((*it).first.name(), "RigidBody") == 0;

    if (it->second)
    {
        delete it->second;
        it->second = nullptr;
    }
  }

  // Delete Component
  components.clear();
}

void GameObject::Start()
{
  // Update Components
  for (auto it = components.begin(); it != components.end(); ++it)
  {
    (*it).second->Start();
  }

  hasStarted = true;
}

void GameObject::Update(float dt)
{
  // Update Components
  for (auto it = components.begin(); it != components.end(); ++it)
  {
	  if ((*it).second != NULL && (*it).second->IsEnabled())
	  {
		  (*it).second->Update(dt);
	  }
  }
}

void GameObject::Shutdown()
{
  // Update Components
  for (auto it = components.begin(); it != components.end(); ++it)
  {
    (*it).second->Shutdown();
  }
}

void GameObject::Destroy()
{
//  GameObjectManager::Inst()->Remove(_name);
  parentScene->GetGameObjectManager()->Remove(_name);
}

// Getters
std::string GameObject::Name() const
 {
   return _name;
 }

std::string GameObject::Tag() const
 {
   return _tag;
 }

bool GameObject::HasStarted()
{
  return hasStarted;
}

bool GameObject::IsDestroyed()
 {
   return isDestoryed;
 }

 // Setters
 void GameObject::Name(const std::string& name)
 {
   _name = name;
 }

  void GameObject::Tag(const std::string& tag)
  {
    _tag = tag;
  }

RigidBodyPtr GameObject::GetRigidbody()
{
	  // return Has(RigidBody);
  return GetComponent<RigidBody>();
}

CameraPtr GameObject::GetCamera()
{
  // return Has(Camera);
  return GetComponent<Camera>();
}

std::vector<HitboxPtr> GameObject::GetHitboxes()
{
  return GetComponents<Hitbox>();
}

HitboxPtr GameObject::GetLargestCircularHitbox()
{
    auto range = components.equal_range(typeid(Hitbox));

    float largest_radius = 0.0f;
    HitboxPtr largest_hitbox = NULL;

    for (auto it = range.first; it != range.second; ++it)
    {
        HitboxPtr hitbox = static_cast<HitboxPtr>((it)->second);

        if (hitbox != NULL)
        {
            if (hitbox->GetShape() == Hitbox::cCircle && hitbox->GetRadius() > largest_radius)
            {
                largest_hitbox = hitbox;

                largest_radius = hitbox->GetRadius();
            }
        }

        //componentSet.push_back(static_cast<component*>((it)->second));
    }

    return largest_hitbox;
}

SpriteRendererPtr GameObject::GetSpriteRenderer()
{
  return GetComponent<SpriteRenderer>();
}

std::vector<SpriteRendererPtr> GameObject::GetSpriteRenderers()
{
  return GetComponents<SpriteRenderer>();
}

ParticleSystemPtr GameObject::GetParticleSystem()
{
  return GetComponent<ParticleSystem>();
}

std::vector<ParticleSystemPtr> GameObject::GetParticleSystems()
{
  return GetComponents<ParticleSystem>();
}

// unsigned int GameObject::ComponentTypeCount(Component::ComponentType cType) const
//   {
// 	  unsigned int componentCount = 0;
//
// 	  for (ComponentConstIterator it = components.begin(); it != components.end(); ++it)
// 	  {
// 		  if ((*it)->Type() == cType)
// 		  {
// 			  componentCount++;
// 		  }
// 	  }
//
// 	  return componentCount;
//   }

sol::table GameObject::GetScript( const std::string& scriptName )
{
  std::vector<LuaBehave*> luaBehaves = GetComponents<LuaBehave>();

  for (auto it : luaBehaves)  // TODO: May need to use dereference
  {
	  if (it->ScriptNameNoExt() == scriptName)
		  return it->GetScriptInst();
  }
 
  //TraceLog::Message("Getting Script: ", scriptName, " not found.");
 
  return sol::nil;
}



// std::vector<ComponentPtr> GameObject::GetMultipleComponents(Component::ComponentType cType)
// {
// 	std::vector<ComponentPtr> componentSet;
//  //
// 	// for (ComponentIterator it = components.begin(); it != components.end(); ++it)
// 	for ()
// 	{
// 	//
// 	 //  if ((*it)->Type() == cType)
// 	 //  {
// 		//   componentSet.push_back(*it);
// 	 //  }
//
//         
//
// 	}
//  
// 	return componentSet;
// 	
// }

  



//-----------------------------------------------------------------------------
// Private Methods
//-----------------------------------------------------------------------------
// ComponentPtr GameObject::FindComponent(Component::ComponentType type)
// {
// 	return BinarySearchComponent(type);
// }
//
// ComponentPtr GameObject::BinarySearchComponent(Component::ComponentType type)
// {
// 	size_t begin = 0;
// 	size_t end = components.size();
//
//   while (begin < end)
//   {
// 	  size_t mid = (begin + end) / 2;
//
// 	  if (components[mid]->Type() < type)
// 		  begin = mid + 1;
// 	  else
// 		  end = mid;
//   }
//
//   return ((begin < components.size()) && (components[begin]->Type() == type) ? components[begin] : nullptr);
// }
