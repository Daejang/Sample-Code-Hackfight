/*!****************************************************************************
\file   LuaBehave.cpp
\author Kalby Jang
\brief  Handles and executes lua scripts
//
// Copyright 2020 DigiPen (USA) Corporation.
//
 *****************************************************************************/
#include "LuaBehave.h"
#include "LuaBinder.h"
// #include "assert.h"

#include "GameObject.h"
#include "Collider.h"
#include "CollisionData.h"


LuaBehave::LuaBehave(const std::string& newScriptFile) : Component(cLuaBehave), scriptInst(*LuaBinder::lua, sol::create, LuaBinder::lua->globals())
{
  scriptName = newScriptFile;
  scriptFullPath = scriptsFolder + scriptName;

  // Loading script
  // script = LuaBinder::lua.load_file(scriptFullPath);  // Load_file is deprecated

  LoadScript();

  // File error check
  ValidateScript();

  // LoadData();

  // Called on loading script instance.  Advance Feature
  // LuaCall("Awake");
  //

  // Test area
  LuaBinder::AddScriptRef(this);
 
}

LuaBehave::LuaBehave(const LuaBehave& other) : Component(other)
{
  scriptInst = (*LuaBinder::lua)["copyTable"](other.scriptInst);
  // *this = other;  // Doesn't work for some reason

  // script         = other.script;
  scriptName     = other.scriptName;
  scriptFullPath = other.scriptFullPath;

  LoadScript();

  LuaBinder::AddScriptRef(this);
}

ComponentPtr LuaBehave::Clone() const
{
  return new LuaBehave(*this);
}

LuaBehave::~LuaBehave()
{
  scriptInst["gameobject"] = sol::nil;
  scriptInst = sol::nil;

  LuaBinder::RemoveScriptRef( this );

}


//-----------------------------------------------------------------------------
// Inherited Methods
//-----------------------------------------------------------------------------
void LuaBehave::Start()
{
  LoadData();

  // Gets called once in the script's lifetime
  LuaCall("Start");
}

void LuaBehave::Update( float dt )
{
  LuaCall("Update");
}

void LuaBehave::Shutdown()
{
  LuaCall("Shutdown");
}

// sol::environment& LuaBehave::GetScript( const std::string& scriptName )
// {
	// return *Parent()->GetScript(scriptName);
// }

void LuaBehave::OnTriggerEnter( ColliderDataPtr otherGuy )
{
  // tempColliderData = otherGuy;

  LuaCall("OnTriggerEnter", otherGuy);
}

void LuaBehave::OnTriggerExit(ColliderDataPtr otherGuy)
{
    LuaCall("OnTriggerExit", otherGuy);
}

//
void LuaBehave::OnCollisionEnter( CollisionDataPtr otherGuy )
{
  // tempCollisionData = otherGuy;

  LuaCall("OnCollisionEnter", otherGuy);
}

void LuaBehave::LoadScript()
{
  try
  {
    scriptLoadResult = LuaBinder::lua->script_file(scriptFullPath, scriptInst);
    validScriptSyntax = scriptLoadResult.valid();

    if (!validScriptSyntax)
    {
      sol::error err = scriptLoadResult;
		  std::cout << "Load Error: " << err.what() << std::endl;
    }

  }
  catch (sol::error& e)
  {
    std::cerr << "Load Script Error: " << e.what() << std::endl;
  }

  
  // validScriptSyntax = true;
}

void LuaBehave::ReloadScript()
{
	LoadScript();
	LuaCall("Start");

	LoadData();
}


//-----------------------------------------------------------------------------
// Private Methods
//-----------------------------------------------------------------------------
const std::string& LuaBehave::ScriptName() const
{
  return scriptName;
}

const std::string& LuaBehave::ScriptPath() const
{
  return scriptFullPath;
}

const std::string LuaBehave::ScriptNameNoExt() const
{
	size_t pos;
	std::string delimiter = ".";
	std::string token;

	pos = scriptName.find(delimiter);
	token = scriptName.substr(0, pos);


	return token;
}

sol::table LuaBehave::GetScriptInst()
{
	return scriptInst;
}

bool LuaBehave::GetScriptSyntaxValidity() const
{
  return validScriptSyntax;
}

void LuaBehave::ResetScriptValidity()
{
  validScriptSyntax = true;
}

void LuaBehave::ScriptName( const std::string& newScript )
{
  scriptName     = newScript;
  scriptFullPath = scriptsFolder + scriptName;
}

//-----------------------------------------------------------------------------
// Private Methods
//-----------------------------------------------------------------------------

void LuaBehave::LuaCall(const std::string& funcName)
{
  if ( !LuaBinder::GetLuaScriptsRunning() || !validScriptSyntax ) return;

  // Check if Function exists
  sol::object funcExists = scriptInst[funcName];

  if (funcExists == sol::nil) return;

  // Starting Error Handler
  #ifdef _DEBUG
	  sol::protected_function func = funcExists;
  #else
	  sol::function func = funcExists;
  #endif
  

  // Error checking
  try
  {
    // sol::safe_function func = LuaBinder::lua[funcName]();
	  // scriptInst[funcName]();

    // Executing script
	  auto execFunc = func();

    validScriptSyntax = execFunc.valid();

    // See if there is syntax errors
    if (!validScriptSyntax)
    {
		  sol::error err = execFunc;
		  std::cout << "Runtime Error: " << err.what() << std::endl;
    }

	  // if (func.valid())
		 //  func();
	  
  }
  catch (sol::error& e)
  {
	  std::cout << "Function Call " << e.what() << std::endl;
	  __debugbreak();  
  }
}


void LuaBehave::LoadData()
{
  try
  {
    scriptInst["gameobject"] = Parent();
	  // scriptInst["script"] = this;
  }
  catch(sol::error e)
  {
    std::cerr << "Load Data: " << e.what() << std::endl;
  }
}


bool LuaBehave::ValidateScript()
{
  validScriptFile = scriptLoadResult.valid();
  
  if (validScriptFile) return true;
  
  // sol::error err = scriptLoadResult;  // This works somehow
  // std::cerr << scriptName << " has errors: " << std::endl
  //           << err.what() << std::endl;
  return false;

  // luaScriptIsClean = scriptLoadResult.valid();
  //
  // if (!luaScriptIsClean)
  // {
  //   sol::error err = scriptLoadResult;
  //
  // }
  //
  // return luaScriptIsClean;
}

bool LuaBehave::CheckScriptChange()
{
  return false;
}

// void LuaBehave::PhysicsCallbacks()
// {
//   if (tempColliderData)
//   {
//     // LuaCall("OnTriggerEnter", tempColliderData);
//     tempCollisionData = nullptr;
//   }
//
//   if (tempCollisionData)
//   {
//     // LuaCall("OnCollisionEnter", tempCollisionData);
//     tempCollisionData = nullptr;
//   }
//
// }







