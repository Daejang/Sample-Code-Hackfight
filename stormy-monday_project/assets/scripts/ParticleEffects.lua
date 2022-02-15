
-- Variables
local pss = {}

-- Function References
local Setup

function Start()

  Setup()

end

Setup = function()

  local curpss
  pss = gameobject:GetParticleSystems()

  if #pss ~= 3 then

    print("There needs to be 3 particle systems")
    return
  end

  -- Default particle values
  -- Particles.main.lifetime = 0.3
  -- Particles.main.useGravity = false
  -- Particles.main.startSpeed = 10.0
  -- Particles.main.startSize  = vec2.new(0.5)
  -- Particles.inheritVelocity = false

  -- Launch Effect --
  -- Initial Cannon Blast
  curpss = pss[1]

  curpss.main.lifetime = 0.2
  curpss.main.startSpeed = 20.0
  curpss.main.startSize  = vec2.new(2.0)
  curpss.emission.burst.particleCount = 50


  -- Nozzle Blast Direction
  curpss = pss[2]

  curpss.main.lifetime = 0.3
  curpss.main.startSpeed = 20.0
  curpss.main.startSize = vec2.new( 1.0 )
  -- curpss.main.startAngle =
  curpss.emission.spreadAngle = 15.0

  -- Flight trail
  curpss = pss[3]

  curpss.main.loop = true
  curpss.main.startSize = vec2.new( 0.5 )
  curpss.emission.spawnRateByTime = 50.0
  curpss.emission.spreadAngle = 50.0
  curpss.emission.burst.particleCount = 0;

end


function PlayCannonBurst()
  pss[1]:Play()
end

function StopCannonBurst()
  pss[1]:Stop()
end

function PlayNozzleBurst( degrees )
  pss[2].main.startAngle = degrees
  pss[2]:Play()
end

function StopNozzleBurst()
  pss[2]:Stop()
end

function PlayFlightTrail()
  pss[3]:Play()
end

function StopFlightTrail()
  pss[3]:Stop(false)
end
