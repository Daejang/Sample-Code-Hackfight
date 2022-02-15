//-----------------------------------------------------------------------------
// File Name:    SceneManager.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#include "SceneManager.h"
#include "Audio.h"
#include "References.h"
#include "Scene.h"
#include "TraceLog.h"
#include "PhysicsSystem.h"
#include "PauseMenu.h"

void SceneManager::Start()
{
  // LoadAllScenes();
    SetPauseStatus(true);
  // Load default scene
  LoadScene(0);
  ChangeLevel();

  // Starting parentScene sequence
  // for (sceneUpdateItter = scenes.begin(); sceneUpdateItter != scenes.end(); sceneUpdateItter++)
  // {
  //   (*sceneUpdateItter)->Start();
  // }
}

void SceneManager::Update( float dt )
{
    if (levelHasChanged == true)
    {
        levelHasChanged = false;
    }

    if (nextScene)
    {
        AudioSystem::Inst()->PlaySound(AudioSystem::BACKGROUND, AudioSystem::START, 0.1f);
    }

  // Change level
  ChangeLevel();

  if (levelHasChanged == true && activeScene_ != scenes[0])
  {
      PauseMenu::Inst()->Shutdown();

      if (scenes.empty() == false && activeScene_ == scenes[scenes.size() - 1])
      {
          PauseMenu::Inst()->EndOfGameBegin();
      }

      PauseMenu::Inst()->IntroScreenOver();

      PauseMenu::Inst()->Start();
  }

  // Update level
  for (sceneUpdateItter = scenes.begin(); sceneUpdateItter != scenes.end(); sceneUpdateItter++)
  {
    // if ((*sceneUpdateItter) == activeScene)
    if ((*sceneUpdateItter)->IsLoaded())
      (*sceneUpdateItter)->Update(dt);
  }
}

void SceneManager::Shutdown()
{
  for (sceneUpdateItter = scenes.begin(); sceneUpdateItter != scenes.end(); sceneUpdateItter++)
  {
    (*sceneUpdateItter)->Shutdown();
    delete *sceneUpdateItter;
  }

  scenes.clear();
}


//todo:: update this function with overload
void SceneManager::LoadScene( const std::string& name, LoadMode mode )
{
  nextScene = GetSceneByName(name);

  if (!nextScene) return;

  loadMode = mode;
 
}

void SceneManager::LoadScene( const int sceneIndex, LoadMode mode )
{
  if (sceneIndex < 0 || sceneIndex >= scenes.size())
  {
    // Error Message "Bad Index"
    TraceLog::Message("Loading scene with bad index");
    return;
  }

  nextScene = scenes[sceneIndex];
  loadMode = mode;

}


void SceneManager::AddSceneToManager( ScenePtr newScene )
{
  scenes.push_back(newScene);
}

void SceneManager::RemoveAllScenes()
{
  scenes.clear();
}


// Getters --------------------------------------------------------------------
ScenePtr SceneManager::GetActiveScene()
{
  return activeScene_;
}

int SceneManager::GetSceneIndex( ScenePtr scene )
{
    std::vector<ScenePtr>::iterator it = std::find(scenes.begin(), scenes.end(), scene);

    // Exit if scene not found
    if (it == scenes.end())
        return -1;

    return std::distance( scenes.begin(), it );
}

ScenePtr SceneManager::GetSceneByName( const std::string& name )
{
  for (sceneSearchItter = scenes.begin(); sceneSearchItter != scenes.end(); ++sceneSearchItter)
  {
    if ((*sceneSearchItter)->Name() == name)
    {
      return *sceneSearchItter;
    }
  }

  return nullptr;
}

ScenePtr SceneManager::GetSceneAt( const int index )
{
  return scenes.at( index );
}

int SceneManager::GetSceneCount()
{
  return scenes.size();
}

// Setters --------------------------------------------------------------------
bool SceneManager::SetActiveScene( ScenePtr scene )
{
  scene->SetActive(true);
  activeScene_ = scene;

  return true;
}

// ----------------------------------------------------------------------------
// Private Constructors
// ----------------------------------------------------------------------------
SceneManager::SceneManager()
{
  AddAllScenes();
}

SceneManager::~SceneManager()
{
  RemoveAllScenes();
}

// ----------------------------------------------------------------------------
// Private Methods
// ----------------------------------------------------------------------------
void SceneManager::AddAllScenes()
{
  // Reference all scenes into the scene pool

  AddSceneToManager( new Scene( "Title Cards.json" ) );
  AddSceneToManager( new Scene( "tutorial_map.json") );
  AddSceneToManager( new Scene( "Level_2.json") );
  AddSceneToManager( new Scene( "Level_3.json") );
  AddSceneToManager( new Scene( "Level_4.json") );

  AddSceneToManager( new Scene( "Ending Cards.json") );
  
  //AddSceneToManager(new Scene("test-map.json"));





}

void SceneManager::UnloadAllScenes()
{
  for (sceneSearchItter = scenes.begin(); sceneSearchItter != scenes.end(); ++sceneSearchItter)
  {
    if ((*sceneSearchItter)->IsLoaded())
    {
      (*sceneSearchItter)->SetLoaded( false );
      (*sceneSearchItter)->SetActive( false );
      (*sceneSearchItter)->UnloadGameObjects();
    }

    // if ((*sceneSearchItter)->IsActive())
    // {
    //   (*sceneSearchItter)->SetActive( false );
    // }
  }

  activeScene_ = nullptr;
}

void SceneManager::SetInactiveScene( ScenePtr scene )
{
  scene->SetActive(false);
  activeScene_ = nullptr;
}

void SceneManager::ChangeLevel()
{
  // Exit if scenes are not changing  
  if (!nextScene) return;

  // Unload Previous Scene
  if (loadMode == Single)
    UnloadAllScenes();

  //Physics::TempShutdown();

  // Load Next Scene
  // nextScene = scenes[sceneIndex];
  nextScene->SetLoaded(true);

  if (!activeScene_ || loadMode == Single)
    SetActiveScene(nextScene);

  // Load Gameobjects
  nextScene->LoadGameObjects();

  //AudioSystem::Inst()->PlaySound(AudioSystem::BACKGROUND, AudioSystem::START, 0.1f);

  nextScene->Start();

  // TraceLog::Message( nextScene->Name().c_str(), " has loaded" );
  std::cout << nextScene->Name() << " has loaded." << std::endl;

  //Physics::Inst()->Start();

  // Reset Vars
  nextScene = nullptr;

  levelHasChanged = true;
}

// ScenePtr SceneManager::FindUnloadedScene( const std::string& name )
// {
//   for (sceneSearchItter = scenes.begin(); sceneSearchItter != scenes.end(); sceneSearchItter++)
//   {
//     if ((*sceneSearchItter)->Name().compare(name) == 0)
//     {
//       return *sceneSearchItter;
//     }
//   }
//
//   return nullptr;
// }
