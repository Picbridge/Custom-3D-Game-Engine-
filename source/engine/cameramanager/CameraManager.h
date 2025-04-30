#pragma once
class CameraComponent;

class CameraManager
{
public:
	CameraManager();
	~CameraManager();

	void Init();
	void Update();
	void Shutdown();

	// @brief Add camera to the camera manager
	// @param name : CameraComponent name
	void AddCamera(const std::string& name = "");

	// @brief Register camera component to the camera manager(should already have GameObject body) 
	// @param camera : CameraComponent pointer
	void RegisterCameraComponent(CameraComponent* camera);

	// @brief Remove camera by name from the camera manager
	// @param camera : CameraComponent pointer
	void RemoveCamera(CameraComponent* camera);
	
	// @brief Adds active cameras to the active camera list
	void InflateActiveCameras(const std::vector<Node*>& nodes);

	// @brief Clear all cameras from the camera manager
	void ClearActiveCameras();

	// @brief Activate/Deactivate camera by name
	// @param camera : CameraComponent pointer
	// @return bool : CameraComponent activation status after toggle
	bool ToggleCamera(CameraComponent* camera);

	// @brief Set main camera by name
	// @param camera : CameraComponent pointer
	inline void SetMainCamera(CameraComponent* camera) { m_pMainCamera = camera; }

	// @brief Get camera by name
	// @param name : CameraComponent name
	// @return CameraComponent* : CameraComponent pointer
	CameraComponent* GetCameraByName(const std::string& name) const;

	std::vector<CameraComponent*> GetActiveCameras() const { return m_activeCameras; }

	//----------Engine CameraComponent Functions----------
	// @brief Set engine camera
	CameraComponent* AddEngineCamera();
	bool ToggleEngineCamera();

	// @brief snap selected camera to engine camera transform
	// @param GameObject* : Camera to snap to engine camera
	void SnapToEngineCamera(GameObject* camera);

	// @brief snap selected camera to engine camera transform
	// @param CameraComponent* : Camera to snap to engine camera
	void SnapToEngineCamera(CameraComponent* camera);

	// @brief get engine camera
	// @return GameObject* : Engine camera
	CameraComponent* GetEngineCamera() const;

	// @brief Get cameras for rendering
	std::vector<CameraComponent*> GetCamerasForRendering() const;

	// @brief Get main camera. Prioritizes the engine camera if it is active
	// @return CameraComponent* : Main camera
	inline CameraComponent* GetMainCamera() const { return m_pMainCamera; }


private:
	static CameraManager* GetInstance();
	static std::unique_ptr<CameraManager> instance;

	// map for displaying names with UI in engine
	// all cameras whithin the current scene will be stored here and is able to be accessed through UI
	std::vector<CameraComponent*> m_cameras;
	std::vector<CameraComponent*> m_activeCameras;
	std::shared_ptr<GameObject> m_pEngineCamera;

	// current main camera is the camera that gets affected by external behaviors other than rendering (e.g. physics, audio)
	CameraComponent* m_pMainCamera;

	friend class ServiceLocator;
};

