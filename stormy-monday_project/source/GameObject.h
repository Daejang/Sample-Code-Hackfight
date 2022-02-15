//-----------------------------------------------------------------------------
// File Name:    GameObject.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once

#include <vector>

#include "Scene.h"
#include "Component.h"
#include "Transform.h"
#include "LuaBehave.h"
#include "Camera.h"
#include "sol.hpp"
#include "magic_enum.hpp"
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include "TraceLog.h"
#include "Hitbox.h"
#include <map>

#include "Sprite.h"
#define Has(type) GetComponent<type>(Component::c##type)

//-----------------------------------------------------------------------------
// Forward References
//-----------------------------------------------------------------------------
typedef class GameObject GameObject, *GameObjectPtr;
typedef class Component Component, *ComponentPtr;
typedef class Transform Transform;
typedef class LuaBehave LuaBehave, *LuaBehavePtr;
typedef class RigidBody RigidBody, *RigidBodyPtr;
typedef class Hitbox Hitbox, *HitboxPtr;
typedef class SpriteRenderer SpriteRenderer, *SpriteRendererPtr;
typedef class ParticleSystem ParticleSystem, *ParticleSystemPtr;




// ComponentIterator
//typedef std::vector<ComponentPtr>::iterator ComponentIterator;
//typedef std::vector<ComponentPtr>::const_iterator ComponentConstIterator;




//-----------------------------------------------------------------------------
// GameObject Class
//-----------------------------------------------------------------------------
class GameObject
{
public:
  typedef std::multimap<std::type_index, ComponentPtr>::iterator ComponentIterator;
  typedef std::multimap<std::type_index, ComponentPtr>::const_iterator ComponentConstIterator;

  struct ComponentIter
  {
    ComponentIterator begin_;
    ComponentIterator end_;

    ComponentIterator begin() const
    {
      return begin_;
    }

    ComponentIterator end() const
    {
      return end_;
    }
  };

 

  GameObject(const std::string name = "");
  GameObject(const GameObject& gameObject);

  ~GameObject();

  GameObjectPtr Clone();

  // Static Methods
  static GameObjectPtr FindByName(const std::string& name);
  static GameObjectPtr FindGameObjectByTag( const std::string& tag );
  static std::vector<GameObjectPtr> FindGameObjectsByTag(const std::string& tag);

  // Class Methods
  void Start();
  void Update(float dt);
  void Shutdown();
  void Destroy();

  // Component Access Methods
  //ComponentPtr AddComponent( ComponentPtr component );

  template <typename Tcomponent>
  Tcomponent* AddComponent(Tcomponent* newComp)
  {
      // static_assert(!std::is_same_v<Component, Tcomponent> && std::is_base_of_v<Component, Tcomponent>); 

      newComp->Parent(this);

      components.emplace( std::type_index( typeid(Tcomponent) ), newComp);
      
      return newComp;
  }

  template <typename component>
  unsigned int ComponentTypeCount() const
  {
    return components.count( typeid( component ) );
  }

  template <typename component>
  component* GetComponent(Component::ComponentType type)
  {
	  // return static_cast<component*>(FindComponent(type));

    return GetComponent<component>();
  };

  template <typename component>
  component* GetComponent()
  {
    //std::string typeStr = "c" + std::string(typeid(component).name());
    //TraceLog::Message( "Type Str is ", typeStr );
	  // return static_cast<component*>( FindComponent( magic_enum::enum_cast<Component::ComponentType>( typeStr ) ));
	  //return static_cast<component*>( FindComponent( Component::cHitbox ));

    auto range = components.find( typeid(component) );

    // if (range.first == range.second) return nullptr;
    if (range == components.end()) return nullptr;

    return static_cast<component*>( range->second );
  };

  sol::table GetScript( const std::string& scriptName ); 
  // std::vector<ComponentPtr> GetMultipleComponents(Component::ComponentType cType);

  template< typename component >
  std::vector<component*> GetComponents()
  {
    std::vector<component*> componentSet;
    //
    // for (ComponentIterator it = components.begin(); it != components.end(); ++it)
    // {
    //   if (*it.first == typeid(component))
    //   {
    //     componentSet.push_back(  )
    //   }
    // }
    //
    // return componentSet;


    auto range = components.equal_range( typeid(component) );

    for (auto it = range.first; it != range.second; ++it)
    {
      componentSet.push_back( static_cast<component*>( (it)->second) );
    }

    return componentSet;
  }

    template< typename component >
  ComponentIter GetComponentsIter()
  {   
    std::pair<ComponentIterator, ComponentIterator> range = components.equal_range( typeid(component) );

    return ComponentIter{ range.first, range.second };
  }



  // Getters-------------------------------------------------------------------
  std::string Name() const;
  std::string Tag() const;
  Transform&  GetTransform();  // Trying to deprecate this

  bool HasStarted();
  bool IsDestroyed();

  // Setters-------------------------------------------------------------------
  void Name(const std::string& name);
  void Tag(const std::string& tag);

  // Lua -----------------------------------------------
  RigidBodyPtr GetRigidbody();
  CameraPtr    GetCamera();
  std::vector< HitboxPtr > GetHitboxes();
  SpriteRendererPtr GetSpriteRenderer();
  std::vector<SpriteRendererPtr> GetSpriteRenderers();
  ParticleSystemPtr GetParticleSystem();
  std::vector< ParticleSystemPtr > GetParticleSystems();
  HitboxPtr GetLargestCircularHitbox();


  // Public Variables
  TransformPtr transform;
  ScenePtr parentScene;


private:
  std::string _name;
  std::string _tag;

  bool hasStarted = false;
  bool isDestoryed = false;  // TODO: correct name

  
  //std::vector<ComponentPtr> components;  // List of components
  std::multimap<std::type_index, ComponentPtr> components;


  // Private Methods ----------------------------------------------------------

  void DeleteAllComponents();

  // ComponentPtr FindComponent(Component::ComponentType type);
  // ComponentPtr BinarySearchComponent(Component::ComponentType type);
};



//
//template<typename component>
//component* GameObject::GetThatComponent(Component::ComponentType type)
//{
//	return static_cast<component*>(FindComponent( type ));
//}

//-----------------------------------------------------------------------------
// Private Structures
//-----------------------------------------------------------------------------

// struct ComponentSorter
// {
// 	bool operator()(ComponentPtr left, ComponentPtr right) const
// 	{
// 		return left->Type() < right->Type();
// 	}
// };

