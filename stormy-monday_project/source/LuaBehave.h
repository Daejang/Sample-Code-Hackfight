/*!****************************************************************************
\file   LuaBehave.h
\author Kalby Jang
\brief  Handles and executes lua scripts
//
// Copyright 2020 DigiPen (USA) Corporation.
//
 *****************************************************************************/

#pragma once

#include "Behavior.h"
#include "sol.hpp"
#include <string>
#include "References.h"

// #include "Collision.h" // No, Bad

// using namespace sol;

//class RigidBody;
typedef class CollisionData CollisionData, *CollisionDataPtr;
typedef class ColliderData ColliderData, *ColliderDataPtr;


enum LuaBehaveStates
{
  Invalid,
  Loading,
  Start,
  Update,
};



class LuaBehave : public Component
{

public:

  LuaBehave(const std::string& newScriptFile = "");
  LuaBehave(const LuaBehave& other);

  // LuaBehave& operator=(const LuaBehave&) = delete; //?

  ComponentPtr Clone() const;
  ~LuaBehave();

  // Inherited Methods
  void Start() override;
  void Update( float dt ) override;
  void Shutdown() override;

  // Methods
  void LoadScript();
  void ReloadScript();

  // sol::environment& GetScript(const std::string& scriptName);

  // Callback methods
  void OnTriggerEnter( ColliderDataPtr otherGuy );
  void OnTriggerExit(ColliderDataPtr otherGuy);
  // // void OnTriggerExit(CollisionDataPtr otherGuy);
  void OnCollisionEnter( CollisionDataPtr otherGuy);  // Need to move collisionData to it's own .h
  // // void OnCollisionExit(CollisionDataPtr otherGuy);
  

  // Getters
  const std::string& ScriptName() const;
  const std::string& ScriptPath() const;
  const std::string ScriptNameNoExt() const;
  sol::table GetScriptInst();

  bool GetScriptSyntaxValidity() const;
  void ResetScriptValidity();

  // Setters
  void ScriptName(const std::string& newScript);


private:

  // state luaState;
  // sol::load_result script;  // Instance of script
  sol::protected_function_result scriptLoadResult;
  sol::environment scriptInst;  // Keeps instance of script

  bool validScriptFile = false;
  bool validScriptSyntax = false;


  const std::string scriptsFolder = References::FilePaths::SCRIPTS_FOLDER;
  std::string scriptName;
  std::string scriptFullPath;

  // Test vars
  // ColliderData testCol;
  // ColliderData tempColliderData;
  // CollisionDataPtr tempCollisionData;
  

  // Private Methods
  void LoadData();
  bool ValidateScript();
  bool CheckScriptChange();

  // void PhysicsCallbacks();

  void LuaCall(const std::string& funcName);
  // void LuaCall(const std::string& funcName, const std::string& saySomething);
  template <typename T>
  T LuaCall(const std::string& funcName);
  template <typename T>
  void LuaCall(const std::string& funcName, T arg);
 

};

//-----------------------------------------------------------------------------
// Private Template Methods
//-----------------------------------------------------------------------------
template <typename T>
T LuaBehave::LuaCall(const std::string& funcName)
{
  if (!validScriptFile) return false;

  try
  {
    return scriptInst[funcName]();
  }
  catch (sol::error e)
  {
    std::cout << "Function Call " << e.what() << std::endl;
  }
}

template <typename T>
void LuaBehave::LuaCall( const std::string& funcName, T arg )
{
  if (!validScriptFile) return;

  // Check if Function exists
  sol::object funcExists = scriptInst[funcName];

  if (funcExists == sol::nil) return;

  // Starting Error Handler
  sol::protected_function func = funcExists;

  // Error checking
  try
  {
    // scriptInst[funcName](arg);

	// sol::protected_function func = scriptInst[funcName];

	// if (func.valid())
	// 	func(arg);
	
	   // Executing script
	  auto execFunc = func(arg);

	  // See if there is syntax errors
	  if (!execFunc.valid())
	  {
		  sol::error err = execFunc;
		  std::cout << "func call: " << err.what() << std::endl;
	  }

	
 }
  catch (sol::error e)
  {
    std::cout << "Function Call " << e.what() << std::endl;
	  __debugbreak();

  }
}


