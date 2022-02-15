/*!****************************************************************************
\file   LuaFunctionBinding.h
\author Kalby Jang
\brief  Binds class functions into a lua state
//
// Copyright 2020 DigiPen (USA) Corporation.
//
 *****************************************************************************/
#pragma once
#include <string>

class LuaFunctionBinding
{
public:

	static void BindFunctions(sol::state& luaState);

};

// Input Friendlies
bool GetKeyPress(std::string key);
bool GetKeyHeld(std::string key);
bool GetKeyRelease(std::string key);
float GetGamepadAxis(const std::string& ID, const std::string& axis );
bool GetButtonPress(const std::string& ID, const std::string& button);
bool GetButtonHeld(const std::string& ID, const std::string& button);
bool GetButtonRelease(const std::string& ID, const std::string& button);

// Scene Manager Friendlies
void LoadScene( const std::string& name, const std::string& mode = "Single" );
void LoadScene( const int sceneIndex, const std::string& mode = "Single" );