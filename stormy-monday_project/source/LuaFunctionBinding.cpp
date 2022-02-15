/*!****************************************************************************
\file   LuaFunctionBinding.cpp
\author Kalby Jang
\brief  Binds class functions into a lua state
//
// Copyright 2020 DigiPen (USA) Corporation.
//
 *****************************************************************************/
#include "sol.hpp"
#include "LuaFunctionBinding.h"


#include "Audio.h"
#include "PhysicsSystem.h"
#include "SceneManager.h"
// #include "Scene.h"
// #include "GameObjectManager.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "RigidBody.h"
#include "ParticleSystem.h"
#include "ParticleSystemModules.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Time.h"
#include "Input.h"
#include "Camera.h"
#include "Hitbox.h"

#include "magic_enum.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "Collider.h"
#include "Collision.h"
#include "RaycastHitData.h"
#include "PlayerStats.h"
#include "Bounds.h"
//#include "CollisionData.h"
#include "Timer.h"
#include "Renderer.h"
#include "Cheats.h"
#include "Resources.h"

//-----------------------------------------------------------------------------
// Forward References
//-----------------------------------------------------------------------------
// class LuaBinder;
class GameObject;
class Transform;
class RigidBody;
class LuaBehave;
class Input;
class ColliderData;
class RaycastHitData;

