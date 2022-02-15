//-----------------------------------------------------------------------------
// File Name:    References.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once
#include <string>

class References
{
public:
  class FilePaths
  {
  public:
	 //inline static const std::string  = R"()"; 
	  inline static const std::string NO_FOLDER				  = "";
	  inline static const std::string ASSETS_FOLDER		  = R"(.\assets\)";
	  inline static const std::string DATA_FOLDER			  = R"(.\data\)";
	  inline static const std::string SOUNDS_FOLDER		  = R"(.\data\sounds\)";
	  inline static const std::string TEXTURES_FOLDER	  = R"(textures\)";
	  inline static const std::string SCENE_FOLDER		  = ASSETS_FOLDER + R"(scenes\)";
	  inline static const std::string SCRIPTS_FOLDER	  = ASSETS_FOLDER + R"(scripts\)";
	  inline static const std::string TILED_FOLDER			= DATA_FOLDER + R"(in\tiled\)";
	  inline static const std::string TILESET_FOLDER		= TEXTURES_FOLDER + R"(ts\)";
	  inline static const std::string LOGOS_FOLDER		  = TEXTURES_FOLDER + R"(Logos\)";

  };

  class GameInfo
  {
  public:
	  inline static const std::string GAME_NAME = "Hackfight";
  };
};
