//-----------------------------------------------------------------------------
// File Name:    GameObjectManager.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------

#include <iostream>
#include <vector>

#include "GameObject.h"
#include "GameObjectManager.h"


//GameObjectManager::_inst = nullptr;
//GameObjectManager* GameObjectManager::_inst = nullptr;

// Constructor & Destructor
GameObjectManager::GameObjectManager( ScenePtr parentScene ) : scene_(parentScene)
{
  gameObjectList = new std::vector<GameObjectPtr>;
  // gameObjectList = new std::list<GameObjectPtr>;
}

GameObjectManager::~GameObjectManager()
{
  ClearGameObjectList();

  delete gameObjectList;
  gameObjectList = nullptr;
}

// ISystem Functions
void GameObjectManager::Start()
{
  for (GOUpdateItter = gameObjectList->begin(); GOUpdateItter != gameObjectList->end(); GOUpdateItter++)
  {
    (*GOUpdateItter)->Start();
  }
}

void GameObjectManager::Update(float dt)
{
  for (GOUpdateItter = gameObjectList->begin(); GOUpdateItter != gameObjectList->end(); GOUpdateItter++)
  {
    if (!(*GOUpdateItter)->HasStarted())
    {
      (*GOUpdateItter)->Start();
    }

    (*GOUpdateItter)->Update(dt);
  }
}

void GameObjectManager::Shutdown()
{
  for (GOUpdateItter = gameObjectList->begin(); GOUpdateItter != gameObjectList->end(); GOUpdateItter++)
  {
    (*GOUpdateItter)->Shutdown();
  }
}


// Class Methods
void GameObjectManager::Add(GameObjectPtr gameObject)
{
  if (!gameObject) return;

  gameObjectList->push_back(gameObject);
  gameObject->parentScene = GetScene();
  
}

GameObjectPtr GameObjectManager::FindByName(const std::string& name)
{
  for (GOSearchItter = gameObjectList->begin(); GOSearchItter != gameObjectList->end(); GOSearchItter++)
  {
    if ((*GOSearchItter)->Name() == name)
    {
      if (!(*GOSearchItter)->IsDestroyed())
        return *GOSearchItter;
    }
  }

  return NULL;
}

std::vector<GameObjectPtr> GameObjectManager::FindGameObjectsByTag( const std::string& tag)
{
  std::vector<GameObjectPtr> foundTags;

  for (GOSearchItter = gameObjectList->begin(); GOSearchItter != gameObjectList->end(); GOSearchItter++)
  {
    if ((*GOSearchItter)->Tag() == tag)
    {
      if (!(*GOSearchItter)->IsDestroyed())
        foundTags.push_back(*GOSearchItter);
    }
  }

  return foundTags;
}


bool GameObjectManager::Remove(std::string name)
{
  for (GOSearchItter = gameObjectList->begin(); GOSearchItter != gameObjectList->end(); GOSearchItter++)
  {
    if ((*GOSearchItter)->Name() == name)
    {
      (*GOSearchItter)->Shutdown();

      // May need switching
      delete *GOSearchItter;

      gameObjectList->erase(GOSearchItter);

      return true;
    }
  }

  return false;
}


void GameObjectManager::ClearGameObjectList()
{
  for (GOUpdateItter = gameObjectList->begin(); GOUpdateItter != gameObjectList->end(); GOUpdateItter++)
  {
    // (*GOUpdateItter)->Shutdown();
    delete *GOUpdateItter;
  }

  // Clear the gameObjectList
  gameObjectList->clear();

  
}




std::vector<GameObjectPtr>* GameObjectManager::GetGameObjectList()
{
  return gameObjectList;
}

ScenePtr GameObjectManager::GetScene() const
{
  return scene_;
}

// std::list<GameObjectPtr>* GameObjectManager::GetGameObjectList()
// {
//   return gameObjectList;
// }

// void GameObjectManager::DeleteAllGameObjects()
// {
//
// }