// --------------------------------------------------------------------------
// Class Function Binding
// --------------------------------------------------------------------------
void LuaFunctionBinding::BindFunctions(sol::state& luaState)
{
	// --------------------------------------------------------------------------
	// Variables
	// --------------------------------------------------------------------------
  luaState["DATA_FOLDER"]    = References::FilePaths::DATA_FOLDER;
  luaState["SCRIPTS_FOLDER"] = References::FilePaths::SCRIPTS_FOLDER;
  luaState["SOUND_FOLDER"]   = References::FilePaths::SOUNDS_FOLDER;
  luaState["TEXTURE_FOLDER"] = References::FilePaths::TEXTURES_FOLDER;
  luaState["LOGOS_FOLDER"]   = References::FilePaths::LOGOS_FOLDER;

  luaState["TEXTURES_DPLOGO"] = Resources::Textures::DPlogo;
  luaState["TEXTURES_TEAMLOGO"] = Resources::Textures::teamLogo;
  luaState["TEXTURES_FMODLOGO"] = Resources::Textures::FMODlogo;
  luaState["TEXTURES_USEITLOGO"] = Resources::Textures::USEITlogo;
  luaState["TEXTURES_TITLESCREENLOGO"] = Resources::Textures::titleScreenLogo;
  luaState["TEXTURES_FINALSCREENLOGO"] = Resources::Textures::finalScreenLogo;


  luaState["Cheats"]				 = Cheats::Inst();


	// --------------------------------------------------------------------------
	// Enter new datatypes
	// ------------------------------------------bounds--------------------------------
	luaState.new_usertype<glm::vec2>("vec2",
		sol::constructors< vec2(), vec2(float), vec2(float, float) >()
		, "x", &glm::vec2::x
		, "y", &glm::vec2::y

		, "Angle", sol::resolve< float(vec2 const&, vec2 const&) >( &glm::orientedAngle )
    , "Distance",  sol::resolve< float(vec2 const &, vec2 const &) >  ( &glm::distance )
    , "Dot",       sol::resolve< float (vec2 const &, vec2 const &) >( &glm::dot ) 
		, "Magnitude",    sol::resolve< float(vec2 const &) >( &glm::length )
    , "Normalize", sol::resolve< vec2(vec2 const &)>  ( &glm::normalize )
    , "Reflect", sol::resolve< vec2 (vec2 const &, vec2 const &)>( &glm::reflect )
		
		, "Right", sol::var( glm::vec2(1.0f, 0.0f) )
		, "Left", sol::var( glm::vec2 (-1.0f, 0.0f))
	  , "Up", sol::var( glm::vec2(0.0f, 1.0f) )
	  , "Down", sol::var( glm::vec2( 0.0f, -1.0f))
	  

    , sol::meta_function::addition, sol::resolve< vec2 (vec2 const&, vec2 const&)>( &glm::operator+ )
    , sol::meta_function::subtraction, sol::resolve< vec2 (vec2 const&, vec2 const&)>( &glm::operator- )
    , sol::meta_function::multiplication, sol::overload( 
	      sol::resolve< vec2 (vec2 const&, vec2 const&)>( &glm::operator* ),
	      sol::resolve< vec2 (vec2 const&, float)>( &glm::operator* ),
	      sol::resolve< vec2 (float, vec2 const&)>( &glm::operator* )
    )
    , sol::meta_function::division, sol::overload(
        sol::resolve< vec2 (vec2 const&, vec2 const&)>( &glm::operator/),
        sol::resolve< vec2 (vec2 const&, float)>( & glm::operator/)
    )
	  
    
  );

	
	luaState.new_usertype<glm::vec3>("vec3",
		sol::constructors< vec3(), vec3(float), vec3(float, float, float) >(),
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z

		);
	
	luaState.new_usertype<glm::vec4>("vec4",
		sol::constructors< vec4(), vec4(float), vec4(float, float, float, float) >(),
		"x", &glm::vec4::x,
		"y", &glm::vec4::y,
		"z", &glm::vec4::z,
		"w", &glm::vec4::w
		);

	
	luaState.new_usertype<ColliderData>("Collider",
		"gameobject", &ColliderData::gameobject,
		"transform", &ColliderData::transform,
		"rigidbody", &ColliderData::rigidbody,
		"isTrigger", &ColliderData::isTrigger,
		"name", &ColliderData::name,
		"tag", &ColliderData::tag
		);

	luaState.new_usertype<CollisionData>("Collision",
		"gameobject", &CollisionData::GetOtherObject

		);

  luaState.new_usertype<RaycastHitData>( "RaycastHit"
	  // , "collider", &RaycastHitData::collider
    , "gameobject", &RaycastHitData::gameobject
	  , "transform", &RaycastHitData::transform
	  , "rigidBody", &RaycastHitData::rigidBody
	  , "distance", &RaycastHitData::distance
    , "normal", &RaycastHitData::normal
    , "point", & RaycastHitData::point
	  );


  luaState.new_usertype<PlayerStats>( "playerStats" );

  luaState.new_usertype<Bounds2D>("Bounds2D"
    , sol::constructors<Bounds2D(vec2, vec2)>()
    , "Contains", &Bounds2D::Contains
    );

  luaState.new_usertype<Timer>( "Timer"
	  , sol::constructors< Timer(), Timer(float)>()
	  , "UpdateTimer", &Timer::UpdateTimer
	  , "IsDone", &Timer::IsDone
	  , "Reset", &Timer::Reset);
  
  luaState.new_usertype<Cheats>( "Cheats"
	  // , "godMode", &Cheats::godMode
	  // , "invisible", &Cheats::invisible
	  // , "speedMode", &Cheats::speedMod
	  // , "jumpMod", &Cheats::jumpMod
	  // , "timeScale", &Cheats::timeScale
	  );

  // --------------------------------------------------------------------------
	// Physics
	// --------------------------------------------------------------------------
  luaState.new_usertype<Physics>( "Physics"
      , "gravity", sol::property( 
		  &Physics::GetGravity,
		  &Physics::SetGravity)
      , "Raycast", &Physics::Raycast
	  , "PlayerIsInvinsible", &Physics::PlayerIsInvinsibleLua
	  //, "GetCollisionSubsystem", &Physics::GetCollisionSubsystem
	  // sol::resolve<std::tuple<bool, RaycastHitData >(vec2, vec2, float)>( &Physics::Raycast ) 
	  );


	// --------------------------------------------------------------------------
	// SceneManager
	// --------------------------------------------------------------------------
	luaState.new_usertype<SceneManager>("SceneManager"
		, "LoadScene", sol::overload(
			sol::resolve<void(const std::string&, const std::string&)>( &LoadScene ),
			sol::resolve<void(const int, const std::string&)>( &LoadScene )
			)
		, "GetSceneIndex", &SceneManager::GetSceneIndex
		, "GetActiveScene", &SceneManager::GetActiveScene
		);
	
	// // --------------------------------------------------------------------------
	// // Scene
	// // --------------------------------------------------------------------------
	luaState.new_usertype<Scene>("Scene",
		  "GetGameObjectManager", &Scene::GetGameObjectManager);
	
	// // --------------------------------------------------------------------------
	// // GameobjectManager
	// // --------------------------------------------------------------------------
	// luaState.new_usertype<GameObjectManager>("gameObjectManager",
		 //  "FindGameObjectByTag", &GameObjectManager::FindGameObjectsByTag);

	// --------------------------------------------------------------------------
	// Gameobject
	// --------------------------------------------------------------------------
	luaState.new_usertype<GameObject>("GameObject"
		, "name", sol::property(
			  sol::resolve< std::string(void) const >(&GameObject::Name),
			  sol::resolve< void(const std::string&) >(&GameObject::Name))
	
		, "tag", sol::property(
			  sol::resolve< std::string(void) const >(&GameObject::Tag),
			  sol::resolve< void(const std::string&) >(&GameObject::Tag))
	
		, "transform", &GameObject::transform
		, "FindGameObjectByName", &GameObject::FindByName
		, "FindGameObjectsByTag", &GameObject::FindGameObjectsByTag
		, "Destroy", &GameObject::Destroy
		, "GetScript", &GameObject::GetScript
		// , "GetComponent", sol::overload( 
  //       sol::resolve<Hitbox* (void)>(&GameObject::GetComponent<Hitbox>),
		// 	  sol::resolve<RigidBody* (void)>(&GameObject::GetComponent<RigidBody>) 
  //   )
		, "GetComponents", sol::overload( 
        &GameObject::GetComponents<Hitbox>,
			  &GameObject::GetComponents<RigidBody>
    )
		, "GetRigidbody", &GameObject::GetRigidbody
    , "GetCamera", &GameObject::GetCamera
	  , "GetHitboxes", &GameObject::GetHitboxes
		, "GetLargestCircularHitbox", &GameObject::GetLargestCircularHitbox
	  , "GetSprite", &GameObject::GetSpriteRenderer
	  , "GetSprites", &GameObject::GetSpriteRenderers
	  , "GetParticleSystem", &GameObject::GetParticleSystem
		, "GetParticleSystems", &GameObject::GetParticleSystems
		);

	luaState.new_usertype<Renderer>("Renderer",
		"DisplayAimingLine", &Renderer::DisplayAimingLineLua
		);

	// --------------------------------------------------------------------------
	// Component
	// --------------------------------------------------------------------------
	luaState.new_usertype<Component>( "Component"
		// , "gameobject", sol::overload(
		// 	  sol::resolve< void (GameObjectPtr) >( &Component::Parent),
		// 	  sol::resolve< GameObjectPtr (void) >( &Component::Parent)
		// )
		, "enabled", sol::property( 
      &Component::IsEnabled,
      &Component::SetEnabled
			)
		);

	// --------------------------------------------------------------------------
	// Transform
	// --------------------------------------------------------------------------
	luaState.new_usertype<Transform>("transform"
	  , sol::base_classes, sol::bases<Component>()
		,"position",
		  sol::property(
			  sol::resolve< const vec2&() const >(&Transform::Position2),  // Returning vec2 is a problem
			  sol::resolve< void(const vec2&) >(&Transform::Position)
			)
		,"rotation", sol::property(
			sol::resolve< const float() const >(&Transform::Rotation),
			sol::resolve< void(const float&) >(&Transform::Rotation)
			)
		,"scale", sol::property(
			sol::resolve< const vec2&() const >(&Transform::Scale2),
			sol::resolve< void(const vec2&) >(&Transform::Scale)
			)
		);

	// --------------------------------------------------------------------------
	// Rigidbody
	// --------------------------------------------------------------------------
	luaState.new_usertype<RigidBody>("rigidbody"
		, sol::base_classes, sol::bases<Component>()
		  , "translation", sol::property(
			&RigidBody::GetTranslation,
			&RigidBody::SetTranslation)
		, "velocity", sol::property(
			&RigidBody::GetVelocity,
			&RigidBody::SetVelocity)
		, "acceleration", sol::property(
			&RigidBody::GetAcceleration,
			&RigidBody::SetAcceleration)
		, "mass", sol::property(
			&RigidBody::GetMass,
			&RigidBody::SetMass)
		, "affectedByGravity", sol::property(
			  &RigidBody::IsAffectedByGravity,
			&RigidBody::SetGravityEffect)
		, "gravityScale", sol::property( 
			&RigidBody::GetGravityEffectMultiplier,
      &RigidBody::SetGravityEffectMultiplier)
    , "Swing", &RigidBody::ActivateSwingPhysics
    , "UnSwing", &RigidBody::DeactivateSwingPhysics
		);

  // --------------------------------------------------------------------------
	// SpriteRenderer
	// --------------------------------------------------------------------------
  luaState.new_usertype<SpriteRenderer>( "SpriteRenderer"
	  , sol::base_classes, sol::bases<Component>()
	  , "texture", sol::property(
		    sol::resolve< const TexturePtr() const >( &SpriteRenderer::Texture ),
		    sol::resolve< void( TexturePtr )>( &SpriteRenderer::Texture )
		  )
	  , "DeleteTexture", &SpriteRenderer::TextureToNull
	  
	  );

  // --------------------------------------------------------------------------
	// Texture
	// --------------------------------------------------------------------------
  luaState.new_usertype<Texture>( "Texture" 
	  , sol::constructors<  Texture(const std::string& filename) >()
	  // , sol::destructor( ~Texture ) 
	  , "file", sol::property( 
		    sol::resolve< const char*( void ) >( &Texture::FileName ),
	      sol::resolve< void( const char* ) >( &Texture::FileName )
		  )
	  
	  );

	// --------------------------------------------------------------------------
	// LuaBehave
	// --------------------------------------------------------------------------
	luaState.new_usertype<LuaBehave>("Script"
		, sol::base_classes, sol::bases<Component>()
		, "scriptName", sol::property(
			sol::resolve<const std::string&() const>(&LuaBehave::ScriptName),
			sol::resolve<void(const std::string&)>(&LuaBehave::ScriptName)
			)
		, "GetInst", &LuaBehave::GetScriptInst
		);

  // --------------------------------------------------------------------------
	// Swing Componenttransform
	// --------------------------------------------------------------------------
  // luaState.new_usertype<>(  )


// --------------------------------------------------------------------------
	// Particle System Modules
	// --------------------------------------------------------------------------
  luaState.new_usertype<BurstModule>( "BurstModule"
	  , "time", &BurstModule::time
	  , "particleCount", &BurstModule::particleCount
	  );

  luaState.new_usertype<EmissionModule>( "EmissionModule"
	  , "numberOfBursts", &EmissionModule::numberOfBursts
	  , "spawnRateByTime", &EmissionModule::spawnRateByTime
	  , "spreadAngle", &EmissionModule::spreadAngle
	  , "burst", &EmissionModule::burst
	  );

  luaState.new_usertype<MainModule>( "MainModule"
	  , "playDuration", &MainModule::playDuration
	  , "playOnStartUp", &MainModule::playOnStartUp
	  , "useGravity", &MainModule::useGravity
	  , "gravityMultiplier", &MainModule::gravityMultiplier
	  , "startSpeed", &MainModule::startSpeed
	  , "lifetime", &MainModule::lifetime
	  , "loop", &MainModule::loop
	  , "startSize", &MainModule::startSize
	  , "startAngle", &MainModule::startAngle
	  , "inheritVelocity", &MainModule::inheritVelocity
	  , "inheritVelocityModifier", &MainModule::inheritVelocityModifier
	  );

	// --------------------------------------------------------------------------
	// Particle System
	// --------------------------------------------------------------------------
  luaState.new_usertype<ParticleSystem>( "ParticleSystem"
	  , "Play", &ParticleSystem::Play
	  , "Pause", &ParticleSystem::Pause
	  , "Stop", &ParticleSystem::Stop
	  , "main", &ParticleSystem::main
	  , "emission", &ParticleSystem::emission
	  );

  // --------------------------------------------------------------------------
	// Particle System Modules
	// --------------------------------------------------------------------------
 
    
  


	// --------------------------------------------------------------------------
	// Time
	// --------------------------------------------------------------------------
	luaState.new_usertype<Time>("Time",
		"deltaTime", &Time::GetDeltaTime,
		"frameRate", sol::property(
			&Time::GetFrameRate,
			&Time::SetFrameRate)
		);
	
	// --------------------------------------------------------------------------
	// Input
	// --------------------------------------------------------------------------
	luaState.new_usertype<Input>("Input"
		, "GetKeyPress", &GetKeyPress
		, "GetKeyHeld", &GetKeyHeld
		, "GetKeyRelease", &GetKeyRelease
    , "GetCursorPosX", &GetCursorXPos
    , "GetCursorPosY", &GetCursorYPos
    , "GetGamepadAxis", &GetGamepadAxis
    , "GetButtonPress", &GetButtonPress
    , "GetButtonHeld", &GetButtonHeld
    , "GetButtonRelease", &GetButtonRelease
		);

  // --------------------------------------------------------------------------
	// Camera
	// --------------------------------------------------------------------------
	luaState.new_usertype<Camera>( "Camera"
		, sol::base_classes, sol::bases<Component>()
    , "position", &Camera::Position
    , "rotation", &Camera::Rotate
    , "zoom",     sol::property( 
      sol::resolve< const float() const >( &Camera::Zoom ),
      sol::resolve< void(float) >( &Camera::Zoom )) 
    );


	// --------------------------------------------------------------------------
	// Collision
	// --------------------------------------------------------------------------
	luaState.new_usertype<Collision>("Collision",
		"CollisionCallback", &Collision::CollisionCallbackLuaInterface
		);

	// --------------------------------------------------------------------------
	// Hitbox
	// --------------------------------------------------------------------------
  luaState.new_usertype<Hitbox>( "Hitbox"
	  , sol::base_classes, sol::bases<Component>()
	  // ,sol::constructors< Hitbox() >()
	  , "bounce", sol::property( 
		    &Hitbox::GetSpringTension,
		    &Hitbox::SetSpringTension),
	  "radius", &Hitbox::GetRadius
	  
	  );
	// --------------------------------------------------------------------------
	// Audio
	// --------------------------------------------------------------------------
  luaState.new_usertype<AudioSystem>("Audio",
	  "PlaySound", &AudioSystem::PlaySoundLua
	  );
}

