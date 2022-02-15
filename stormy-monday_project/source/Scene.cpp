//-----------------------------------------------------------------------------
// File Name:    Scene.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#include <string>
#include <vector>

#include "Audio.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "GameObjectFactory.h"


// Scene::Scene(std::string name) : name_(name)
// {
//   gameObjectManager_ = new GameObjectManager( this );
// }

Scene::Scene(std::string name) : sceneFile_(name)
{
  gameObjectManager_ = new GameObjectManager( this );

  name_ = name;

}

Scene::~Scene()
{
  delete gameObjectManager_;
  gameObjectManager_ = nullptr;
}

// Class Methods --------------------------------------------------------------
void Scene::Start()
{ 
  // LoadGameObjects();

  // Calling start 
  gameObjectManager_->Start();
}

void Scene::Update( float dt )
{
  // Calling update
  gameObjectManager_->Update(dt);
}

void Scene::Shutdown()
{
  // Unloading the scene
  gameObjectManager_->Shutdown();

  UnloadGameObjects();
}

void Scene::LoadGameObjects()
{
      // Adding free camera with every scene
  // TODO: Load Gameobjects here
  mainCamera_ = new GameObject("Main Camera");
  mainCamera_->Tag("Camera");
  mainCamera_->AddComponent( new Camera(20.0f, vec2(16, 9)) );
  // _mainCamera->AddComponent( new LuaBehave("ActionCamera.lua") );
  mainCamera_->AddComponent( new LuaBehave("FollowCamera.lua") );
  mainCamera_->parentScene = this;

  gameObjectManager_->Add( mainCamera_ );

  // Loading from file
  GameObjectFactory::Inst()->LoadGameobjects( sceneFile_, gameObjectManager_ );

}

void Scene::UnloadGameObjects()
{
  gameObjectManager_->ClearGameObjectList();
}

// Getters --------------------------------------------------------------------
std::string& Scene::Name()
{
  return name_; 
}

bool Scene::IsActive()
{
  return active_;
}

bool Scene::IsLoaded()
{
  return loaded_;
}

GameObjectManagerPtr Scene::GetGameObjectManager()
{
  return gameObjectManager_;
}


std::vector<GameObjectPtr>* Scene::GetGameObjects()
{
  return gameObjectManager_->GetGameObjectList();
}

// std::list<GameObjectPtr>* Scene::GetGameObjects()
// {
//   return _gameObjectManager->GetGameObjectList();
// }

// Setters --------------------------------------------------------------------
void Scene::Name(std::string& name)
{
  name_ = name;
}
void Scene::SetActive(bool active)
{
  active_ = active;
}

void Scene::SetLoaded( bool loaded )
{
  loaded_ = loaded;
}
