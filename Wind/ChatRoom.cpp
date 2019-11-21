#include "ChatRoom.h"

// game objects
#include "CameraObject.h"
#include "FPSLabel.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UITextField.h"
// components
#include "Transform.h"
#include "Animation.h"
#include "Render.h"
#include "Camera.h"
#include "Button.h"
#include "Script.h"
// systems
#include "AnimationSystem.h"
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "ScriptSystem.h"
// utils
#include "LoadFNT.h"
// events
#include "InputEvents.h"

#include <GLFW/glfw3.h>

#include <regex>

ChatRoom::ChatRoom() {
	components->Subscribe<Transform>(10, 1);
	components->Subscribe<Animation>(10, 1);
	components->Subscribe<Render>(10, 1);
	components->Subscribe<Text>(10, 1);
	components->Subscribe<Camera>(1, 1);
	components->Subscribe<Button>(10, 1);
	components->Subscribe<Script>(1, 1);
		
	entities->Subscribe<Sprite>(1, 1);
	entities->Subscribe<UILabel>(10, 1);
	entities->Subscribe<UIButton>(10, 1);
	entities->Subscribe<UITextField>(1, 1);
	entities->Subscribe<FPSLabel>(1, 1);
	entities->Subscribe<CameraObject>(1, 1);

	systems->Subscribe<AnimationSystem>();
	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ButtonSystem>();
	systems->Subscribe<ScriptSystem>();
}

ChatRoom::~ChatRoom() {
	delete client;
}

void ChatRoom::Awake() {
	Scene::Awake();

	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->SetSize(10);

	auto cursor = entities->Create<Sprite>();
	cursor->GetComponent<Transform>()->scale.Set(0.1f, 1.f, 1.f);
	cursor->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 1.f);

	auto back = entities->Create<UILabel>();
	back->GetComponent<Transform>()->scale.Set(20.f, 2.f, 0.f);
	back->GetComponent<Render>()->tint.Set(0.25f);

	textField = entities->Create<UITextField>();
	textField->GetComponent<Transform>()->scale.Set(19.f, 2.f, 0.f);
	textField->GetComponent<Render>()->tint.Set(0.f);
	textField->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	textField->GetComponent<Text>()->color.Set(1.f);
	textField->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_LEFT;
	textField->SetCursor(cursor);
	textField->BindDidChange(&ChatRoom::DidChangeHandler, this);
	textField->BindDidReturn(&ChatRoom::ReturnHandler, this);

	auto prompt = entities->Create<UILabel>();
	prompt->GetComponent<Transform>()->translation.y = 2.f;
	prompt->GetComponent<Transform>()->scale.Set(20.f, 1.f, 0.f);
	prompt->GetComponent<Render>()->tint.Set(0.f);
	prompt->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	prompt->GetComponent<Text>()->color.Set(0.f, 1.f, 1.f, 1.f);
	prompt->GetComponent<Text>()->scale = 0.5f;
	prompt->GetComponent<Text>()->text = "IP Address:";
	prompt->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_LEFT;

	confirm = entities->Create<UIButton>();
	confirm->GetComponent<Transform>()->translation.y = -5.f;
	confirm->GetComponent<Transform>()->scale.Set(5.f, 1.5f, 0.f);
	confirm->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 0.5f);
	confirm->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	confirm->GetComponent<Text>()->color.Set(0.f, 0.f, 0.f, 1.f);
	confirm->GetComponent<Text>()->scale = 0.65f;
	confirm->GetComponent<Text>()->text = "JOIN";
	confirm->GetComponent<Button>()->isEnabled = false;
	confirm->GetComponent<Button>()->BindHandler(&ChatRoom::MouseOverHandler, this, MOUSE_OVER);
	confirm->GetComponent<Button>()->BindHandler(&ChatRoom::MouseOutHandler, this, MOUSE_OUT);
	confirm->GetComponent<Button>()->BindHandler(&ChatRoom::MouseOnClick, this, MOUSE_CLICK);

	client = new TCP(8230);
}

void ChatRoom::DidChangeHandler(UITextField * const target) {
	auto text = target->GetComponent<Text>();
	auto& textColor = text->color;

	const std::regex ipCheck("(\\d{1,3}(\\.\\d{1,3}){3})");

	if (std::regex_match(text->text, ipCheck)) {
		textColor.Set(1.f);

		confirm->GetComponent<Button>()->isEnabled = true;
		confirm->GetComponent<Animation>()->Animate(
			AnimationBase(false, 0.2f),
			confirm->GetComponent<Render>()->tint,
			vec4f(0.f, 1.f, 1.f, 0.5f)
		);
	} else {
		textColor.Set(1.f, 0.f, 0.f, 1.f);

		confirm->GetComponent<Button>()->isEnabled = false;
		confirm->GetComponent<Animation>()->Animate(
			AnimationBase(false, 0.2f),
			confirm->GetComponent<Render>()->tint,
			vec4f(1.f, 0.f, 0.f, 0.5f)
		);
	}
}

void ChatRoom::ReturnHandler(UITextField* const target) {
	client->Send(target->GetComponent<Text>()->text);
}

void ChatRoom::MouseOverHandler(Entity* target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		1.f
	);
}

void ChatRoom::MouseOutHandler(Entity* target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		0.5f
	);
}

void ChatRoom::MouseOnClick(Entity * target) {
	client->Connect(textField->GetComponent<Text>()->text);
}
