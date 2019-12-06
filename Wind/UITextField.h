#ifndef UI_TEXT_FIELD_H
#define UI_TEXT_FIELD_H

#include "Sprite.h"

#include <functional>

class UITextField : public Sprite {

	static std::vector<UITextField*> all;

	float et;
	bool isFocused;
	bool shiftHeld;

	unsigned cursorPosition;

	Sprite* cursor;

public:

	UITextField();

	virtual void Build();
	virtual void Initialize();

	void Focus();

	void SetCursor(Sprite* const _cursor);

	template<typename Context>
	void BindDidChange(void(Context::*callback)(UITextField*), Context* context);

	template<typename Context>
	void BindDidReturn(void(Context::*callback)(UITextField*), Context* context);

private:

	std::function<void(UITextField*)> didChangeCallback;
	std::function<void(UITextField*)> didReturnCallback;

	void DidChange(UITextField* tf);
	void DidReturn(UITextField* tf);

	void FixedUpdate(const float& dt);

	void KeyHandler(Events::Event* event);
	void TextHandler(Events::Event* event);

	void MouseOnClick(Entity* target);

	void UpdateCursorOffset();


};

template<typename Context>
void UITextField::BindDidChange(void(Context::*callback)(UITextField*), Context* context) {
	didChangeCallback = std::bind(callback, context, std::placeholders::_1);
}

template<typename Context>
void UITextField::BindDidReturn(void(Context::*callback)(UITextField*), Context* context) {
	didReturnCallback = std::bind(callback, context, std::placeholders::_1);
}


#endif
