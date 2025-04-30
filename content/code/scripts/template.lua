-- Copyright 2025 DigiPen (USA) Corporation
-- File Author(s): Grace Biggs
-- Any and all C++ function calls or variable references MUST be exposed in LuaEngineBindings/LuaExternalBindings!
-- If your script breaks for no apparent reason, you're probably invoking an invalid function! Check LuaEngineBindings/LuaExternalBindings.cpp to see if your function call is bound correctly!
-- Also check out those files for Math helper functions that you can access!

-- Globals go here (make sure you're not initializing them with variables/functions that may not be ready until after everything is deserialized!)

-- ScriptComponent expects Init(), Update(deltaTime), and Shutdown() to be present in all scripts. 
function Init()
	print("LUA: template:Init() called!")
end

function Update(deltaTime)
	print("LUA: template:Update() called!")
end

function Shutdown()
	print("LUA: template:Shutdown() called!")
end