// --------------------------------------------------------------------------
	// Wrapper Functions for lua
	// --------------------------------------------------------------------------

// Input Friendlies
bool GetKeyPress(std::string key)
{
	return IsPressed(magic_enum::enum_cast<Input::Key> (key).value());
}

bool GetKeyHeld(std::string key)
{
	return IsRepeated(magic_enum::enum_cast<Input::Key> (key).value());
}

bool GetKeyRelease(std::string key)
{
	return IsReleased(magic_enum::enum_cast<Input::Key> (key).value());
}

float GetGamepadAxis(const std::string& ID, const std::string& axis )
{
  return GetJoystickAxis( magic_enum::enum_cast<Input::Joystick>( ID ).value(),
	                        magic_enum::enum_cast<Input::JoystickAxis>(axis).value());
}

bool GetButtonPress(const std::string& ID, const std::string& button)
{
  return IsPressed( magic_enum::enum_cast<Input::Joystick>( ID ).value(),
	                    magic_enum::enum_cast<Input::JoystickButton>(button).value() );
}
bool GetButtonHeld(const std::string& ID, const std::string& button)
{
  return IsRepeated( magic_enum::enum_cast<Input::Joystick>( ID ).value(),
	                   magic_enum::enum_cast<Input::JoystickButton>(button).value() );
}
bool GetButtonRelease(const std::string& ID, const std::string& button)
{
  return IsReleased( magic_enum::enum_cast<Input::Joystick>( ID ).value(),
	                magic_enum::enum_cast<Input::JoystickButton>(button).value() );
}

// Scene Manager Friendlies
void LoadScene( const std::string& name, const std::string& mode )
{
  SceneManager::LoadScene( name, magic_enum::enum_cast<SceneManager::LoadMode>( mode ).value() );
}

void LoadScene( const int sceneIndex, const std::string& mode )
{
  SceneManager::LoadScene( sceneIndex, magic_enum::enum_cast<SceneManager::LoadMode>( mode ).value() );
}

// ParticleSystem Friendlies