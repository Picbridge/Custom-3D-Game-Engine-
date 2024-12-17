#pragma once
#define PI 3.141592f
#define GLFW_INCLUDE_NONE

struct WINDOW_PROPS
{
    int Width;
    int Height;
    std::string Title;
};

struct FRAME_BUFFER_PROPS
{
    int Width;
    int Height;
};

struct VERTEX_DATA
{
    std::vector<glm::vec3> vertex_buffer;
    std::vector<unsigned int> index_buffer;
};

struct NORMAL_DATA
{
    std::vector<glm::vec3> vertex_normal_buffer;
    std::vector<glm::vec3> face_normal_buffer;
    std::vector<glm::vec3> center;
};

struct UV_INFO
{
    std::vector<glm::vec2> Cylindrical;
    std::vector<glm::vec2> Spherical;
    std::vector<glm::vec2> Planar;
    std::vector<glm::vec2> Cube;
};

enum UV_TYPE
{
    PLANAR,
    CYLINDRICAL,
    SPHERICAL,
    CUBE
};

enum IMGUI_ELEMENT_TYPE 
{
    BUTTON,
    DROPDOWN_TOGGLE,
    DROPDOWN_SELECTION,
	SLIDER,
    DROPDOWN_SLIDER,
    END
};

struct IMGUI_ELEMENT
{
    std::string name;
    std::string savePath;
    IMGUI_ELEMENT_TYPE type;
    unsigned short selected;

    IMGUI_ELEMENT()
        : name(""), savePath(""), type(IMGUI_ELEMENT_TYPE::END), selected(0) {}

    IMGUI_ELEMENT(const char* _name)
        : IMGUI_ELEMENT() {
        name = _name;
    }

    virtual ~IMGUI_ELEMENT() = default;
};

struct IMGUI_BUTTON : public IMGUI_ELEMENT
{
    IMGUI_BUTTON()
        : IMGUI_ELEMENT()
    {
        type = IMGUI_ELEMENT_TYPE::BUTTON;
    }

    IMGUI_BUTTON(const char* _name)
        : IMGUI_BUTTON() {
        name = _name;
    }

    ~IMGUI_BUTTON() = default;
};

struct IHasGettersSetters 
{
public:
    //@brief Get the component setter functions
    //@return std::unordered_map<std::string, std::function<void(std::any)>> : The setter functions
    const std::unordered_map<std::string, std::function<void(std::any)>>& GetSetters() const { return m_setters; }

    //@brief Get the component getter functions
    //@return std::unordered_map<std::string, std::function<std::any()>> : The getter functions
    const std::unordered_map<std::string, std::function<std::any()>>& GetGetters() const { return m_getters; }

protected:
    std::unordered_map<std::string, std::function<void(std::any)>> m_setters;
    std::unordered_map<std::string, std::function<std::any()>> m_getters;

	virtual void defineMember() = 0;
};

struct IMGUI_SLIDER : public IMGUI_ELEMENT
{
    float min;
    float max;
    float value;

    IMGUI_SLIDER()
        : IMGUI_ELEMENT(), min(0.0f), max(1.0f), value(0.5f)
    {
        type = IMGUI_ELEMENT_TYPE::SLIDER;
    }

    IMGUI_SLIDER(const char* _name)
        : IMGUI_SLIDER() {
        name = _name;
    }

    ~IMGUI_SLIDER() = default;
};

struct IMGUI_DROPDOWN_MENU : public IMGUI_ELEMENT
{
    std::vector<std::unique_ptr<IMGUI_ELEMENT>> items;

    IMGUI_DROPDOWN_MENU()
        : IMGUI_ELEMENT()
    {
        type = IMGUI_ELEMENT_TYPE::DROPDOWN_SELECTION;
    }

    IMGUI_DROPDOWN_MENU(const char* _name)
        : IMGUI_DROPDOWN_MENU() {
        name = _name;
    }

    ~IMGUI_DROPDOWN_MENU() = default;
};