class CameraComponent : public Component
{
public:
	CameraComponent() : m_fov(45.0f), m_nearPlane(0.1f), m_farPlane(1000.0f),
		m_useDynamicClipping(true), m_viewport(Viewport(0, 0, 1, 1)),
		m_isActive(false), m_occlusionCulling(false), m_pTransform(nullptr), m_window(nullptr), m_xRatio(0.0f), m_yRatio(0.0f), m_wRatio(1.0f), m_hRatio(1.0f)
	{
		defineMember();
	}

	~CameraComponent();

	void Init() override;
	void Update() override;
	void Shutdown() override;

	// @brief Set camera viewport x, y, width, height
	// @param x : x position
	// @param y : y position
	// @param width : width
	// @param height : height
	void SetViewport(int x, int y, int width, int height);

	// @brief Set camera viewport
	// @param viewport : Viewport struct
	void SetViewport(Viewport viewport);

	//not implemented
	bool ToggleDynamicClipping() { m_useDynamicClipping = !m_useDynamicClipping; return m_useDynamicClipping; }
	bool ToggleOcclusionCulling() { m_occlusionCulling = !m_occlusionCulling; return m_occlusionCulling; }
	//---------------

	bool ToggleActive() { m_isActive = !m_isActive; return m_isActive; }
	
	//@brief Set the camera position
	//@param position : CameraComponent position
	void SetPosition(const glm::vec3 position);
	//@brief Set the camera rotation
	//@param rotation : CameraComponent rotation
	void SetRotation(const glm::vec3 rotation);

	//@brief Set the camera field of view
	//@param fov : Field of view
	inline void SetFOV(float fov) { m_fov = fov; }
	//@brief Set the camera near plane
	//@param nearPlane : Near plane
	inline void SetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
	//@brief Set the camera far plane
	//@param farPlane : Far plane
	inline void SetFarPlane(float farPlane) { m_farPlane = farPlane; }

	inline const bool IsDynamicClipping() const { return m_useDynamicClipping; }
	inline const bool IsOcclusionCulling() const { return m_occlusionCulling; }
	inline const bool IsActive() const { return m_isActive; }
	
	inline const float GetFOV() const { return m_fov; }
	inline const float GetNearPlane() const { return m_nearPlane; }
	inline const float GetFarPlane() const { return m_farPlane; }

	inline const glm::vec3 GetPosition() const { return m_pTransform->GetPosition(); }
	inline const glm::vec3 GetRotation() const { return m_pTransform->GetRotation(); }
	inline const glm::mat4 GetViewMatrix() const { return m_pTransform->GetView(); }
	inline const glm::mat4 GetProjectionMatrix() const { return m_pTransform->GetProjection(); }

	inline Viewport GetViewport() const { return m_dynamicViewport; }
private:
	bool m_useDynamicClipping;
	bool m_occlusionCulling;
	bool m_isActive;

	float m_nearPlane;
	float m_farPlane;
	float m_fov;

	WindowHandler* m_window;
	float m_xRatio;
	float m_yRatio;
	float m_wRatio;
	float m_hRatio;
	Transform* m_pTransform;
	Viewport m_viewport;
	Viewport m_dynamicViewport;

	void updateProjectionMatrix();
	void updateViewMatrix();

	void defineMember() override 
	{
		m_setters["fov"] = [this](std::any value) { this->SetFOV(std::any_cast<float>(value)); };
		m_setters["nearPlane"] = [this](std::any value) { this->SetNearPlane(std::any_cast<float>(value)); };
		m_setters["farPlane"] = [this](std::any value) { this->SetFarPlane(std::any_cast<float>(value)); };
		m_setters["viewport"] = [this](std::any value) { this->SetViewport(std::any_cast<Viewport>(value)); };

		m_getters["fov"] = [this]() -> std::any { return this->GetFOV(); };
		m_getters["nearPlane"] = [this]() -> std::any { return this->GetNearPlane(); };
		m_getters["farPlane"] = [this]() -> std::any { return this->GetFarPlane(); };
		m_getters["viewport"] = [this]() -> std::any { return m_viewport; };

	}
};