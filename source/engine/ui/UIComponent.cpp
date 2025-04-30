#include "../pch.h"
#include "UIComponent.h"
#include "../DeserializeJSON.h"
#include "TransformComponent.h"

UIComponent::UIComponent()
	: Component()
{
	defineMember();
}

UIComponent::~UIComponent()
{}

void UIComponent::Init()
{
}

void UIComponent::Update()
{
 	m_pUIElement->name = GetOwner()->GetName();
	m_pUIElement->savePath = m_savePath;
}

void UIComponent::Shutdown()
{
	m_pUIElement->savePath = m_savePath;
}

glm::vec2 UIComponent::GetPosition()
{
	return glm::vec2(GetOwner()->GetTransform()->GetPosition());
}

void UIComponent::SetPosition(glm::vec2 pos)
{
	GetOwner()->GetTransform()->SetPosition(glm::vec3(pos, 0.0f));
}

glm::vec2 UIComponent::GetScale()
{
	return glm::vec2(GetOwner()->GetTransform()->GetScale());
}

void UIComponent::SetScale(glm::vec2 scale)
{
	GetOwner()->GetTransform()->SetScale(glm::vec3(scale, 0.0f));
}

std::string UIComponent::GetSaveFilePath() const
{
	return m_savePath;
}

void UIComponent::SetSaveFilePath(const std::string& saveFilePath)
{
	m_savePath = saveFilePath;
	if (m_pUIElement)
		m_pUIElement->savePath = m_savePath;
}

int UIComponent::GetElementType() const
{
	return static_cast<int>(m_pUIElement->type);
}

void UIComponent::SetElementType(int type)
{
	m_type = static_cast<IMGUI_ELEMENT_TYPE>(type);
}

void UIComponent::UpdateSettingsElement()
{
	std::any* value = SERVICE_LOCATOR.GetSystemSettings()->GetSetting(m_pUIElement->savePath);
	if (!value) 
	{ 
		value = SERVICE_LOCATOR.GetSystemSettings()->GetSetting(m_pUIElement->name);
	}
	// Checks if UIElement is associated with a system setting
	// if so, updates UI element with the settings value
	if (value) 
	{
		switch (m_pUIElement->type)
		{
		case IMGUI_ELEMENT_TYPE::SLIDER:
			static_cast<IMGUI_SLIDER*>(m_pUIElement.get())->value = std::any_cast<float>(*value);
			break;
		case IMGUI_ELEMENT_TYPE::BUTTON_TOGGLE:
			m_pUIElement->selected = std::any_cast<bool>(*value);
			break;
		case IMGUI_ELEMENT_TYPE::TEXT:
			std::string settingType = SERVICE_LOCATOR.GetSystemSettings()->GetSettingType(m_pUIElement->name);
			if (settingType[0] != '\0') 
			{
				if (settingType == "int") 
				{
					std::string val = std::to_string(std::any_cast<int>(*value));
					m_savePath = val;
					if (m_pUIElement) { m_pUIElement->savePath = val; }
				}
				else if (settingType == "float")
				{
					std::string str = std::to_string(std::any_cast<float>(*value));
					if (str[0] == '-') 
					{
						m_savePath = "NA";
						if (m_pUIElement) { m_pUIElement->savePath = "NA"; }
					}
					else 
					{
						int index = str.find('.');
						index += 3;
						str = str.substr(0, index);
						m_savePath = str;
						if (m_pUIElement) { m_pUIElement->savePath = str; }
					}
				}
			}
		}
	}
}

void UIComponent::defineMember()
{
	m_getters["elementType"] = [this]() -> std::any { return this->GetElementType(); };
	m_getters["saveFilePath"] = [this]() -> std::any { return this->GetSaveFilePath(); };
	m_getters["font"] = [this]() -> std::any { return this->GetFont(); };
	m_getters["color"] = [this]() -> std::any { return this->GetColor(); };

	m_setters["elementType"] = [this](std::any val) { this->SetElementType(std::any_cast<int>(val)); };
	m_setters["saveFilePath"] = [this](std::any val) { this->SetSaveFilePath(std::any_cast<std::string>(val)); };
	m_setters["font"] = [this](std::any val) { this->SetFont(std::any_cast<const char*>(val)); };
	m_setters["color"] = [this](std::any val) { this->SetColor(std::any_cast<glm::vec3>(val)); };
}

void UIComponent::CreateUIElement(IMGUI_ELEMENT_TYPE type, const char* saveFilePath)
{
	m_type = type;
	m_savePath = saveFilePath;

	switch (type)
	{
	case IMGUI_ELEMENT_TYPE::BUTTON_ACTION:
		m_pUIElement = std::make_unique<IMGUI_BUTTON>(IMGUI_BUTTON());
		m_pUIElement->type = IMGUI_ELEMENT_TYPE::BUTTON_ACTION;
		break;
	case IMGUI_ELEMENT_TYPE::BUTTON_TOGGLE:
		m_pUIElement = std::make_unique<IMGUI_BUTTON>(IMGUI_BUTTON());
		m_pUIElement->savePath = m_savePath;
		break;
	case IMGUI_ELEMENT_TYPE::SLIDER:
		m_pUIElement = std::make_unique<IMGUI_SLIDER>(IMGUI_SLIDER());
		m_pUIElement->savePath = m_savePath;
		break;
	case IMGUI_ELEMENT_TYPE::TEXT:
		m_pUIElement = std::make_unique<IMGUI_TEXT_ELEMENT>(IMGUI_TEXT_ELEMENT());
		m_pUIElement->type = IMGUI_ELEMENT_TYPE::TEXT;
		m_pUIElement->savePath = m_savePath;
		break;
	}

	m_pUIElement->name = GetOwner()->GetName();
}

void UIComponent::CreateUIElement()
{
	CreateUIElement(m_type, m_savePath.c_str());
	m_pUIElement->fontName = m_fontName;
	m_pUIElement->color = m_color;
}

void UIComponent::SetElementState(const char* saveFilePath)
{
	m_pUIElement->savePath = m_savePath;
	DeserializeJSON::GetElementState(m_pUIElement.get());
}

void UIComponent::SetColor(glm::vec3 _color)
{
	m_color = _color;
	if (m_pUIElement)
	{
		static_cast<IMGUI_TEXT_ELEMENT*>(m_pUIElement.get())->color = m_color;
	}
}

const glm::vec3 UIComponent::GetColor() const

{
	if (m_pUIElement)
	{
		return m_pUIElement->color;
	}
	else
	{
		return m_color;
	}
}

void UIComponent::SetFont(const std::string& _fontPath)
{
	m_fontName = _fontPath;
	if (m_pUIElement)
	{
		m_pUIElement->fontName = m_fontName;
	}
}
