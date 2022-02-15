
-- Camera Vars


-- Refernces
local cam    = nil
local player = nil

-- Function Forwarding Referencing
UpdateCamera = nil

-- Helper Functions

-- Behavior Begins
function Start()

	cam    = gameobject:GetCamera()
	local players = gameobject.FindGameObjectsByTag("Player")


	print("Player Count: ", #players)
	print("CAM is: ", cam)

	if (#players > 0) then
		player = players[1]
	end
end

UpdateCamera = function()

	if player ~= nil then
		cam:position( player.transform.position )
	end
end
