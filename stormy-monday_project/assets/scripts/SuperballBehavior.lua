
--References
local GO
local RB

--Variables
local timeOnWall = 0.5
local onWallTimer = timeOnWall

local onWall = false

local collisionData

local reflection_velocity

--Function Forward Referencing


function Start()

	GO = gameobject

	RB = GameObject:GetRigidBody()

end

function OnCollisionEnter( col )

	collisionData = col

	otherObj = collisionData.GetOtherObject(GO.name)

	if otherObj.tag == "Environment" then

		onWall = true

		reflection_velocity = RB.velocity

	end

end

UpdateBounceBehavior = function()

	if onWall == true then

		RB.velocity = vec3.new(0.0, 0.0, 0.0)

		GO.transform.position = collisionData

		onWallTimer = onWallTimer - Time:deltaTime()

		if onWallTimer <= 0.0 then

			onWall = false

			RB.velocity = reflection_velocity

			onWallTimer = timeOnWall

		end

	end

end