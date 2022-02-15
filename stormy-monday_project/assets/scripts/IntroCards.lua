
-- Logo Array
Logos = {
  LOGOS_FOLDER .. "DigipenLogo.png",
  LOGOS_FOLDER .. "Team_Logo_Touchup_1.png",
  LOGOS_FOLDER .. "FMOD.png",
  LOGOS_FOLDER .. "USE_IT.png",
  LOGOS_FOLDER .. "Title_Screen.png",
}

LogoTextures = {
  TEXTURES_DPLOGO,
  TEXTURES_TEAMLOGO,
  TEXTURES_FMODLOGO,
  TEXTURES_USEITLOGO,
  TEXTURES_TITLESCREENLOGO,
}

local cardTime = 2.0
local curCardTime = 0.0
local cardNum = 1

local curLogo = nil

-- References
local SR
local NextLevel


-- Function Forwarding

ChangeLogo = function( file )
  SR.texture = Texture.new( file )
end

ChangeTexture = function( inst )
  SR.texture = inst
end

-- Behavior Begins
function Start()

  -- Vars
  curCardTime = cardTime

  -- Refs
  SR        = gameobject:GetSprite()
  NextLevel = gameobject:GetScript("Level_Next")

  -- Inits
  gameobject.transform.scale = vec2.new(15.0)

  -- ChangeLogo( Logos[cardNum] )
  ChangeTexture( LogoTextures[cardNum] )



end

function Update()

  if Input.GetKeyPress("LeftClick") or
     Input.GetKeyPress("RightClick") or
     Input.GetKeyPress("Enter") or
     Input.GetKeyPress("Space") or
     Input.GetKeyPress("Escape") or
     Input.GetButtonPress("Joystick0", "A") or
     Input.GetButtonPress("Joystick0", "Menu")
     then
    NextLevel:Now()
    return
  end

  curCardTime = curCardTime - Time:deltaTime()

  if (curCardTime <= 0.0) then

    curCardTime = cardTime
    cardNum = cardNum + 1

    -- Change to next scene
    -- if  (cardNum > #Logos) then
    if  (cardNum > #LogoTextures) then
      -- next scene
      NextLevel:Now()
    else

    -- Display next logo
      -- curLogo = Logos[cardNum]
      -- curLogo()

      -- ChangeLogo( Logos[cardNum] )
      ChangeTexture( LogoTextures[cardNum] )

    end
  end
end

function Shutdown()

  SR:DeleteTexture()

end
