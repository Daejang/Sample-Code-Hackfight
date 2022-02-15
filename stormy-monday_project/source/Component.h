/*!****************************************************************************
\file   Component.h
\author James Milton and Kalby Jang (for Start and Shutdown)
\brief  Contains a base component class for all of the game object components 
        to inherit from
//
// Copyright 2020 DigiPen (USA) Corporation.
//
******************************************************************************/

#pragma once

  // forward reference to game object class pointer
typedef class GameObject GameObject, *GameObjectPtr;

  // creates a label for component pointers
typedef class Component Component, *ComponentPtr;

  // serves as base class for each of the game object components
typedef class Component
{
  // public functions
public:

	  // enum for labeling each of the different components
	typedef enum ComponentType
	{
		cAnimation,
		cBehavior,
    cLuaBehave,
		cRigidBody,
    cSprite,
		cSpriteRenderer,
		cTransform,
		cHitbox,
    cParticle,
    cCamera,
	
    bPlayer,
    bPlayerManager,
    bRat,
    bRatManager,
    bHealth,

		cTypeCount
	} ComponentType;

	  // constructor for a component
	Component(Component::ComponentType cType) : cType(cType), inSystem(false) {};
  virtual ~Component() {}

	  // indicates the type of the component
	ComponentType Type() const { return cType; }

  void Type(ComponentType type) { cType = type; }

	  // sets the component to a game object
	void Parent(GameObjectPtr parent) { cParent = parent; }

	  // indicates what game object the component is attached to. 
    // --- Don't use until after the constructor call ---
	GameObjectPtr Parent() const { return cParent; }

	  // clones the component for use by another game object (pure virtual function)
	virtual ComponentPtr Clone() const = 0;

  // starts the component
  virtual void Start() {};

	  // updates the component (different for each component)
	virtual void Update(float dt) { dt = 0; }

  // shuts down the component
  virtual void Shutdown() {};

	  // draws the component on the screen (some components might not use this function)
	// virtual void Draw() const {};

  // Public Methods
  bool isInSystem() const { return inSystem; }
  void addToSystem() { inSystem = true; }
  void removeFromSystem() { inSystem = false; }

  bool IsEnabled() const { return isEnabled; }
  void SetEnabled(bool enabled) { isEnabled = enabled; }



  // private data
private:

	  // the type of the component (Animation, Behavior, etc.)
	ComponentType cType;

	  // the game object that the component is attached to
	GameObjectPtr cParent = nullptr;

	bool inSystem;
  bool isEnabled = true;

}Component;