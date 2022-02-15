//-----------------------------------------------------------------------------
// File Name:    Scene.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once

#include "GameObjectManager.h"

//-----------------------------------------------------------------------------
// Forward References
//-----------------------------------------------------------------------------
typedef class Scene Scene, *ScenePtr;

//-----------------------------------------------------------------------------
// Scene Class
//-----------------------------------------------------------------------------
class Scene
{
public:

  Scene(std::string name);
  ~Scene();
 
  // Class Methods
  void Start();
  void Update(float dt);
  void Shutdown();

  // Methods
  void LoadGameObjects();
  void UnloadGameObjects();

  // Getters
  std::string& Name();
  bool IsActive();
  bool IsLoaded();

  GameObjectManagerPtr GetGameObjectManager();
  std::vector<GameObjectPtr>* GetGameObjects();
  // std::list<GameObjectPtr>* GetGameObjects();

  // Setters
  void Name(std::string& name);
  void SetActive(bool active);
  void SetLoaded(bool loaded);


private:
  std::string name_ = "";
  bool active_ = false;
  bool loaded_ = false;
  GameObjectManagerPtr gameObjectManager_;

  std::string sceneFile_ = "";

  // Test Stuff
  GameObjectPtr mainCamera_;

};


