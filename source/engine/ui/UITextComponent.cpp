#include "../pch.h"
#include "UITextComponent.h"

void UITextComponent::Update()
{
	UIComponent::Update();
	if (m_pUIElement)
	{
		m_pUIElement->selected = m_active;
	}
}

void UITextComponent::SetText(const char* text)
{
	m_savePath = text;
	if (m_pUIElement)
		m_pUIElement->savePath = text;
}

const char* UITextComponent::GetText() const
{
	return m_savePath.c_str();
}

void UITextComponent::CreateUIElement()
{
	UIComponent::CreateUIElement(m_type, m_savePath.c_str());
	m_pUIElement->selected = m_active;
	m_pUIElement->fontName = m_fontName;
	m_pUIElement->color = m_color;
	static_cast<IMGUI_TEXT_ELEMENT*>(m_pUIElement.get())->anchorLeft = m_anchorLeft;
}

void UITextComponent::defineMember()
{
	m_getters["text"] = [this]() -> std::any { return this->GetText(); };
	m_getters["active"] = [this]() -> std::any { return this->IsActive(); };
	m_getters["font"] = [this]() -> std::any { return this->GetFont(); };
	m_getters["color"] = [this]() -> std::any { return this->GetColor(); };
	m_getters["anchorLeft"] = [this]() -> std::any { return this->GetAnchorLeft(); };

	m_setters["text"] = [this](std::any val) { this->SetText(std::any_cast<const char*>(val)); };
	m_setters["active"] = [this](std::any val) { this->SetActive(std::any_cast<bool>(val)); };
	m_setters["font"] = [this](std::any val) { this->SetFont(std::any_cast<const char*>(val)); };
	m_setters["color"] = [this](std::any val) { this->SetColor(std::any_cast<glm::vec3>(val)); };
	m_setters["anchorLeft"] = [this](std::any val) { this->SetAnchorLeft(std::any_cast<bool>(val)); };
}
