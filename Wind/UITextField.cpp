#include "UITextField.h"

#include "Transform.h"
#include "Render.h"
#include "Text.h"
#include "Script.h"
// Events
#include "InputEvents.h"

#include <Events/EventsManager.h>

#include <GLFW/glfw3.h>

UITextField::UITextField() {
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &UITextField::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_INPUT", &UITextField::TextHandler, this);
}


void UITextField::Build() {
	Sprite::Build();
	AddComponent<Text>();
	AddComponent<Script>();

	et = 0.f;
	shiftHeld = false;
}

void UITextField::Initialize() {
	Sprite::Initialize();
	GetComponent<Script>()->fixedUpdate = std::bind(&UITextField::FixedUpdate, this, std::placeholders::_1);
}

void UITextField::SetCursor(Sprite * const _cursor) {
	cursor = _cursor;
	UpdateCursorOffset();
}

void UITextField::FixedUpdate(const float& dt) {
	et += dt;
	cursor->GetComponent<Render>()->tint.a = Math::Abs(sin(et * 5.f));
}

void UITextField::KeyHandler(Events::Event* event) {
	const auto keyInput = static_cast<Events::KeyInput* >(event);
	if (keyInput->key == GLFW_KEY_BACKSPACE && keyInput->action != GLFW_RELEASE) {
		auto& text = GetComponent<Text>()->text;
		if (text.length() && cursorPosition > 0) {
			text.erase(text.begin() + cursorPosition - 1);
			--cursorPosition;
			UpdateCursorOffset();
			didChangeCallback(this);
		}
	} else if (keyInput->key == GLFW_KEY_ENTER && keyInput->action == GLFW_PRESS) {
			auto& text = GetComponent<Text>()->text;
		if (shiftHeld) {
			text.insert(text.begin() + cursorPosition, '\n');
			++cursorPosition;
			UpdateCursorOffset();
			didChangeCallback(this);
		} else {
			didReturnCallback(this);
			text.clear();
			cursorPosition = 0;
			UpdateCursorOffset();
			didChangeCallback(this);
		}
	} else if (keyInput->key == GLFW_KEY_LEFT && keyInput->action != GLFW_RELEASE) {
		if (cursorPosition != 0) {
			--cursorPosition;
			UpdateCursorOffset();
		}
	} else if (keyInput->key == GLFW_KEY_RIGHT && keyInput->action != GLFW_RELEASE) {
		auto text = GetComponent<Text>();
		if (cursorPosition != text->text.length()) {
			++cursorPosition;
			UpdateCursorOffset();
		}
	} else if (keyInput->key == GLFW_KEY_LEFT_SHIFT) {
		if (keyInput->action == GLFW_PRESS) {
			shiftHeld = true;
		} else if (keyInput->action == GLFW_RELEASE) {
			shiftHeld = false;
		}
	}
}

void UITextField::TextHandler(Events::Event * event) {
	const auto c = static_cast<Events::TextInput* >(event)->data;
	auto& text = GetComponent<Text>()->text;
	text.insert(text.begin() + cursorPosition, c);
	++cursorPosition;
	UpdateCursorOffset();
	didChangeCallback(this);
}

void UITextField::UpdateCursorOffset() {
	auto transform = GetComponent<Transform>();
	auto text =GetComponent<Text>();
	auto font = text->GetFont();

	const float scale = text->scale;

	std::vector<float> lineOffset;
	float numLines = 0;
	vec2f size(0.f);

	const auto& content = text->text;

	for (unsigned i = 0; i <= content.size(); ++i) {
		auto& c = content[i];

		switch (c) {
		case '\0':
		case '\n':
			switch (text->paragraphAlignment) {
			case PARAGRAPH_CENTER:
				lineOffset.push_back(size.x * scale * 0.5f);
				break;
			case PARAGRAPH_RIGHT:
				lineOffset.push_back(transform->scale.x * -0.5f - size.x * -scale);
				break;
			default:
				lineOffset.push_back(transform->scale.x * 0.5f);
				break;
			}
			size.x = 0.f;
			++numLines;
			break;
		default:
			size.x += font->characters[c].xAdvance;
			break;
		}
	}

	size.y = numLines + 1;
	size.y *= font->lineHeight * text->lineSpacing * text->scale;

	GetComponent<Transform>()->scale.y = size.y;

	size.y = numLines - 1;
	size.y *= font->lineHeight * text->lineSpacing * text->scale;

	const vec3f translation = transform->GetWorldTranslation();
	vec3f position(0.f);
	position.x = translation.x - lineOffset[0];

	switch (text->verticalAlignment) {
	case ALIGN_MIDDLE:
		position.y = translation.y + size.y * 0.5f;
		break;
	case ALIGN_BOTTOM:
		position.y = translation.y - transform->scale.y * 0.5f + size.y;
		break;
	default:
		position.y = translation.y + transform->scale.y * 0.5f;
		break;
	}

	int lineNumer = 0;
	for (unsigned i = 0; i < cursorPosition; ++i) {
		const char& c = text->text[i];
		if (c == '\0') continue;

		switch (c) {
		case '\n':
			position.y -= font->lineHeight * text->lineSpacing * text->scale;
			position.x = translation.x - lineOffset[++lineNumer];
			break;
		default:
			const auto& character = font->characters[c];
			const vec3f offset = character.rect.origin * scale;

			position.x += character.xAdvance * text->characterSpacing * scale;
			break;
		}
	}

	position.z = -10.f;
	cursor->GetComponent<Transform>()->translation = position;
}


