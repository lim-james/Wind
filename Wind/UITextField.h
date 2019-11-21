#ifndef UI_TEXT_FIELD_H
#define UI_TEXT_FIELD_H

#include "Sprite.h"

#include <functional>

class UITextField : public Sprite {

	float et;
	bool shiftHeld;

	unsigned cursorPosition;

	Sprite* cursor;

public:

	UITextField();

	virtual void Build();

	void SetCursor(Sprite* const _cursor);

	template<typename Context>
	void BindDidReturn(void(Context::*callback)(UITextField*), Context* context);

private:

	std::function<void(UITextField*)> didReturnCallback;

	void FixedUpdate(const float& dt);

	void KeyHandler(Events::Event* event);
	void TextHandler(Events::Event* event);

	void UpdateCursorOffset();

};

template<typename Context>
void UITextField::BindDidReturn(void(Context::*callback)(UITextField*), Context* context) {
	didReturnCallback = std::bind(callback, context, std::placeholders::_1);
}


#endif
