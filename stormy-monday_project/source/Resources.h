//-----------------------------------------------------------------------------
// File Name:    Resources.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2019 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------

#pragma once
#include <memory>
#include "References.h"
#include "Texture.h"

class Resources
{
public:
  class Textures
  {
  public:
      inline static TexturePtr DPlogo; 
      inline static TexturePtr teamLogo; 
      inline static TexturePtr FMODlogo; 
      inline static TexturePtr USEITlogo; 
      inline static TexturePtr titleScreenLogo; 
      inline static TexturePtr finalScreenLogo; 

      static void Init()
      {
        DPlogo          = new Texture( References::FilePaths::LOGOS_FOLDER + "DigipenLogo.png" );
        teamLogo        = new Texture( References::FilePaths::LOGOS_FOLDER + "Team_Logo_Touchup_1.png" );
        FMODlogo        = new Texture( References::FilePaths::LOGOS_FOLDER + "FMOD.png" );
        USEITlogo       = new Texture( References::FilePaths::LOGOS_FOLDER + "USE_IT.png" );
        titleScreenLogo = new Texture( References::FilePaths::LOGOS_FOLDER + "Title_Screen.png" );
        finalScreenLogo = new Texture( References::FilePaths::LOGOS_FOLDER + "FINAL_SCREEN.png" );
      }

  };
};