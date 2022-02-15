//-----------------------------------------------------------------------------
// File Name:    GameObjectManager.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  The idea of this class is that it should only manage existing gameObjects
//    and not be touching by anyone outside.
//-----------------------------------------------------------------------------

#pragma once


//#include "ISystem.h"
#include <string>
#include <list>
//using namespace std;

//-----------------------------------------------------------------------------
// Forward References
//-----------------------------------------------------------------------------
typedef class GameObjectManager GameObjectManager, *GameObjectManagerPtr;
typedef class GameObject GameObject, *GameObjectPtr;
typedef class Scene Scene, *ScenePtr;

//-----------------------------------------------------------------------------
// GameObject Class
//-----------------------------------------------------------------------------
class GameObjectManager
{
public:

//  static GameObjectManager* Inst()
//  {
//    static GameObjectManager _inst;
//
////    if (!_inst)
////      _inst = new GameObjectManager();
//
//    return &_inst;
////    return _inst;
//  }

  GameObjectManager( ScenePtr parentScene = nullptr );
  ~GameObjectManager();

  // ISystem Functions
  void Start();
  void Update(float dt);
  void Shutdown();

  //Public Methods
  void Add(GameObjectPtr gameObject);
  GameObjectPtr FindByName(const std::string& name);
  std::vector<GameObjectPtr> FindGameObjectsByTag( const std::string& tag);
  bool Remove(std::string name);  // Change to specific gameobject instance
  void ClearGameObjectList();

  // Getters
  std::vector<GameObjectPtr>* GetGameObjectList();
  // std::list<GameObjectPtr>* GetGameObjectList();
  ScenePtr GetScene() const;


private:

  // Private Variables
//  static GameObjectManager* _inst;

  ScenePtr scene_ = nullptr;

  std::vector<GameObjectPtr>* gameObjectList;
  std::vector<GameObjectPtr>::iterator GOUpdateItter;
  std::vector<GameObjectPtr>::iterator GOSearchItter;

  // std::list<GameObjectPtr>* gameObjectList;
  // std::list<GameObjectPtr>::iterator GOUpdateItter;
  // std::list<GameObjectPtr>::iterator GOSearchItter;

  // Private Methods
  // void DeleteAllGameObjects();
};
