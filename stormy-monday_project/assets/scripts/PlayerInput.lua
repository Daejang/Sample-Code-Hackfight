-- References
client = nil

-- Input Vars
id 			= nil
inputDirVec = vec2.new(0.0)
aimDirVec   = vec2.new(0.0)
jump 		= false
grapple		= false
attack		= false
anchorTele  = false
testLaunch	= false
resetPos  	= false


function bool_to_number(value)
  return value and 1 or 0
end


function UpdateInput()

	inputDirVec = vec3.new(0.0)

	if client.playerNumber == 1 then
		-- Keyboard
		inputDirVec.x = bool_to_number( Input.GetKeyHeld("D") ) - bool_to_number( Input.GetKeyHeld("A") )

		aimDirVec.x   = bool_to_number( Input.GetKeyHeld("D") ) - bool_to_number( Input.GetKeyHeld("A") )
		aimDirVec.y   = bool_to_number( Input.GetKeyHeld("W") ) - bool_to_number( Input.GetKeyHeld("S") )
		-- aimDirVec = vec2.Normalize(aimDirVec)

		jump 	   = Input.GetKeyPress("Space")
		-- grapple    = Input.GetKeyPress("Q")
		attack     = Input.GetKeyPress("Space")
		anchorTele = Input.GetKeyPress("E")
		cancel = Input.GetKeyPress("LeftShift")


		-- Gamepad
		inputDirVec.x = inputDirVec.x + Input.GetGamepadAxis(id, "LeftStickX")

		aimDirVec.x   = aimDirVec.x + Input.GetGamepadAxis(id, "LeftStickX")
		aimDirVec.y   = aimDirVec.y - Input.GetGamepadAxis(id, "LeftStickY")

		aimDirVec = vec2.Normalize(aimDirVec)

		jump 	   = jump       or Input.GetButtonPress(id, "A")
		-- grapple  = grapple  or Input.GetButtonPress(id, "RightBumper")
		attack     = attack     or Input.GetButtonPress(id, "A")
		anchorTele = anchorTele or Input.GetButtonPress(id, "Y")
		cancel     = cancel 	or Input.GetButtonPress(id, "B")



	else

	-- Joystick Controls

		inputDirVec.x = Input.GetGamepadAxis(id, "LeftStickX")

		aimDirVec.x   = Input.GetGamepadAxis(id, "LeftStickX")
		aimDirVec.y   = Input.GetGamepadAxis(id, "LeftStickY")
		aimDirVec.y = -aimDirVec.y
		aimDirVec = vec2.Normalize(aimDirVec)

		jump 	   = Input.GetButtonPress(id, "A")
		-- grapple  = Input.GetButtonPress(id, "RightBumper")
		attack     = Input.GetButtonPress(id, "A")
		anchorTele = Input.GetButtonPress(id, "Y")
		cancel     = Input.GetButtonPress(id, "B")


	end

	resetPos   = Input.GetKeyHeld ("G") or Input.GetButtonPress(id, "Y")

end

function Start()

	client = gameobject:GetScript("Client")
	id = client["joystickID"]


end

function Update()

	-- print("Input Update")
	UpdateInput()

end
