//-----------------------------------------------------------------------------
// File Name:    SceneManager.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once
#include "ISystem.h"
#include <string>
#include <vector>
#include <string>

#include "References.h"

//-----------------------------------------------------------------------------
// Forward References
//-----------------------------------------------------------------------------
typedef class Scene Scene, *ScenePtr;

//-----------------------------------------------------------------------------
// Scene Build Order
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Scene Manager Class
//-----------------------------------------------------------------------------
class SceneManager : public ISystem
{
public:

  enum LoadMode
  {
    Single,
    Additive
  };

  static SceneManager* Inst()
  {
    if (!inst_)
      inst_ = new SceneManager;

    return inst_;
  }

  // ISystem Methods
  void Start() override;
  void Update(float dt) override;  // Currently supports one active parentScene
  void Shutdown() override;

  // Class Methods
  // void LoadScene(ScenePtr scene);
  static void LoadScene( const std::string& name, LoadMode mode = Single );
  static void LoadScene( const int sceneIndex, LoadMode mode = Single );
  // void UnloadScene(ScenePtr scene);
  void AddSceneToManager(ScenePtr newScene);
  


  // Getters
  // bool ActiveSceneChanged();
  static ScenePtr GetActiveScene();
  static int GetSceneIndex( ScenePtr scene );
  static ScenePtr GetSceneByName( const std::string& name );
  static ScenePtr GetSceneAt( const int index);
  static int  GetSceneCount();

  // Setters
  static bool SetActiveScene(ScenePtr scene);
  

private:

  inline static SceneManager* inst_ = nullptr;

  SceneManager();
  ~SceneManager();

  const std::string scenesDir = References::FilePaths::TILED_FOLDER;  // Temp folder for scenes

  inline static ScenePtr activeScene_ = nullptr;
  inline static std::vector<ScenePtr> scenes;
  inline static std::vector<ScenePtr> loadedScenes;
  // std::vector<ScenePtr> unloadedScenes;
  inline static std::vector<ScenePtr>::iterator sceneUpdateItter;
  inline static std::vector<ScenePtr>::iterator sceneSearchItter;

  void AddAllScenes();
  void RemoveAllScenes();
  static void UnloadAllScenes();
  void SetInactiveScene(ScenePtr scene);

  void ChangeLevel();
  inline static ScenePtr nextScene = nullptr;
  inline static LoadMode loadMode  = Single;

  bool levelHasChanged = false;

  // ScenePtr FindUnloadedScene(const std::string& name);

  
};

