/*!****************************************************************************
\file   LuaBinder.h
\author Kalby Jang
\brief  Holds the lua state variable.  Also manages, reloads, and checks scripts
//
// Copyright 2020 DigiPen (USA) Corporation.
//
 *****************************************************************************/
#pragma once
#include "ISystem.h"
#include "sol.hpp"

typedef class LuaBehave LuaBehave, *LuaBehavePtr;
class LuaFunctionBinding;

class LuaBinder : public ISystem
{
public:

  static LuaBinder* Inst()
  {
    // static LuaBinder _inst;
    // return &_inst;

    if (!inst_)
        inst_ = new LuaBinder;

    return inst_;
  }

  void Start() override;
  void Update(float dt) override;
  void Shutdown() override;

  // Static Methods
  static void AddScriptRef(LuaBehavePtr luaBehave);
  static void RemoveScriptRef(LuaBehavePtr luaBehave);
  static void ReloadAllScripts();
  static bool GetLuaScriptsRunning();

  // Static Variables
  inline static sol::state* lua;
  inline static std::vector<LuaBehavePtr> luaBehaves;

private:

  inline static LuaBinder* inst_ = nullptr;

  LuaBinder();
  ~LuaBinder();

  // Private Methods
  void CheckAllScripts();
  void ClearScriptRefs();

  // Static Private Variables
  inline static bool allValidScriptSyntax = true;
  
  
  
};

