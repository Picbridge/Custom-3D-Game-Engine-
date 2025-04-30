#pragma once

class UITextComponent : public UIComponent
{
public:
	UITextComponent() : UIComponent() { m_type = IMGUI_ELEMENT_TYPE::TEXT; defineMember(); }

	void Update() override;

	//@brief Sets the text of the text component
	void SetText(const char* text);
	//@brief Returns the text of the text component
	const char* GetText() const;

	void SetActive(bool active) { m_active = active; if (m_pUIElement) { m_pUIElement->selected = active; } }
	bool IsActive() const { return m_active; }

	void SetAnchorLeft(bool anchorLeft) { m_anchorLeft = anchorLeft; if (m_pUIElement) { static_cast<IMGUI_TEXT_ELEMENT*>(m_pUIElement.get())->anchorLeft = anchorLeft; } }
	bool GetAnchorLeft() const { return m_anchorLeft; }

	void CreateUIElement() override;

protected:
	bool m_active = true;
	bool m_anchorLeft = false;
	void defineMember() override;
};