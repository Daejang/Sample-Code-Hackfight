
Now = nil

local scene = nil
local sceneIndex = nil

function Start()

	scene = SceneManager:GetActiveScene()
	sceneIndex = SceneManager.GetSceneIndex( scene )

	print("Scene Index Type: ".. type(sceneIndex))

end

function OnTriggerEnter( collider )

	print("Level Changer triggered")

	if collider.gameobject.tag == "Player" then

		print( "Changing to scene ".. sceneIndex)
		Now()
	end
end

Now = function()

	SceneManager.LoadScene( sceneIndex + 1, "Single" )

end
