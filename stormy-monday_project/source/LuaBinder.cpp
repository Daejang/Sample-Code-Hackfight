/*!****************************************************************************
\file   LuaBinder.h
\author Kalby Jang
\brief  Holds the lua state variable.  Also manages, reloads, and checks scripts
//
// Copyright 2020 DigiPen (USA) Corporation.
//
 *****************************************************************************/
#include "LuaBinder.h"
#include "LuaBehave.h"
#include "LuaFunctionBinding.h"
#include <vector>
#include "TraceLog.h"

//-----------------------------------------------------------------------------
// Public Methods
//-----------------------------------------------------------------------------
// state LuaBinder::GetState()
// {
//   return luaState;  // TODO: fix this
// }
void LuaBinder::Start()
{
  lua->script("print('lua startup')");
  lua->script(R"(
  function copyTable(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[deepcopy(orig_key)] = deepcopy(orig_value)
        end
        setmetatable(copy, deepcopy(getmetatable(orig)))
    else -- number, string, boolean, etc
        copy = orig
    end
    return copy
  end
  )");


  SetPauseStatus(true);

}

void LuaBinder::Update( float dt )
{
  // TraceLog::Message("Lua Update");
  CheckAllScripts();
}

void LuaBinder::Shutdown()
{
  lua->script("print('lua shutdown')");
}


void LuaBinder::AddScriptRef( LuaBehavePtr luaBehave )
{
	luaBehaves.push_back(luaBehave);
}

void LuaBinder::RemoveScriptRef( LuaBehavePtr luaBehave )
{
  std::vector< LuaBehavePtr >::iterator result = std::find( luaBehaves.begin(), luaBehaves.end(), luaBehave );

  if (result == luaBehaves.end()) return;

  luaBehaves.erase( result );
}

void LuaBinder::ReloadAllScripts()
{
  allValidScriptSyntax = true;

	for (std::vector<LuaBehavePtr>::iterator it = luaBehaves.begin(); it != luaBehaves.end(); ++it)
		(*it)->ReloadScript();
}

bool LuaBinder::GetLuaScriptsRunning()
{
  return allValidScriptSyntax;
}

//-----------------------------------------------------------------------------
// Private Methods
//-----------------------------------------------------------------------------
LuaBinder::LuaBinder()
{
  lua = new sol::state;

  lua->open_libraries( sol::lib::base,
                       sol::lib::os, 
                       sol::lib::package,
                       sol::lib::table,
	                     sol::lib::math);


  LuaFunctionBinding::BindFunctions(*lua);
}

LuaBinder::~LuaBinder()
{
    // delete lua;
}

void LuaBinder::CheckAllScripts()
{
    for (std::vector<LuaBehavePtr>::iterator it = luaBehaves.begin(); it != luaBehaves.end(); ++it)
    {
		  if ( !(*it)->GetScriptSyntaxValidity() )
		  {
        allValidScriptSyntax = false;
        break;
		  }
    }    
}

// ----------------------------------------------------------------------------
// Private Methods
// ----------------------------------------------------------------------------
void LuaBinder::ClearScriptRefs()
{
	luaBehaves.clear();
}
