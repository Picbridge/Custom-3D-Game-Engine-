#pragma once
//-----------------------
// Standard Library Headers
//-----------------------
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <unordered_set>
#include <fstream> //File input
#include <sstream> //File input
#include <unordered_map> //Hash map for storing assets by id(GameObjects, shaders and materials)
#include <cassert> //Debugging
#include <map>
#include <chrono>
#include <queue>
#include <functional>
#include <any>
#include <variant>
#include <typeindex>
#include <source_location>
#include <filesystem>
#include <type_traits>
#include <array>
#include <random>
#include <memory>
//-----------------------
// ImGui Library Headers
//-----------------------
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//-----------------------
// OpenGL Library Headers
//-----------------------
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/vec3.hpp>
#include <gl/glew.h>
#include <glfw/glfw3.h>
#include "glm/mat4x4.hpp" 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/matrix_transform.inl" //Rotate
#include "glm/gtx/euler_angles.hpp"
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
//------------------
// rapidjson Library Headers
//-----------------------
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "filereadstream.h"
//-----------------------
// ASSIMP Library Headers
//-----------------------
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//-----------------------
// FMOD Library Headers
//-----------------------
#include <fmod.hpp>
#include <fmod_errors.h>
//-----------------------
// ServiceLocator Headers
//-----------------------
#include "ServiceLocator.h"
//-----------------------
// Utility Headers
//-----------------------
#include "lua.hpp"
#include "sol/sol.hpp"
#include "lodepng.h"
#include "Definitions.h"
#include "Utils.h"
#include "objectmanager/GameObjectSystemComponentConstants.h"
#include "Time.h"
#include "Random.h"
//-----------------------
// Event Headers
//-----------------------
#include "events/Event.h"
//-----------------------
// Renderer Headers
//-----------------------
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Geometry.h"
#include "Transform.h"
#include "scenemanager/Scene.h"
#include "Renderer.h"
#include "Quaternion.h"
#include "VQS.h"
#include "Bone.h"
#include "Mesh.h"
#include "FBO.h"
//-----------------------
// Physics Headers
//-----------------------
#include "ParticleSystem.h"
#include "physics/CollisionShape.h"
#include "physics/CollisionShape_Sphere.h"
#include "physics/CollisionShape_Cuboid.h"
#include "physics/CollisionChecks.h"
#include "physics/CollisionManager.h"
//-----------------------
// GameObject Headers
//-----------------------
#include "Node.h"
#include "GameObject.h"
#include "Component.h"
//-----------------------
// Game Headers
//-----------------------
#include "Game.h"
//-----------------------
// Engine Headers
//-----------------------
#include "Skybox.h"