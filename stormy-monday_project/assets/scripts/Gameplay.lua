
-- Gameplay Vars


local curTimer

local startingTimer = 3.0
local endingTimer   = 3.0

respawnTime = 2.0

-- References
local GameplayStates
local curState

local LI  -- LevelInfo
local PM  -- Player Manager
local CAM
local Player
local PlayerClient
local PlayerMove

-- Function Forwarding Referencing
local CheckPlayersInBounds

local UpdateGameplay
local GameStarting
local GameOn
local GameFinished
local GamePaused

local EnterGameStart
local EnterGameOn
local EnterGameFinished
local EnterPause

-- Behavior Begins
function Start()

	Player = gameobject.FindGameObjectByName("The Player")

	if Player == nil then
		return
	end

	PlayerMove   = Player:GetScript("CharacterMovement")
	PlayerClient = Player:GetScript("Client")



	LI = gameobject:GetScript("LevelInfo")
	PM = gameobject:GetScript("PlayerManager")
	CAM = gameobject.FindGameObjectByName("Main Camera"):GetScript("FollowCamera")


	PlayerClient["playerNumber"] 	= 1
	PlayerClient["joystickID"] 		= "Joystick0"
	PlayerClient["characterNumber"] = 0



end



function Update()

	UpdateGameplay()

end

-- Gameplay Functions
UpdateGameplay = function()

	-- print("Gameplay Update")

	-- if curState then
	-- 	curState()
	-- else
	-- 	print("gameplay state has invalid state")
	-- end
	if Player ~= nil then
		GameOn()
	end

end

-- Gameplay Functions
CheckPlayersInBounds = function()

	local players = PM.playerGOs

	for p = 1, #players do

		local curPlayer = players[p]

		local inBounds = LI.levelBounds:Contains( curPlayer.transform.position )

		-- print("Player "..p.. "is bound:", inBounds)

		if not inBounds then

			curPlayer.transform.position = LI.respawnPoint

		end

	end
end

-- Gameplay States
GameStarting  = function ()


end

GameOn = function ()

	-- CheckPlayersInBounds()

	-- PM:UpdatePlayerManager()

	PlayerMove:UpdateMovement()

	CAM:UpdateCamera()

end

GameFinished = function ()

end

GamePaused = function ()

end

-- State Transitions
EnterNoGame = function ()

	curState = GameplayStates[1]
end

EnterGameStart = function ()

	curState = GameplayStates[2]

	curTimer = startingTimer

end

EnterGameOn = function ()

	curState = GameplayStates[3]


end

EnterGameFinished = function ()

	curState = GameplayStates[4]
end

EnterPause = function ()

	curState = GameplayStates[5]
end



-- Psuedo State Switch
GameplayStates = {
	[1] = NoGame,
	[2] = GameStarting,
	[3] = GameOn,
	[4] = GameFinished,
	[5] = GamePaused
}

curState = GameplayStates[3]
