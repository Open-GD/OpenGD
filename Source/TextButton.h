#pragma once
#include <axmol.h>
#include <string>

class TextButton : public ax::MenuItemSprite {
private:
	bool init(std::string_view text, std::string_view font, float width, float height, std::string_view bgTexture, std::function<void(TextButton*)> callback);
	void selected() override;
	void unselected() override;
	void activate() override;

	ax::ui::Scale9Sprite* m_pBG;
	ax::Label* m_pText;
	std::function<void(TextButton*)> m_fCallback;
	float m_fWidth;
	float m_fHeight;
	AX_SYNTHESIZE(float, m_fNewScale, NewScale)

public:

	/// @brief Basically a ButtonSprite implementation.
	/// @param text The button text
	/// @param font The font of the text
	/// @param width The width of the bg. Leave 0 for auto-adjusting
	/// @param height The height of the bg. Leave 0 for auto-adjusting
	/// @param bgTexture The texture of the bg
	/// @param callback The callback function, called on button release. Can be a lambda. The Node* is the button itself
	/// @return A new button
	static TextButton* create(std::string_view text, std::string_view font, float width, float height, std::string_view bgTexture, std::function<void(TextButton*)> callback);

	/// @brief Basically a ButtonSprite implementation.
	/// @param text The button text
	/// @param font The font of the text
	/// @param width The width of the bg. Leave 0 for auto-adjusting
	/// @param height The height of the bg. Leave 0 for auto-adjusting
	/// @param callback The callback function, called on button release. Can be a lambda. The Node* is the button itself
	/// @return A new button
	static TextButton* create(std::string_view text, std::string_view font, float width, float height, std::function<void(TextButton*)> callback);

	/// @brief Basically a ButtonSprite implementation.
	/// @param text The button text
	/// @param font The font of the text
	/// @param callback The callback function, called on button release. Can be a lambda. The Node* is the button itself
	/// @return A new button
	static TextButton* create(std::string_view text, std::string_view font, std::function<void(TextButton*)> callback);

	/// @brief Basically a ButtonSprite implementation.
	/// @param text The button text
	/// @param callback The callback function, called on button release. Can be a lambda. The Node* is the button itself
	/// @return A new button
	static TextButton* create(std::string_view text, std::function<void(TextButton*)> callback);

	void setText(std::string_view text);
	void setCallback(std::function<void(TextButton*)> callback);
	
	ax::Label* getLabel();
	ax::ui::Scale9Sprite* getBG();
};