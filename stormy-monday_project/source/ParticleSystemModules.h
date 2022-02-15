//-----------------------------------------------------------------------------
// File Name:    ParticleSystemModules.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------

#pragma once

#include "glm/glm.hpp"

class BurstModule
{
public:

  // BurstModule(float time_ = 0, size_t particleCount_ = 10 ) :
  //   time( time_ ), particleCount( particleCount_ )
  // {}

  float  time = 0;
  size_t particleCount = 10;

};

class EmissionModule
{
public:

  size_t numberOfBursts = 1;     // Returns the number of burst in the burst array
  float spawnRateByTime = 1.0;
  // float spawnRateOverTimeMultiplier = 1.0;
  float spreadAngle = 360.0f;

  // BurstModule burst = BurstModule();
  BurstModule burst;

};

// class ShapeModule
// {
// public:
//
//   
// };

class MainModule
{
public:

  float playDuration = 5.0f;
  bool  playOnStartUp = false;

  bool  useGravity = false;
  float gravityMultiplier = 1.0f;

  float startSpeed = 1.0;
  float lifetime = 1.0;
  bool  loop = false;

  glm::vec2 startSize = glm::vec2(1.0);

  float startAngle = 0.0f;  // Angle 0 starts on the Y-axis

  bool inheritVelocity = false;
  float inheritVelocityModifier = 1.0;

};


class ParticleSettings
{
public:

  MainModule main;
  EmissionModule emission;

};